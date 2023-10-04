/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/04 16:16:02 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void	ExecuteCommands::privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
	
	string target = ProcessMessage.getParams()[0];
	string trailing = ProcessMessage.getTrailing();

	if (trailing.length() == 2) { // testing // TODO: I later need to figure out what to do with \r\n at the end of messages

		string	nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
		string	message = "412 " + nickname + " :No text to send\r\n";

		cout << "Message to be sent to client" << message << endl;

		send(clientSocket, message.c_str(), message.length(), 0);
	}

	
	if (_serverReactor.getChannelManager().itsChannel(target)) { // If Channel
			cout << "Sending to all clients in channel" << endl;
	} else { // If Nickname
			cout << "Sending the message to the user" << endl;
			_serverReactor.getClientManager().doesClientExist(target);
		// Check if nickname exists
			// If exists, send message to client
			// If not exists, send error message to client
	} 
	
}
