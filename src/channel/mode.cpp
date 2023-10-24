/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/23 11:58:03 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

// ~~~~~~~~~~~~~ ONLY THE OPERATORS ARE ALLOWED TO PERFORM SUCH OPERATION ~~~~~~~~~~/ 

bool    isDecimal(string str){
		for (unsigned int i = 0; i < str.size(); i++){
				if (!(str[i] >= '0' && str[i] <= '9'))
						return (false);
		}
		return (true);
}

// Test the size of parameter with nc punch: server.
std::vector<string>	ChannelOperatorPrivilege(ServerReactor &_serverReactor, Message &msg, int clientSocket){
	ClientData  &client = _serverReactor.getClientDataFast(clientSocket);
	ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(msg.getParams()[0]);
	const string& nickSender = client.getNickname();

	if (msg.getParams().size() < 3){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname() ,msg.getCommand()));
		throw std::exception();
	}
	if (!channel.isCLient(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nickSender, channel.getName()));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
		throw std::exception();
	}
	string nickName = msg.getParams()[2];
	int ClSocket = _serverReactor.getClientManager().MatchNickName(channel.getClientSockets(), nickName);
	if (ClSocket == -1){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, msg.getParams()[0]));
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_USERNOTINCHANNEL(nickSender, nickName, channel.getName()));
		throw std::exception();
	}
	if (msg.getParams()[1].compare("+o") == 0)
		channel.addOperator(ClSocket);
	else if (msg.getParams()[1].compare("-o") == 0)
		channel.removeOperator(ClSocket);
	std::vector<string> params;
	params.push_back(msg.getParams()[0]);
	params.push_back(msg.getParams()[1]);
	params.push_back(msg.getParams()[2]);
	return (params);
}


std::vector<string>	ChanneLimitMode(ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(msg.getParams()[0]);

	if (msg.getParams().size() < 2){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname() ,msg.getCommand()));
		throw std::exception();
	}
	int limit = -1;
	string channelName = msg.getParams()[0];
	if (!channel.isCLient(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channelName));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
		throw std::exception();
	}
	if (msg.getParams().size() == 3)
	{
		if (!isDecimal(msg.getParams()[2])){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
			throw std::exception();
		}
		limit = std::atoi(msg.getParams()[2].c_str());
	}
	if (msg.getParams()[1].compare("+l") == 0 && limit){
		channel.setLimitFlag(true);
		channel.setLimit(limit);
	}
	else if (msg.getParams()[1].compare("-l") == 0){
		channel.setLimitFlag(false);
		channel.setLimit(-1);
	}
	std::vector<string> params;
	params.push_back(msg.getParams()[0]);
	params.push_back(msg.getParams()[1]);
	if (msg.getParams().size() > 2)
		params.push_back(msg.getParams()[2]);
	return (params);
}

std::vector<string>    ChannelTopicMode(ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(msg.getParams()[0]);

	if (msg.getParams().size() < 2){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname() ,msg.getCommand()));
		throw std::exception();
	}
	if (!channel.isCLient(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
		throw std::exception();
	}
	string channelName = msg.getParams()[0];
	string topic;
	if (msg.getParams().size() > 2)
		topic = msg.getParams()[2];
	if (msg.getParams()[1].compare("+t") == 0 && !topic.empty()){
		channel.setTopicFlag(true);
		channel.setTopic(topic);
	}
	else if (msg.getParams()[1].compare("-t") == 0){
		channel.setTopicFlag(false);
		channel.setTopic("");
	}
	std::vector<string> params;
	params.push_back(msg.getParams()[0]);
	params.push_back(msg.getParams()[1]);
	if (msg.getParams().size() > 2)
		params.push_back(msg.getParams()[2]);
	return (params);
}

std::vector<string>	ChannelSecureMode(ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(msg.getParams()[0]);

	if (msg.getParams().size() < 2){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
			throw std::exception();
	}
	if (!channel.isCLient(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
			throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
			throw std::exception();
	}
	string  key;
	if (msg.getParams().size() == 3)
			key = msg.getParams()[2];
	if (msg.getParams()[1].compare("+k") == 0 && key.empty())
			throw std::exception();
	if (msg.getParams()[1].compare("+k") == 0 && !key.empty())
	{
			channel.setSecurity(true);
			channel.setKey(key);
	}
	else if (msg.getParams()[1].compare("-k") == 0){
			channel.setSecurity(false);
			channel.setKey("");
	}
	std::vector<string> params;
	params.push_back(msg.getParams()[0]);
	params.push_back(msg.getParams()[1]);
	if (msg.getParams().size() > 2)
		params.push_back(msg.getParams()[2]);
	return (params);
}


std::vector<string>	inviteOnly(ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(msg.getParams()[0]);

	if (msg.getParams().size() < 2){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
			throw std::exception();
	}
	if (!channel.isCLient(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
			throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
			throw std::exception();
	}
	if (msg.getParams()[1].compare("+i") == 0)
		channel.setInviteFlag(true);
	else if (msg.getParams()[1].compare("-i") == 0)
		channel.setInviteFlag(false);
	std::vector<string> params;
	params.push_back(msg.getParams()[0]);
	params.push_back(msg.getParams()[1]);
	return (params);
}

void	ExecuteCommands::mode(ServerReactor &_server, Message &ProccessMessage, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	if (ProccessMessage.getParams().size() == 1)
		throw std::exception();
	if (ProccessMessage.getParams().size() < 2) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProccessMessage.getCommand()));
		throw std::exception();
	}
	if (!_server.doesChannelExist(ProccessMessage.getParams()[0])){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), ProccessMessage.getParams()[0]));
		throw std::exception();
	}
	string mode = ProccessMessage.getParams()[1];
	std::vector<string>	params;

	if (mode.compare("+sn") == 0)
		return ;
	if (mode.compare("+i") == 0 || mode.compare("-i") == 0) {
		params = inviteOnly(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+k") == 0 || mode.compare("-k") == 0) {
		params = ChannelSecureMode (_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+t") == 0 || mode.compare("-t") == 0) {
		params = ChannelTopicMode(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+l") == 0 || mode.compare("-l") == 0) {
		params = ChanneLimitMode(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+o") == 0 || mode.compare("-o") == 0) {
		params = ChannelOperatorPrivilege(_server, ProccessMessage, clientSocket);
	}
	else {
		_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(mode));
		throw std::exception();
	}
	ChannelData &channel = _server.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);
	informMembers(channel.getClientSockets(), _server.createMsg(client, "MODE", params));
}
