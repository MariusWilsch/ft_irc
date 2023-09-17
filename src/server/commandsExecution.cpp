/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandsExecution.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:14 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/17 16:13:23 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

ExecuteCommands::~ExecuteCommands(){};

ExecuteCommands::ExecuteCommands(){};



void ExecuteCommands::execute(Message &ProcessMessage, ClientManager &_ClientManager, int clientSocket)
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
                nick(ProcessMessage, _ClientManager, clientSocket);
            }
            else if (ProcessMessage.getCommand().compare("USER") == 0){
                user(ProcessMessage, _ClientManager, clientSocket);
            }
            // else if (ProcessMessage.getCommand().compare("PASS") == 0){
            //     pass(ProcessMessage, _ClientManager, clientSocket);
            // }
            
            //~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL OPERATORS COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~//

        }
    }
    catch (std::exception &ex){}
}