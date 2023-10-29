/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/29 20:42:54 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool ExecuteCommands::partParser(std::vector<string> &ChannelNames, string &partMessage, Message &ProcessMessage) {
	vector<string> params = ProcessMessage.getParams();
	if (params.empty())
		return false;
	ChannelNames = splitReceivers(params[0]);  // Split channels using splitReceivers
	if (params.size() > 1)
		partMessage = params[1];
	return true;
}

void     ExecuteCommands::part(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {
	ClientData  &client = _server.getClientDataFast(clientSocket);
	vector<string> ChannelNames;
	string 				partMessage;

	int stat = partParser(ChannelNames, partMessage, ProcessMessage);
	if (stat == false){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProcessMessage.getCommand()));
		throw std::exception();
	}
	const string &nick = client.getNickname();
	if (ChannelNames.size() == 0 && partMessage.size() >= 1){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, partMessage[0]));
	}
	for (unsigned int i = 0; i < ChannelNames.size(); i++)
	{
		if (ChannelNames[i][0] != '#') {
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, ChannelNames[i]));
			continue;
		}
		if (!_server.doesChannelExist(ChannelNames[i])) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, ChannelNames[i]));
			continue ;
		}
		ChannelData &channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
		if (!channel.isCLient(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nick, ChannelNames[i]));
			continue ;
		}

		std::vector<string> params;
		params.push_back(ChannelNames[i]);
		params.push_back(partMessage);
		informMembers(channel.getClientSockets(), _server.createMsg(client, "PART", params));


		channel.removeClient(clientSocket);
		if (channel.isOperator(clientSocket))
			channel.removeOperator(clientSocket);
		if (channel.getClientSockets().size() == 0)
			_server.getChannelManager().removeChannel(ChannelNames[i]);
	}
}
