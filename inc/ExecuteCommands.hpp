/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecuteCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:05:52 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/07 18:13:32 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTECOMMANDS_HPP
#define EXECUTECOMMANDS_HPP

#include "main.hpp"
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
    static void     topic(ServerReactor &_ServerReactor, Message &ProcessMessage, int clientSocket);
    static void     privmsg(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    
    //****************************************** UTILS METHODS ***************************************//
    static void     informMembers(set <int> clientSockets, string message, int clientSocket);
    static bool     whiteCheck(string str);
};

#endif



//    if (ProcessMessage.getCommand().compare("NICK") == 0){
//         nick(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("USER") == 0){
//         user(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("PASS") == 0){
//         pass(_serverReactor, ProcessMessage, clientSocket);
//     }
//     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//     else if (ProcessMessage.getCommand().compare("JOIN") == 0){
//         join(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("MODE") == 0){
//         mode(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("INVITE") == 0){
//         invite(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("TOPIC") == 0){
//         topic(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("KICK") == 0){
//         kick(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("PART") == 0){
//         part(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("PRIVMSG") == 0){
//         privmsg(_serverReactor, ProcessMessage, clientSocket);
//     }
//     else if (ProcessMessage.getCommand().compare("/HELP") == 0){
//         cout << "you're using the HELP bot now." << endl;
//         HelpBot::Help(_serverReactor, ProcessMessage, clientSocket);
//     }