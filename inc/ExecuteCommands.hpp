/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecuteCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:05:52 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/13 19:03:34 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTECOMMANDS_HPP
#define EXECUTECOMMANDS_HPP

#include<iostream>
#include "message.hpp"
#include "server.hpp"
#include "client.hpp"

class Message;
class ExecuteCommands
{
    public:
    ExecuteCommands();
    ~ExecuteCommands();


    static void     execute(Message &ProcessMessage, ClientManager &_ClientManager, int clientSocket);
    static void     nick(Message &ProcessMessage, ClientManager &_ClientManager, int clientSocket);
    static void     user(Message &ProcessMessage, ClientManager &_ClientManager, int clientSocket);
};

#endif