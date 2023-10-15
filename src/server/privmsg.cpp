/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 11:16:22 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void	ExecuteCommands::privmsg(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {
	
	cout << "size" << ProcessMessage.getParams().size() << endl;

	if (ProcessMessage.getParams().size() < 2)
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));;
		
	vector<string> params = ProcessMessage.getParams();
	const string& nickSender = _server.getClientDataFast(clientSocket).getNickname();
	
	if (params[0].empty())
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NORECIPIENT(nickSender));
		// return _server.sendNumericReply(clientSocket, "411", params[0], "No recipient given (PRIVMSG)"); // TODO: USE MACRO FOR ERRORS
		
	if (params[1].empty())
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NOTEXTTOSEND(nickSender));
		// return _server.sendNumericReply(clientSocket, "412", params[0], "No text to send"); // TODO: USE MACRO FOR ERRORS

	int targetFD = _server.getClientManager().getClientSocketByNick(params[0]);
	bool isUser = (targetFD != -1);

	// FIXME: What if channel and user doesn't exist then we should throw ERR_NO_SUCH_NICK

	// Error if neither channel nor user exists
	if (!_server.doesChannelExist(params[0]) && !isUser)
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender));
			// return _server.sendNumericReply(clientSocket, "401", params[0], "No such nick/channel"); // TODO: USE MACRO FOR ERRORS
	

	// Code blow is working 
	if (isUser) // If User 
		return _server.sendMsg(targetFD, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", params[0], params[1]);
	
	set<int> channelMembers = _server.getChannelManager().getChannelByName(params[0]).getClientSockets();
	for (set<int>::iterator it = channelMembers.begin(); it != channelMembers.end(); it++) {
		if (*it == clientSocket)
				continue ;
		_server.sendMsg(*it, _server.getClientManager().getClientData(*it).getClientInfo() ,"PRIVMSG", params[0], params[1]); 
	}
}

