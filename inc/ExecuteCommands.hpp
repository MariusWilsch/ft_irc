/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecuteCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:05:52 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/25 20:18:34 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTECOMMANDS_HPP
#define EXECUTECOMMANDS_HPP

#include<iostream>
#include "message.hpp"
#include "server.hpp"
#include "client.hpp"


/*
    * Start with implementing the commmand privmsg: the command takes <Target> <Message> as parameter.
    * Example "PRIVMSG PrivateChnnel :  Wa hamid jiblya lizar li khdity, okhit byed"
*/

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
    static void     mode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket);
    static void     privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
};

#endif