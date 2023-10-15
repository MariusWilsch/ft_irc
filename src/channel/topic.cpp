/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:28 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 12:50:10 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


/*
    Command     :   Topic
    Parameters  :   #<channel>
    Trailing    :   [ <topic> ] 
*/

void     ExecuteCommands::topic(ServerReactor &_ServerReactor, Message &ProcessMessage, int clientSocket){

    string channelName = ProcessMessage.getParams()[0];
    if (ProcessMessage.getParams().size() < 1){
        _ServerReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
        throw std::exception();
    }
    if (!_ServerReactor.doesChannelExist(channelName)){
        _ServerReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(channelName));
        throw std::exception();
    }

    if (!_ServerReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
        _ServerReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channelName));
        throw std::exception();
    }

    // !Check if you can use TOPIC command without parameters to get the topic of the channel
    // if (ProcessMessage.getParams()[0].size() == 0)
    // {
    //     string Message = "Topic: ";
    //     Message.append(_ServerReactor.getChannelManager().getChannelByName(channelName).getTopic());
    //     Message.append("\n");
    //     send(clientSocket, Message.c_str(), Message.size(), 0);
    //     throw std::exception();
    // }
    string topic = ProcessMessage.getParams()[0];
    _ServerReactor.getChannelManager().getChannelByName(channelName).setTopic(ProcessMessage.getParams()[0]);

    //! Here you need to inform  all the channel members about the channel new topic.
    // string message = _ServerReactor.getClientManager().getClientData(clientSocket).getUsername();
    // message.append(" has set topic to: ");
    // message.append(topic);
    // message.append("\n");
    // ExecuteCommands::informMembers(_ServerReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);
}