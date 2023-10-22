/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:13:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/21 23:19:15 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

int joinParser(std::vector<string> &ChannelNames, std::vector<string> &ChannelKeys, Message &ProcessMessage){
	if (ProcessMessage.getParams().size() == 0 || ProcessMessage.getParams()[0].empty())
		return (-1);
	string param = ProcessMessage.getParams()[0];
	if (ProcessMessage.getParams().size() == 1 && param.compare("0") == 0){
		return (0);
	}
	if (ProcessMessage.getParams().size() < 1 || ExecuteCommands::whiteCheck(ProcessMessage.getParams()[0]))
		return (-1);
	for (size_t i = 0; i < ProcessMessage.getParams().size(); i++) {
		param = ProcessMessage.getParams()[i];
		if (param[0] == '#')
			ChannelNames.push_back(param);
		else if (!ExecuteCommands::whiteCheck(param))
			ChannelKeys.push_back(param);
	}
	if (ChannelKeys.size() > ChannelNames.size())
		return (-1);
	for (unsigned int i = ChannelKeys.size(); i < ChannelNames.size(); i++){
		ChannelKeys.push_back("");
	}
	return (1);
}

// TODO: Refactor Suggestion
// 1. We directly access the ChannelManager and the map of channels.
// 2. For each channel, we try to remove the client from both the client list and the operators list. The removeClient and removeOperator functions should handle the case where the client is not present, so there's no need to check existence beforehand.
// 3/ After the removal operations, if the channel is empty, it's deleted. If not, we move to the next channel.

// void leaveChannels(ServerReactor &_serverReactor, int clientSocket) {
//     ChannelManager &channelManager = _serverReactor.getChannelManager();
//     map<string, ChannelData> &channels = channelManager.getChannels();

//     for (auto it = channels.begin(); it != channels.end();) {
//         ChannelData &channel = it->second;

//         channel.removeClient(clientSocket);
//         channel.removeOperator(clientSocket);

//         if (channel.getOperators().empty() && channel.getClientSockets().empty()) {
//             it = channels.erase(it);
//         } else {
//             ++it;
//         }
//     }
// }

void    leaveChannels(ServerReactor &_serverReactor, int clientSocket){
	map<string, ChannelData>::iterator  it;
	map<string, ChannelData> &m = _serverReactor.getChannelManager().getChannels();
	for (it = m.begin(); it != m.end(); it++)
	{
		ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(it->first);
		set<int>::iterator RemoveIt;
		RemoveIt = it->second.getClientSockets().find(clientSocket);
		if (RemoveIt != it->second.getClientSockets().end())
			it->second.removeClient(*RemoveIt);
		RemoveIt = it->second.getOperators().find(clientSocket);
		if (RemoveIt != it->second.getOperators().end())
			it->second.removeOperator(*RemoveIt);
		std::vector<string> params;
		params.push_back(channel.getName());
		ExecuteCommands::informMembers(channel.getClientSockets(), _serverReactor.createMsg(_serverReactor.getClientDataFast(clientSocket), "PART", params));
	}
	_serverReactor.getChannelManager().removeGarbageChannels();

}

bool	createNewChannel(ServerReactor &_serverReactor, int clientSocket, string ChannelName){
	ChannelData    NewChannel(ChannelName, clientSocket);

	_serverReactor.getChannelManager().addChannel(ChannelName, NewChannel);
	
	string nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
	string channelKey = "=";
	_serverReactor.sendMsg(clientSocket, _serverReactor.getClientManager().getClientData(clientSocket).getClientInfo(), "JOIN", NewChannel.getName());
	_serverReactor.sendNumericReply_FixLater(clientSocket, RPL_NAMREPLY(nickname , channelKey, NewChannel.getName(), _serverReactor.getChannelManager().createUserList(NewChannel.getName(), _serverReactor, clientSocket)));
	_serverReactor.sendNumericReply_FixLater(clientSocket, RPL_ENDOFNAMES(nickname, NewChannel.getName()));

	return (true);
}

bool	joinPrivateChannel(ServerReactor &_server, int clientSocket, ChannelData& Channel, string inputKey){
	if ((inputKey.c_str() != NULL) && (Channel.getKey().compare(inputKey) == 0)){
		Channel.addClient(clientSocket);
		return(true);
	}
	_server.sendNumericReply_FixLater(clientSocket, ERR_BADCHANNELKEY(_server.getClientDataFast(clientSocket).getNickname(), Channel.getName()));
	return (false);
}

bool	joinPublicChannel(int clientSocket, ChannelData& Channel){
	Channel.addClient(clientSocket);
	return (true);
}

void ExecuteCommands::join(ServerReactor &_server, Message &ProcessMessage, int clientSocket){
	std::vector<string> ChannelNames;
	std::vector<string> ChannelKeys;

	int stat = joinParser(ChannelNames, ChannelKeys, ProcessMessage);
	if (stat == -1) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
		return;
	}
	if (stat == 0) {
		leaveChannels(_server, clientSocket);
		throw std::exception();
	}
	for (unsigned int i = 0; i < ChannelNames.size(); i++) {
		bool Joined = false;
		if (!_server.getChannelManager().channelExistence(ChannelNames[i])) {
			Joined = createNewChannel(_server, clientSocket, ChannelNames[i]);
			continue;
		}
		ChannelData&	Channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
		string			ChannelName = Channel.getName();
		if (Channel.isCLient(clientSocket))
			continue;
		if (Channel.getLimitFlag() && Channel.getClientSockets().size() >= Channel.getLimit()) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_CHANNELISFULL(ChannelName));
			continue;
		}
		if (Channel.getInviteFlag() && !Channel.isInvited(_server.getClientDataFast(clientSocket).getNickname())) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_INVITEONLYCHAN(_server.getClientDataFast(clientSocket).getNickname(), ChannelName));
			continue;
		}
		Joined = (Channel.getSecurity()) ? 
			joinPrivateChannel(_server, clientSocket, Channel, ChannelKeys[i]) : 
			joinPublicChannel(clientSocket, Channel);
		string nickname = _server.getClientManager().getClientData(clientSocket).getNickname();

		if (Joined) {
			if (Channel.getTopicFlag())
				_server.sendNumericReply_FixLater(clientSocket, RPL_TOPIC(nickname, ChannelName, Channel.getTopic()));
			else
				_server.sendNumericReply_FixLater(clientSocket, RPL_NOTOPIC(nickname, ChannelName));
			std::vector<string> params;
			params.push_back(ChannelName);
			informMembers(Channel.getClientSockets(), _server.createMsg(_server.getClientDataFast(clientSocket), "JOIN", params));
			string channelKey = "=";
			_server.sendNumericReply_FixLater(clientSocket, RPL_NAMREPLY(nickname , channelKey, ChannelName, _server.getChannelManager().createUserList(ChannelName, _server, clientSocket)));
			_server.sendNumericReply_FixLater(clientSocket, RPL_ENDOFNAMES(nickname, ChannelName));
		}
	}
	_server.printUserInformation();
}
