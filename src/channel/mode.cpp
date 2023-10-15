/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 19:18:55 by ahammout         ###   ########.fr       */
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

void    inviteOnly(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
		ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

		if (ProccessMessage.getParams().size() < 2){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
				throw std::exception();
		}
		if (!channel.isCLient(clientSocket)){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channel.getName()));
				throw std::exception();
		}
		if (!channel.isOperator(clientSocket)){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
				throw std::exception();
		}
		if (ProccessMessage.getParams()[1].compare("+i") == 0)
				channel.setInviteFlag(true);
		else if (ProccessMessage.getParams()[1].compare("-i") == 0)
				channel.setInviteFlag(false);
}

void    ChannelSecureMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
		ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

		if (ProccessMessage.getParams().size() < 2){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
				throw std::exception();
		}
		if (!channel.isCLient(clientSocket)){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channel.getName()));
				throw std::exception();
		}
		if (!channel.isOperator(clientSocket)){
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
				throw std::exception();
		}
		string  key;
		if (ProccessMessage.getParams().size() == 3)
				key = ProccessMessage.getParams()[2];
		if (ProccessMessage.getParams()[1].compare("+k") == 0 && !key.empty()){
				channel.setSecurity(true);
				channel.setKey(key);
		}
		else if (ProccessMessage.getParams()[1].compare("-k") == 0 && key.empty()){
				channel.setSecurity(false);
				channel.setKey("");
		}
}

void    ChannelTopicMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
		ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

		if (ProccessMessage.getParams().size() < 2){
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
			throw std::exception();
		}
		if (!channel.isCLient(clientSocket)){
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channel.getName()));
			throw std::exception();
		}
		if (!channel.isOperator(clientSocket)){
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
			throw std::exception();
		}
		string channelName = ProccessMessage.getParams()[0];
		string topic;
		if (ProccessMessage.getParams().size() == 3)
			topic = ProccessMessage.getParams()[2];
		if (ProccessMessage.getParams()[1].compare("+t") == 0 && !topic.empty()){
			channel.setTopicFlag(true);
			channel.setTopic(topic);
		}
		else if (ProccessMessage.getParams()[1].compare("-t") == 0 && topic.empty()){
			channel.setTopicFlag(false);
			channel.setTopic("");
		}
}

void ChanneLimitMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
	ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

	if (ProccessMessage.getParams().size() < 2){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
		throw std::exception();
	}
	int limit = -1;
	string channelName = ProccessMessage.getParams()[0];
	if (!channel.isCLient(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channelName));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
		throw std::exception();
	}
	if (ProccessMessage.getParams().size() == 3)
	{
		if (!isDecimal(ProccessMessage.getParams()[2])){
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
			throw std::exception();
		}
		limit = std::atoi(ProccessMessage.getParams()[2].c_str());
	}
	if (ProccessMessage.getParams()[1].compare("+l") == 0 && limit){
		channel.setLimitFlag(true);
		channel.setLimit(limit);
	}
	else if (ProccessMessage.getParams()[1].compare("-l") == 0 && (limit == -1)){
		channel.setLimitFlag(false);
		channel.setLimit(-1);
	}
	else{
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
		throw std::exception();
	}
}

// Test the size of parameter with nc punch: server.
void    ChannelOperatorPrivilege(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
	ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);
	const string& nickSender = _serverReactor.getClientDataFast(clientSocket).getNickname();

	if (ProccessMessage.getParams().size() < 3){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
		throw std::exception();
	}
	if (!channel.isCLient(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channel.getName()));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channel.getName()));
		throw std::exception();
	}
	string nickName = ProccessMessage.getParams()[2];
	int ClSocket = _serverReactor.getClientManager().MatchNickName(channel.getClientSockets(), nickName);
	if (ClSocket == -1){
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, ProccessMessage.getParams()[0]));
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_USERNOTINCHANNEL(nickSender, nickName, channel.getName()));
		throw std::exception();
	}
	if (ProccessMessage.getParams()[1].compare("+o") == 0)
		channel.addOperator(ClSocket);
	else if (ProccessMessage.getParams()[1].compare("-o") == 0)
		channel.removeOperator(ClSocket);
}

void     ExecuteCommands::mode(ServerReactor &_server, Message &ProccessMessage, int clientSocket){
	if (ProccessMessage.getParams().size() == 1)
		throw std::exception();
	if (ProccessMessage.getParams().size() < 2) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
		throw std::exception();
	}
	if (!_server.doesChannelExist(ProccessMessage.getParams()[0])){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]));
		throw std::exception();
	}
	string mode = ProccessMessage.getParams()[1];
	// Handle the mode of limeChat when join command is sended by the user.
	if (mode.compare("+sn") == 0)
		return ;
	if (mode.compare("+i") == 0 || mode.compare("-i") == 0){
		inviteOnly(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+k") == 0 || mode.compare("-k") == 0){
		ChannelSecureMode (_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+t") == 0 || mode.compare("-t") == 0){
		ChannelTopicMode(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+l") == 0 || mode.compare("-l") == 0){
		ChanneLimitMode(_server, ProccessMessage, clientSocket);
	}
	else if (mode.compare("+o") == 0 || mode.compare("-o") == 0){
		ChannelOperatorPrivilege(_server, ProccessMessage, clientSocket);
	}
	else {
		_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(mode));
		throw std::exception();
	}
	ChannelData &channel = _server.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);
	informMembers(channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "MODE", ProccessMessage.getParams()));
}