
#include "ExecuteCommands.hpp"

bool    isDecimal(string str){
	for (unsigned int i = 0; i < str.size(); i++){
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (false);
	}
	return (true);
}

void	ChannelOperatorPrivilege(std::vector<string> params, ServerReactor &_serverReactor, Message &msg, int clientSocket){
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
		_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_USERNOTINCHANNEL(nickSender, nickName, channel.getName()));
		throw std::exception();
	}
	if (params[1].compare("+o") == 0)
		channel.addOperator(ClSocket);
	else if (params[1].compare("-o") == 0)
		channel.removeOperator(ClSocket);
}


void	ChanneLimitMode(std::vector<string> params, ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(msg.getParams()[0]);

	if (msg.getParams().size() < 2){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname() ,msg.getCommand()));
		throw std::exception();
	}
	int limit = -1;
	string channelName = params[0];
	if (!channel.isCLient(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(client.getNickname(), channelName));
		throw std::exception();
	}
	if (!channel.isOperator(clientSocket)){
		_server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
		throw std::exception();
	}
	if (params.size() == 3)
	{
		if (!isDecimal(params[2])){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
			throw std::exception();
		}
		limit = std::atoi(params[2].c_str());
	}
	if (params[1].compare("+l") == 0 && limit == -1)
		throw std::exception();
	if (params[1].compare("+l") == 0 && limit){
		channel.setLimitFlag(true);
		channel.setLimit(limit);
	}
	else if (params[1].compare("-l") == 0){
		channel.setLimitFlag(false);
		channel.setLimit(-1);
	}
}

void	ChannelSecureMode(std::vector<string> params, ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(params[0]);

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
	if (params.size() == 3)
		key = params[2];
	if (params[1].compare("+k") == 0 && key.empty())
		throw std::exception();
	if (params[1].compare("+k") == 0 && !key.empty()) {
		channel.setSecurity(true);
		channel.setKey(key);
	}
	else if (params[1].compare("-k") == 0) {
		channel.setSecurity(false);
		channel.setKey("");
	}
}

void   ChannelTopicMode(std::vector<string> params, ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(params[0]);

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
	string channelName = params[0];

	if (params[1].compare("+t") == 0)
		channel.setTopicRestriction(true);

	else if (params[1].compare("-t") == 0)
		channel.setTopicRestriction(false);
}

void	inviteOnly(std::vector<string> params, ServerReactor &_server, Message &msg, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	ChannelData &channel = _server.getChannelManager().getChannelByName(params[0]);

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
	if (params[1].compare("+i") == 0)
		channel.setInviteFlag(true);
	else if (params[1].compare("-i") == 0)
		channel.setInviteFlag(false);
}

std::vector<string>	modeParser(string param){
	std::vector<string> modes;
	char sign;

	sign = param[0];
	if (sign != '-' && sign != '+')
		return (modes);
	for (size_t i = 1; i < param.size(); i++){
		string mode;
		mode.push_back(sign);
		mode.push_back(param[i]);
		modes.push_back(mode);
	}
	return (modes);
}

void	ExecuteCommands::mode(ServerReactor &_server, Message &ProccessMessage, int clientSocket){
	ClientData  &client = _server.getClientDataFast(clientSocket);
	vector<string> param = ProccessMessage.getParams();
	if (param.size() == 1)
		throw std::exception();
	if (param.size() < 2) {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProccessMessage.getCommand()));
		throw std::exception();
	}
	if (param[0][0] != '#') {
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), param[0]));
		throw std::exception();
	}
	if (!_server.doesChannelExist(param[0])){
		_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(client.getNickname(), param[0]));
		throw std::exception();
	}
	if (param[1].compare("+sn") == 0)
		return ;
	std::vector<string> modes = modeParser(param[1]);
	if (modes.empty()){
		_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(param[1]));
		throw std::exception();
	}

	size_t p = 2;
	for (std::vector<string>::iterator it = modes.begin(); it != modes.end(); it++){
		std::vector<string>	params;
		params.push_back(params[0]);
		if (it->compare("+i") == 0 || it->compare("-i") == 0) {
			params.push_back(*it);
			inviteOnly(params, _server, ProccessMessage, clientSocket);
		}
		else if (it->compare("+t") == 0 || it->compare("-t") == 0) {
			params.push_back(*it);
			ChannelTopicMode(params, _server, ProccessMessage, clientSocket);
		}
		else if (it->compare("+k") == 0 || it->compare("-k") == 0) {
			params.push_back(*it);
			if (p < ProccessMessage.getParams().size())
				params.push_back(ProccessMessage.getParams()[p++]);
			ChannelSecureMode (params, _server, ProccessMessage, clientSocket);
		}
		else if (it->compare("+l") == 0 || it->compare("-l") == 0) {
			params.push_back(*it);
			if (p < ProccessMessage.getParams().size())
				params.push_back(ProccessMessage.getParams()[p++]);
			ChanneLimitMode(params, _server, ProccessMessage, clientSocket);
		}
		else if (it->compare("+o") == 0 || it->compare("-o") == 0) {
			params.push_back(*it);
			if (p < ProccessMessage.getParams().size())
				params.push_back(ProccessMessage.getParams()[p++]);
			ChannelOperatorPrivilege(params, _server, ProccessMessage, clientSocket);
		}
		else {
			_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(*it));
			throw std::exception();
		}
		ChannelData &channel = _server.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);
		informMembers(channel.getClientSockets(), _server.createMsg(client, "MODE", params));
	}
}
