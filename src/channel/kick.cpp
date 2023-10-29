/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:47:40 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/28 18:00:22 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void ExecuteCommands::kick(ServerReactor &_server, Message &msg, int clientSocket){
    ClientData &client = _server.getClientDataFast(clientSocket);
    const std::vector<string>& params = msg.getParams();

		cout << "Kick command" << endl;

    // Check if parameters are sufficient
    if (params.size() < 2) {
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
        throw std::exception();
    }
		
    const string &channelName = params[0];
    const string &userToKick = params[1];
    const string &comment = (params.size() > 2) ? params[2] : "You were kicked!";

		if (channelName[0] != '#')  {
				_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
				return;
		}

    // Check if channel exists
    if (!_server.doesChannelExist(channelName)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    ChannelData &channel = _server.getChannelManager().getChannelByName(channelName);



    // Check if sender is in the channel
    if (!channel.isCLient(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }

    // Check if sender is an operator in the channel
    if (!channel.isOperator(clientSocket)){
        _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }

    // Check if user to be kicked is in the channel
    int kickedID = _server.getClientManager().MatchNickName(channel.getClientSockets(), userToKick);
    if (kickedID == -1){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(client.getNickname(), userToKick));
        return;
    }

    // Inform members of the channel about the kick
    vector<string> kickParams;
		kickParams.push_back(channelName);
		kickParams.push_back(userToKick);
    informMembers(channel.getClientSockets(), _server.createMsg(client, "KICK", kickParams, comment));

    // Remove the kicked user from the channel and its operator list if applicable
    channel.removeClient(kickedID);
    if (channel.isOperator(kickedID))
        channel.removeOperator(kickedID);
}

