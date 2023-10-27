
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

	if (params[1].compare("+t") == 0){
		// restrict regular members from changing the topic of the channel. 
		channel.setTopicRestriction(true);
	}

	else if (params[1].compare("-t") == 0){
		// Allow all the channel member to change the topic.
		channel.setTopicRestriction(false);
	}
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
	cout << "Displaying modes vector: " << endl;
	for (std::vector<string>::iterator it = modes.begin(); it != modes.end(); it++){
		cout << *it << "   " << endl;
	}
	return (modes);
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

	if (ProccessMessage.getParams()[1].compare("+sn") == 0)
		return ;
	std::vector<string> modes = modeParser(ProccessMessage.getParams()[1]);
	// Erasing the second element from params vector.
	std::vector<string>::iterator remove = std::find(ProccessMessage.getParams().begin(), ProccessMessage.getParams().end(), ProccessMessage.getParams()[1]);
	ProccessMessage.getParams().erase(remove);
	// if the parser failed to parse the parameter will throw ERR_UNKNOWNMODE & exception.
	if (modes.empty()){
		_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(ProccessMessage.getParams()[1]));
		throw std::exception();
	}

	cout << "Display paraemters vector: " << endl;
	std::vector<string>::iterator it;
	for (it = ProccessMessage.getParams().begin(); it != ProccessMessage.getParams().end(); it++){
			cout << *it << "   " << endl;
	}
	// ! handle this: Note that there is a maximum limit of three (3) changes per command for modes that take a parameter.

	if (modes.size() > 3)
		throw std::exception();
	size_t p = 2;
	for (std::vector<string>::iterator it = modes.begin(); it != modes.end(); it++){
		std::vector<string>	params;
		params.push_back(ProccessMessage.getParams()[0]);

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
			// Unknown mode passed between modes.
			_server.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(*it));
			throw std::exception();
		}
		// Informing message> 
		ChannelData &channel = _server.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);
		informMembers(channel.getClientSockets(), _server.createMsg(client, "MODE", params));
	}
}
