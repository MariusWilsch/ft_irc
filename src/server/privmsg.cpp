/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/03 11:27:47 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void	ExecuteCommands::privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
	
	string target = ProcessMessage.getTrailing();

	cout << (int)target[0] << endl;
	
	// 412 <nickname> :No text to send
	if (target.length() == 2) { // testing // TODO: I later need to figure out what to do with \r\n at the end of messages

		string	nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
		string	message = "412 " + nickname + " :No text to send\r\n";

		cout << "Message to be sent to client" << message << endl;

		send(clientSocket, message.c_str(), message.length(), 0);
	}
}
