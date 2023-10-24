/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecuteCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:05:52 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/24 16:56:51 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTECOMMANDS_HPP
#define EXECUTECOMMANDS_HPP

#include "main.hpp"
#include "message.hpp"
#include "server.hpp"
#include "client.hpp"
#include <vector>

class Message;
class ServerReactor;
class ExecuteCommands
{
    public:
    ExecuteCommands();
    ~ExecuteCommands();

    //*********************************** AUTHENTICATION COMMANDS ***********************************//
    static void     execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     nick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     pass(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);

    //*********************************** CHANNEL OPERATOR COMMANDS ***********************************//
    static void     join(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     part(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     kick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     mode(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     invite(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     topic(ServerReactor &_server, Message &Message, int clientSocket);
    static void     privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    
    //****************************************** UTILS METHODS ***************************************//
    static void     informMembers(set <int> clientSockets, string message);
    static bool     whiteCheck(string str);
};

#endif
