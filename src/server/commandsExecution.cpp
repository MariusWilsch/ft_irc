/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/23 12:06:18 by ahammout         ###   ########.fr       */
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
 * @brief 
 * 
 * @param channelMembers All sockets of the members of the channel
 * @param message Use createInfoMessage to create the message
 */
void    ExecuteCommands::informMembers(set <int> channelMembers, string message) {
		for (set<int>::iterator it = channelMembers.begin() ; it !=  channelMembers.end() ; ++it)
				send(*it, message.c_str(), message.length(), 0);
}

// TODO: Refactor Suggestion - Create a map of CommandProperties and use it to execute the commands

// void ServerReactor::createPropertiesMap() {
//     _properties["PASS"] = CommandProperties(1, true, ExecuteCommands::pass);
//     _properties["NICK"] = CommandProperties(1, true, ExecuteCommands::nick);
//     _properties["USER"] = CommandProperties(3, false, ExecuteCommands::user);
//     _properties["JOIN"] = CommandProperties(1, true, ExecuteCommands::join);
//     _properties["PRIVMSG"] = CommandProperties(1, false, ExecuteCommands::privmsg);
//     // _properties["OPER"] = CommandProperties(2, true, ExecuteCommands::invite); // Not mapped to a function as it wasn't provided
//     _properties["KICK"] = CommandProperties(1, false, ExecuteCommands::kick);
//     _properties["INVITE"] = CommandProperties(2, true,  ExecuteCommands::invite); 
//     _properties["MODE"] = CommandProperties(2, true, ExecuteCommands::mode); 
//     _properties["PART"] = CommandProperties(2, true, ExecuteCommands::part);

//     _properties["HELP"] = CommandProperties(1, true); // Not mapped to a function as it wasn't provided in the snippet
// }
// struct CommandProperties {
//     int                mandatoryParams;
//     bool            ignoreTrailing;
//     void (executeCommand)(ServerReactor &, Message &, int);

//     CommandProperties();
//     CommandProperties(int m, bool i,  void (func)(ServerReactor &, Message &, int) = nullptr);
// };
// void ExecuteCommands::execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
// {
//         string command = ProcessMessage.getCommand();
//         try 
//         {
//                 if (ProcessMessage.getFatal()){
//                         string Err = ERR_UNKNOWNCOMMAND();
//                         send(clientSocket, Err.c_str(), Err.size(), 0);
//                         throw std::exception();
//                 }

//             map<std::string, CommandProperties>::iterator it = _serverReactor._properties.find(command);
//              if (it != _serverReactor._properties.end() && it->second.executeCommand) {
//                     it->second.executeCommand(_serverReactor, ProcessMessage, clientSocket);
//             }
//         }
//         catch (std::exception &ex){}
// }

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
		std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE" , "PART", }; // TODO: Add function to Command Properties (REFACTOR)
		void(*FunctionPointers[10])(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) = {pass, nick, user, join, privmsg, kick, invite, topic, mode, part};
		if (ProcessMessage.getCommand().compare("MAN") == 0)
				return (ManBot::man(_serverReactor, ProcessMessage, clientSocket));
		for (unsigned int i = 0; i < 10; i++) {
			if (ProcessMessage.getCommand().compare(commands[i]) == 0)
				FunctionPointers[i](_serverReactor, ProcessMessage, clientSocket);
		}
		} catch (std::exception &ex) {}
		// 	 catch (const ServerException &e) {
    //     _server.sendNumericReply_FixLater(clientSocket, e.what());
    // }
}