/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:28 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/24 17:20:02 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void     ExecuteCommands::topic(ServerReactor &_server, Message &ProcessMessage, int clientSocket){
    ClientData  &client = _server.getClientDataFast(clientSocket);
	const vector<string>& params = ProcessMessage.getParams();

	if (params.empty()) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProcessMessage.getCommand()));
		throw std::exception();
    }
    string channelName = params[0];
    if (!_server.doesChannelExist(channelName)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        throw std::exception();
    }
    ChannelData&	Channel = _server.getChannelManager().getChannelByName(channelName);
    if (!Channel.isCLient(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channelName));
        throw std::exception();
    }
    if (params.size() == 1) {
        if (Channel.getTopicFlag())
            _server.sendNumericReply_FixLater(clientSocket, RPL_TOPIC(client.getNickname(), channelName, Channel.getTopic()));
        else
            _server.sendNumericReply_FixLater(clientSocket, RPL_NOTOPIC(client.getNickname(), channelName));
        return ;
    }
    if (!Channel.isOperator(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        return ;
    }
    if (params.size() == 2 && whiteCheck(params[1])){
        Channel.setTopic("");
        Channel.setTopicFlag(false);
    }
    Channel.setTopic(params[1]);
    Channel.setTopicFlag(true);
    std::vector <string> param;
    param.push_back(params[0]);
    param.push_back(params[1]);
    informMembers(Channel.getClientSockets(), _server.createMsg(client, "TOPIC", param));
}