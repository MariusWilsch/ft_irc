/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:28 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 15:31:06 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


/*
    Command     :   Topic
    Parameters  :   #<channel>
    Trailing    :   [ <topic> ] 
*/

void     ExecuteCommands::topic(ServerReactor &_ServerReactor, Message &ProcessMessage, int clientSocket){
    if (ProcessMessage.getParams().size() == 1){
        string channelName = ProcessMessage.getParams()[0];
        if (_ServerReactor.getChannelManager().itsChannel(channelName)){
            channelName.erase(0, 1);
            if (_ServerReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
                if (ProcessMessage.getParams()[0].size() == 0){
                    string Message = "Topic: ";
                    Message.append(_ServerReactor.getChannelManager().getChannelByName(channelName).getTopic());
                    Message.append("\n");
                    send(clientSocket, Message.c_str(), Message.size(), 0);
                    throw std::exception();
                }
                else{
                    string topic = ProcessMessage.getParams()[0];
                    topic.erase(remove(topic.begin(), topic.end(), '\n'), topic.end());
                    if (ExecuteCommands::whiteCheck(topic))
                        _ServerReactor.getChannelManager().getChannelByName(channelName).setTopic("NoTopic");
                    else{
                        _ServerReactor.getChannelManager().getChannelByName(channelName).setTopic(topic);
                    }
                    string message = _ServerReactor.getClientManager().getClientData(clientSocket).getUsername();
                    message.append(" has set topic to: ");
                    message.append(topic);
                    message.append("\n");
                    ExecuteCommands::informMembers(_ServerReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);
                }
            }
            else{
                string Err = ERR_NOTONCHANNEL(channelName);
                send(clientSocket, Err.c_str(), Err.size(), 0);
                throw std::exception();
            }
        }
        else{
            string Err = ERR_NOSUCHCHANNEL(channelName);
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
    }
    else{
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
}