/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/24 18:12:27 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"
#include "ManBot.hpp"

bool    ExecuteCommands::whiteCheck(string str)
{
	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
				return false;
	}
	return true;
}

/**
	* @param channelMembers All sockets of the members of the channel
	* @param message Use createInfoMessage to create the message
 */

void    ExecuteCommands::informMembers(set <int> channelMembers, string message) {
	for (set<int>::iterator it = channelMembers.begin() ; it !=  channelMembers.end() ; ++it)
		send(*it, message.c_str(), message.length(), 0);
}

void ExecuteCommands::execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
	try {
		ClientData& clientData = _serverReactor.getClientDataFast(clientSocket);
		string command = ProcessMessage.getCommand();
		string nickname = clientData.getNickname();
	
		if (clientData.isRegistered() && ProcessMessage.getFatal()) {
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNCOMMAND(nickname, command));
			throw std::exception();
		}
		if (command != "PASS" && command != "USER" && command != "NICK" && !clientData.isRegistered()) {
			if (nickname.empty())
				nickname = "*";
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTREGISTERED(nickname, command));
			throw std::exception();
		}
		std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE" , "PART", };
		void(*FunctionPointers[10])(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) = {pass, nick, user, join, privmsg, kick, invite, topic, mode, part};
		if (ProcessMessage.getCommand().compare("MAN") == 0)
				return (ManBot::man(_serverReactor, ProcessMessage, clientSocket));
		for (unsigned int i = 0; i < 10; i++) {
			if (ProcessMessage.getCommand().compare(commands[i]) == 0)
				FunctionPointers[i](_serverReactor, ProcessMessage, clientSocket);
		}
	}
	catch (std::exception &ex) {}
}