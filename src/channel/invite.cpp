/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 00:35:22 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


// New design to remove if / else  if /else forest >.. 
    // *

void     ExecuteCommands::invite(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){

    string channelName = ProccessMessage.getParams()[1];
    if (ProccessMessage.getParams().size() < 2){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
        throw std::exception();
    }
    if (!_serverReactor.getChannelManager().itsChannel(channelName)){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(channelName));
        throw std::exception();
    }
    if (!_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channelName));
        throw std::exception();
    }
    if (!_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        throw std::exception();
    }
    set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets();

    if (_serverReactor.getClientManager().MatchNickName(ChannelMembers, ProccessMessage.getParams()[0]) != -1){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_USERONCHANNEL(ProccessMessage.getParams()[0], channelName));
        throw std::exception();
    }
    _serverReactor.getChannelManager().getChannelByName(channelName).addGuest(ProccessMessage.getParams()[0]);
    // Inform The members of the channel about this event.
            
}
