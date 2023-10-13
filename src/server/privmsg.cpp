/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/13 11:52:55 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void	ExecuteCommands::privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
	
	string target = ProcessMessage.getParams()[0];
	// string trailing = ProcessMessage.getTrailing();

	cout << "ClientSocket: " << clientSocket << endl;
	

	if (target.empty())
		return _serverReactor.sendNumericReply(clientSocket, "411", target, "No recipient given (PRIVMSG)");
	// if (trailing.empty())
	// 	return _serverReactor.sendNumericReply(clientSocket, "412", target, "No text to send");

	cout << "Channel Exist: " << _serverReactor.getChannelManager().itsChannel(target) << endl;
	cout << "Channel Exist: " << _serverReactor.doesChannelExist(target) << endl; // Try this one
	if (_serverReactor.getChannelManager().itsChannel(target)) { // If Channel
			set<int> channelMembers = _serverReactor.getChannelManager().getChannelByName(target).getClientSockets();
			for (set<int>::iterator it = channelMembers.begin(); it != channelMembers.end(); it++) {
				if (*it == clientSocket)
					continue ;
				_serverReactor.sendMsg(*it, _serverReactor.getClientManager().getClientData(*it).getClientInfo() ,"PRIVMSG", target, "");
			}
	} else {
		cout << "In Not Channel" << endl;
		int targetSocket = _serverReactor.getClientManager().getClientSocketByNick(target);
		if (_serverReactor.sendMsg(targetSocket,  _serverReactor.getClientManager().getClientData(clientSocket).getClientInfo(), "PRIVMSG", target, "") == -1)
		 	_serverReactor.sendNumericReply(clientSocket, "401", target, "No such nick/channel"); // No such nick/channel
	}
}
