/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecuteCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:05:52 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/19 02:23:22 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTECOMMANDS_HPP
#define EXECUTECOMMANDS_HPP

#include<iostream>
#include "message.hpp"
#include "server.hpp"
#include "client.hpp"

class Message;
class ServerReactor;
class ExecuteCommands
{
    public:
    ExecuteCommands();
    ~ExecuteCommands();

    //*********************************** ATHENTICATION COMMANDS ***********************************//
    static void     execute(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     nick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static void     pass(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);

    //*********************************** CHANNEL OPERATOR COMMANDS ***********************************//
    // static void     join(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    
};

#endif