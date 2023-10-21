/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:28 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/17 00:52:29 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


/*
    Command     :   Topic
    Parameters  :   #<channel>
    Trailing    :   [ <topic> ] 
*/

// The regular user can't change the channel topic, but they can view the topic of the channel by using [ Topic #channelName ]
//:mok!~d@5c8c-aff4-7127-3c3-1c20.230.197.ip TOPIC #ch77 :Hello,World
void     ExecuteCommands::topic(ServerReactor &_server, Message &ProcessMessage, int clientSocket){

    if (ProcessMessage.getParams().size() < 1){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
        throw std::exception();
    }
    string channelName = ProcessMessage.getParams()[0];
		const string &nick = _server.getClientDataFast(clientSocket).getNickname();
		if (!_server.doesChannelExist(channelName))
		{
				_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, channelName));
        throw std::exception();
		}
		ChannelData&	Channel = _server.getChannelManager().getChannelByName(channelName);
    if (!Channel.isCLient(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nick, channelName));
        throw std::exception();
    }
    // Any member can ask for the topic of the channel, in this case w'll send RPL_TOPIC or RPL_NOTOPIC
    if (ProcessMessage.getParams().size() == 1) {
        if (Channel.getTopicFlag())
            _server.sendNumericReply_FixLater(clientSocket, RPL_TOPIC(_server.getClientDataFast(clientSocket).getNickname(), channelName, Channel.getTopic()));
        else
            _server.sendNumericReply_FixLater(clientSocket, RPL_NOTOPIC(channelName));
        return ;
    }
    // Only the operators of the channel can change the topic.
    if (!Channel.isOperator(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        return ;
    }
    if (ProcessMessage.getParams().size() == 2 && whiteCheck(ProcessMessage.getParams()[1])){
        Channel.setTopic("");
        Channel.setTopicFlag(false);
    }
		Channel.setTopic(ProcessMessage.getParams()[1]);
		Channel.setTopicFlag(true);
    std::vector <string> params;
    params.push_back(ProcessMessage.getParams()[0]);
    params.push_back(ProcessMessage.getParams()[1]);
    informMembers(Channel.getClientSockets(), _server.createMsg(_server.getClientDataFast(clientSocket), "TOPIC", params));
}