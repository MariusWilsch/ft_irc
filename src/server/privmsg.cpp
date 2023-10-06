/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/06 12:15:32 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"



void	ExecuteCommands::privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
	
	string target = ProcessMessage.getParams()[0];
	string trailing = ProcessMessage.getTrailing();

	if (target.empty())
		return _serverReactor.sendNumericReply(clientSocket, "411", target, "No recipient given (PRIVMSG)");
	if (trailing.empty()) // If no text to send
		return _serverReactor.sendNumericReply(clientSocket, "412", target, "No text to send");

	if (_serverReactor.getChannelManager().itsChannel(target)) { // If Channel
			set<int> channelMembers = _serverReactor.getChannelManager().getChannelByName(target.erase(0, 1)).getClientSockets();
			for (set<int>::iterator it = channelMembers.begin(); it != channelMembers.end(); it++) {
				if (*it == clientSocket)
					continue ;
				_serverReactor.sendMsg(*it, "PRIVMSG", target, trailing);
			}
	} else
			_serverReactor.sendMsg( _serverReactor.getClientManager().getClientSocketByNick(target), "PRIVMSG", target, trailing);
	// No such nick/channel
	return _serverReactor.sendNumericReply(clientSocket, "401", target, "No such nick/channel");
}
