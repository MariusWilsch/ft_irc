/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/07 17:27:47 by ahammout         ###   ########.fr       */
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

void    ExecuteCommands::informMembers(set <int> clientSockets, string message, int clientSocket){
    for (set<int>::iterator it = clientSockets.begin() ; it !=  clientSockets.end() ; it++){
        if (*it != clientSocket)
            send(*it, message.c_str(), message.size(), 0);
    }
}

void ExecuteCommands::execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    try 
    {
        if (ProcessMessage.getFatal()){
            string Err = ERR_UNKNOWNCOMMAND();
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
        else{
            std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"  , "PART"};
            void    (*FunctionPointers[10])(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) = {nick, user, pass, join, mode, invite, topic, kick, part, privmsg};
    
            if (ProcessMessage.getCommand().compare("/HELP") == 0)
                HelpBot::Help(_serverReactor, ProcessMessage, clientSocket);
            else{
               for (unsigned int i = 0; i < 10; i++){
                    if (ProcessMessage.getCommand().compare(commands[i].c_str()) == 0)
                        FunctionPointers[i](_serverReactor, ProcessMessage, clientSocket);
                }
            }
        }
    }
    catch (std::exception &ex){}
}