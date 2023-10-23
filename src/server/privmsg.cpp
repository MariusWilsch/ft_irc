/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 13:09:37 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void	ExecuteCommands::privmsg(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {


	vector<string> params = ProcessMessage.getParams();
	const string& nickSender = _server.getClientDataFast(clientSocket).getNickname();
	
	if (params.empty())
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NORECIPIENT(nickSender));
		
		
	int		targetFD = _server.getClientManager().getClientSocketByNick(params[0]);
	bool	isUser = (targetFD != -1);
	
	if (!_server.doesChannelExist(params[0]) && !isUser)
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, params[0]));
		

	if (params.size () < 2)
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NOTEXTTOSEND(nickSender));
		
	if (_server.doesChannelExist(params[0])) {
    if (_server.getClientManager().MatchNickName(_server.getChannelManager().getChannelByName(params[0]).getClientSockets(), nickSender) == -1)
        return _server.sendNumericReply_FixLater(clientSocket, ERR_CANNOTSENDTOCHAN(nickSender, params[0]));
}
	
	if (isUser) 
		return _server.sendMsg(targetFD, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", params[0], params[1]);
	// If Channel
	set<int> channelMembers = _server.getChannelManager().getChannelByName(params[0]).getClientSockets();
	for (set<int>::iterator it = channelMembers.begin(); it != channelMembers.end(); it++) {
		if (*it == clientSocket)
				continue ;
		_server.sendMsg(*it, _server.getClientManager().getClientData(*it).getClientInfo() ,"PRIVMSG", params[0], params[1]); 
	}
}

