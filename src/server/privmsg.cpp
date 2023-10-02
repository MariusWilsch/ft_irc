/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/02 17:34:47 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void     ExecuteCommands::privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    

// 412 <nickname> :No text to send
	string target = ProcessMessage.getTrailing();

	cout << target << endl;
	
	if (target.empty()) { // testing
		string message = "412 nickHere :No text to send";
		cout << message << endl;
		send(clientSocket, message.c_str(), message.length(), 0);
	}
}
