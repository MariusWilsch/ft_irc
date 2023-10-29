/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/24 18:13:24 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"



void	ExecuteCommands::privmsg(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {
	vector<string> params = ProcessMessage.getParams();
	const string& nickSender = _server.getClientDataFast(clientSocket).getNickname();
	
	if (params.empty())
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NORECIPIENT(nickSender));
	if (params.size () < 2)
		return _server.sendNumericReply_FixLater(clientSocket, ERR_NOTEXTTOSEND(nickSender));

	vector<string> targets = splitReceivers(params[0]);

	for (uint64_t i = 0; i < targets.size(); i++) {

		int		targetFD = _server.getClientManager().getClientSocketByNick(targets[i]);
		bool	isUser = (targetFD != -1);

		// User nor channel exist
		if (!_server.doesChannelExist(targets[i]) && !isUser) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, targets[i]));
			continue ;
		}
	
		// User exist but not in the same channel
		if (_server.doesChannelExist(targets[i]) && _server.getClientManager().MatchNickName(_server.getChannelDataFast(targets[i]).getClientSockets(), nickSender) == -1) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_CANNOTSENDTOCHAN(nickSender, targets[i]));
			continue;
		}
		if (isUser) {
			_server.sendMsg(targetFD, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", targets[i], params[1]);
			continue;
		}
		set<int> channelMembers = _server.getChannelDataFast(targets[i]).getClientSockets();
		for (set<int>::iterator it = channelMembers.begin(); it != channelMembers.end(); it++) {
			if (*it == clientSocket)
				continue ;
		_server.sendMsg(*it, _server.getClientManager().getClientData(*it).getClientInfo() ,"PRIVMSG", targets[i], params[1]); 
		}
	}
}
