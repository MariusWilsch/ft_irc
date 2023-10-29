/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:20:25 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/28 15:53:52 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

void     ExecuteCommands::invite(ServerReactor &_server, Message &Message, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
    if (Message.getParams().size() < 2) {
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), Message.getCommand()));
        return ;
    }
	vector<string>	params = Message.getParams();
    string			channelName = params[1];
	ChannelData&	channel = _server.getChannelManager().getChannelByName(channelName);
	const string &nick = client.getNickname();
	if (channelName[0] != '#') {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, channelName));
		return ;
	}
	if (!_server.doesChannelExist(params[1])){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, channelName));
		return ;
	}
	if (!channel.isCLient(clientSocket)) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nick, channelName));
		return ;
	}
    if (!channel.isOperator(clientSocket)) {
        _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        return ;
    }
	if (_server.getClientManager().MatchNickName(channel.getClientSockets(), params[0]) != -1) { 
		_server.sendNumericReply_FixLater(clientSocket, ERR_USERONCHANNEL(params[0], channelName));
	return ;
	}
	int targetFD = _server.getClientManager().getClientSocketByNick(params[0]);
	if (targetFD == -1) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(params[0], channelName));
		return ;
	}
	channel.addGuest(params[0]);
	_server.sendMsg_FixLater(targetFD, _server.createMsg(client, Message.getCommand(), params));
	_server.sendNumericReply_FixLater(clientSocket, RPL_INVITING(client.getNickname(), channelName, params[0]));
}
