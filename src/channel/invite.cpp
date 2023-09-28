/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/28 22:02:45 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void     ExecuteCommands::invite(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    // Check the syntax of the parameters befor executing the command
    //* SYNTAX: INVITE <nickname> #channelName
    //* Example: INVITE Archer #PrivetChannel

    if (ProccessMessage.getParams().size() == 2){
        string channelName = ProccessMessage.getParams()[1];
        channelName.erase(remove(channelName.begin(), channelName.end(), '\n'), channelName.end());
        if (_serverReactor.getChannelManager().itsChannel(channelName)){
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
                // check if the client if he is a channel member,
                set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets();
                if (_serverReactor.getClientManager().MatchNickName(ChannelMembers, ProccessMessage.getParams()[0])){                
                    string buffer = "error(443): @";
                    buffer.append(ProccessMessage.getParams()[0]);
                    buffer.append(" #");
                    buffer.append(channelName);
                    buffer.append(": is already on channel");
                    buffer.append("\n");
                    send(clientSocket, buffer.c_str(), buffer.size(), 0);
                    throw std::exception();
                }
                else{
                    _serverReactor.getChannelManager().getChannelByName(channelName).addGuest(ProccessMessage.getParams()[0]);
                }
            }
            else{
                string buffer = "error(482): #";
                buffer.append(channelName);
                buffer.append(": You're not channel operator");
                buffer.append("\n");
                send(clientSocket, buffer.c_str(), buffer.size(), 0);
                throw std::exception();
            }
        }
        else{
            string buffer = "error(403): #";
            buffer.append(channelName);
            buffer.append(" :No such channel");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
    }
    else{
        string buffer = "error(461): ";
        buffer.append(" Not enough parameters");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
}
