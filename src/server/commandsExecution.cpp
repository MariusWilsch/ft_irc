/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/06 11:20:57 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

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
        if (ProcessMessage.getFatal())
        {
            string Err = ERR_UNKNOWNCOMMAND();
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
        else{
            //~~~~~~~~~~~~~~~~~~~~~~~~~~ ATHENTICATON COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~//
            // Replace this with a chaine of array of pointer to function and execute it the appropriate function without using if/else statements.
            if (ProcessMessage.getCommand().compare("NICK") == 0){
                nick(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("USER") == 0){
                user(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("PASS") == 0){
                pass(_serverReactor, ProcessMessage, clientSocket);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            else if (ProcessMessage.getCommand().compare("JOIN") == 0){
                join(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("MODE") == 0){
                mode(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("INVITE") == 0){
                invite(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("TOPIC") == 0){
                topic(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("KICK") == 0){
                kick(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("PART") == 0){
                part(_serverReactor, ProcessMessage, clientSocket);
            }
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            else if (ProcessMessage.getCommand().compare("PRIVMSG") == 0){
            cout << "Execute PRIVMSG command" << endl;
                privmsg(_serverReactor, ProcessMessage, clientSocket);
            }
        }
    }
    catch (std::exception &ex){}
}