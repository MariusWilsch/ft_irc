/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:13:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 16:29:29 by ahammout         ###   ########.fr       */
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

// !When using join 0 with limechat, lime chat will send #0: and this means that the server will create the channel #0.
// ? Is this correct or not
// * nc will send it like this: join 0 > this means leaveChannels.
// * limeChat will send it like this: join #0 > this means create a channel with the name #0.
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
		ExecuteCommands::informMembers(channel.getClientSockets(), _serverReactor.createInfoMsg(_serverReactor.getClientDataFast(clientSocket), "PART", params));
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

// :AMSKLDN!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#ChannelNadia
bool	joinPrivateChannel(ServerReactor &_serverReactor, int clientSocket, ChannelData& Channel, string inputKey){
	if ((inputKey.c_str() != NULL) && (Channel.getKey().compare(inputKey) == 0)){
		Channel.addClient(clientSocket);
		return(true);
	}
	_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_BADCHANNELKEY(Channel.getName()));
	return (false);
}

bool	joinPublicChannel(ServerReactor &_serverReactor, int clientSocket, ChannelData& Channel, string inputKey){
	if (inputKey.empty()){
		Channel.addClient(clientSocket);
		return (true);
	}
	// Send this when the user wants to join to a public channel with  key.
	_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_BADCHANNELKEY(Channel.getName()));
	return (false);
}

void ExecuteCommands::join(ServerReactor &_server, Message &ProcessMessage, int clientSocket){
	std::vector<string> ChannelNames;
	std::vector<string> ChannelKeys;

	int stat = joinParser(ChannelNames, ChannelKeys, ProcessMessage);

	cout << "stat: " << stat << endl;
	// Handle missing parameters or malformed command
	if (stat == -1) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
		return;
	}

	// Handle request to leave all channels
	if (stat == 0) {
		leaveChannels(_server, clientSocket);
		throw std::exception();
	}

	for (unsigned int i = 0; i < ChannelNames.size(); i++) {
		bool Joined = false;  // Moved the declaration here to avoid re-declaration
		if (!_server.getChannelManager().channelExistence(ChannelNames[i])) {
			Joined = createNewChannel(_server, clientSocket, ChannelNames[i]);
			continue;  // Ensure we move to the next channel
		}

		ChannelData&	Channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
		string			ChannelName = Channel.getName();

		// If the client is already in the channel, move to the next channel
		if (Channel.isCLient(clientSocket))
			continue;

		// Various checks and error handling
		if (Channel.getLimitFlag() && Channel.getClientSockets().size() >= Channel.getLimit()) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_CHANNELISFULL(ChannelName));
			continue;
		}

		if (Channel.getInviteFlag() && !Channel.isInvited(_server.getClientDataFast(clientSocket).getNickname())) {
			_server.sendNumericReply_FixLater(clientSocket, ERR_INVITEONLYCHAN(ChannelName));
			continue;
		}

		// Join either a private or public channel based on its security setting
		Joined = (Channel.getSecurity()) ? 
			joinPrivateChannel(_server, clientSocket, Channel, ChannelKeys[i]) : 
			joinPublicChannel(_server, clientSocket, Channel, ChannelKeys[i]);

		// If the client joined the channel, send the topic and inform members
		if (Joined) {
			if (Channel.getTopicFlag())
				_server.sendNumericReply_FixLater(clientSocket, RPL_TOPIC(ChannelName, Channel.getTopic()));
			else
				_server.sendNumericReply_FixLater(clientSocket, RPL_NOTOPIC(ChannelName));
			
			informMembers(Channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "JOIN", ProcessMessage.getParams()));
			string nickname = _server.getClientManager().getClientData(clientSocket).getNickname();
			string channelKey = "=";
			// _server.sendMsg(clientSocket, _server.getClientManager().getClientData(clientSocket).getClientInfo(), "JOIN", ChannelName);
			_server.sendNumericReply_FixLater(clientSocket, RPL_NAMREPLY(nickname , channelKey, ChannelName, _server.getChannelManager().createUserList(ChannelName, _server, clientSocket)));
			_server.sendNumericReply_FixLater(clientSocket, RPL_ENDOFNAMES(nickname, ChannelName));
		}
	}
	// Print user information
	_server.printUserInformation();
}
