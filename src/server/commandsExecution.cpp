/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/28 22:47:43 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

ExecuteCommands::~ExecuteCommands(){};

ExecuteCommands::ExecuteCommands(){};

void ExecuteCommands::execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    try 
    {
        if (ProcessMessage.getFatal())
        {
            string buffer = "error(421): ";
            buffer.append(" Unknown command");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
        else{
            //~~~~~~~~~~~~~~~~~~~~~~~~~~ ATHENTICATON COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~//
            if (ProcessMessage.getCommand().compare("NICK") == 0){
                nick(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("USER") == 0){
                user(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("PASS") == 0){
                pass(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("JOIN") == 0){
                join(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("MODE") == 0){
                mode(_serverReactor, ProcessMessage, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("INVITE") == 0){
                invite(_serverReactor, ProcessMessage, clientSocket);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            else if (ProcessMessage.getCommand().compare("TOPIC") == 0){
                cout << "Execute TOPIC command" << endl;
            }
            else if (ProcessMessage.getCommand().compare("KICK") == 0){
                cout << "Execute KICK command" << endl;
            }
            else if (ProcessMessage.getCommand().compare("PRIVMSG") == 0){
                cout << "Execute PRIVMSG command" << endl;
            }
        }
    }
    catch (std::exception &ex){}
}