/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 13:16:27 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"
#include "HelpBot.hpp"

ExecuteCommands::~ExecuteCommands(){};

ExecuteCommands::ExecuteCommands(){};

bool    ExecuteCommands::whiteCheck(string str)
{
		for (unsigned int i = 0; i < str.length(); i++)
		{
				if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
						return false;
		}
		return true;
}

void    ExecuteCommands::informMembers(set <int> clientSockets, string message, int clientSocket) {

	
	
		for (set<int>::iterator it = clientSockets.begin() ; it !=  clientSockets.end() ; it++){
				if (*it == clientSocket)
						continue;
				send(*it, message.c_str(), message.length(), 0);
		}
}

void ExecuteCommands::execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
		try {
			ClientData& clientData = _serverReactor.getClientDataFast(clientSocket);
			string command = ProcessMessage.getCommand();
			string nickname = clientData.getNickname();
			if (clientData.getRegistration() && ProcessMessage.getFatal()) {
					_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNCOMMAND(nickname, command));
					throw std::exception();
			}
			if (command != "PASS" && command != "USER" && command != "NICK" && !clientData.getRegistration()) {
					if (nickname.empty())
						nickname = "*";
					_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTREGISTERED(nickname, command));
					throw std::exception();
			}
			std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"  , "PART", }; // TODO: Add function to Command Properties
			void(*FunctionPointers[10])(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) = {pass, nick, user, join, privmsg, kick, invite, topic, mode, part};
		
			if (ProcessMessage.getCommand().compare("HELP") == 0)
					HelpBot::Help(_serverReactor, ProcessMessage, clientSocket);
			else {
				 for (unsigned int i = 0; i < 10; i++) {
							if (ProcessMessage.getCommand().compare(commands[i]) == 0)
								FunctionPointers[i](_serverReactor, ProcessMessage, clientSocket);
					}
			}



			} catch (std::exception &ex){}
}