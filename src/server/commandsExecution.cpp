/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/02 17:30:54 by mwilsch          ###   ########.fr       */
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
//             else if (ProcessMessage.getCommand().compare("TOPIC") == 0){
// =                topic(_serverReactor, ProcessMessage, clientSocket);
//             }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            else if (ProcessMessage.getCommand().compare("KICK") == 0){
                cout << "Execute KICK command" << endl;
            }
            else if (ProcessMessage.getCommand().compare("PRIVMSG") == 0){
                cout << "Execute PRIVMSG command" << endl;
								privmsg(_serverReactor, ProcessMessage, clientSocket);
            }
        }
    }
    catch (std::exception &ex){}
}