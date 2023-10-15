/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:13:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 12:32:02 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 
#include "ExecuteCommands.hpp"


/*
    :WldL3arbi!~Aissam@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#HDI
    :punch.wa.us.dal.net 353 WldL3arbi = #HDI :@WldL3arbi
    :punch.wa.us.dal.net 366 WldL3arbi #HDI :End of /NAMES list.

*/

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


void    leaveChannels(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    map<string, ChannelData>::iterator  it;
    map<string, ChannelData> &m = _serverReactor.getChannelManager().getChannels();
    for (it = m.begin(); it != m.end();)
    {
        set<int>::iterator RemoveIt;
        RemoveIt = it->second.getClientSockets().find(clientSocket);
        if (RemoveIt != it->second.getClientSockets().end())
            it->second.removeClient(*RemoveIt);
        RemoveIt = it->second.getOperators().find(clientSocket);
        if (RemoveIt != it->second.getOperators().end())
            it->second.removeOperator(*RemoveIt);
        if (it->second.getOperators().size() == 0 && it->second.getClientSockets().size() == 0)
            _serverReactor.getChannelManager().getChannels().erase(it++);
        else
            ++it;
    }
}

// !Check if the new constructor is working good !
bool	createNewChannel(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket, string ChannelName){
    ChannelData    NewChannel(ChannelName, clientSocket);

    _serverReactor.getChannelManager().addChannel(ChannelName, NewChannel);
    string nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
    string channelKey = "=";
    _serverReactor.sendMsg(clientSocket, _serverReactor.getClientManager().getClientData(clientSocket).getClientInfo(), "JOIN", NewChannel.getName());
    _serverReactor.sendNumericReply_FixLater(clientSocket, RPL_NAMREPLY(nickname , channelKey, NewChannel.getName(), _serverReactor.getChannelManager().createUserList(NewChannel.getName(), _serverReactor, clientSocket)));
    _serverReactor.sendNumericReply_FixLater(clientSocket, RPL_ENDOFNAMES(nickname, NewChannel.getName()));

    return (true);
}

bool	joinPrivateChannel(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket, ChannelData& Channel, string inputKey){
    if ((inputKey.c_str() != NULL) && (Channel.getKey().compare(inputKey) == 0)){
        Channel.addClient(clientSocket);
        return(true);
    }
    _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_BADCHANNELKEY(Channel.getName()));
    return (false);
}

bool	joinPublicChannel(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket, ChannelData& Channel, string inputKey){
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
    // Handle missing parameters or malformed command
    if (stat == -1) {
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
        throw std::exception();
    }
    if (stat == 0) {
        leaveChannels(_server, ProcessMessage, clientSocket);
        throw std::exception();
    }
    for (unsigned int i = 0; i < ChannelNames.size(); i++) {
        bool Joined = false;  // Moved the declaration here to avoid re-declaration
        if (!_server.getChannelManager().channelExistence(ChannelNames[i])) {
            Joined = createNewChannel(_server, ProcessMessage, clientSocket, ChannelNames[i]);
            continue;  // Ensure we move to the next channel
        }
        ChannelData&	Channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
        if (Channel.isCLient(clientSocket))
                continue;
        // Various checks and error handling
        if (Channel.getLimitFlag() && Channel.getClientSockets().size() >= Channel.getLimit()) {
            _server.sendNumericReply_FixLater(clientSocket, ERR_CHANNELISFULL(Channel.getName()));
            continue;
        }
        if (Channel.getInviteFlag() && !Channel.isInvited(_server.getClientDataFast(clientSocket).getNickname())) {
            _server.sendNumericReply_FixLater(clientSocket, ERR_INVITEONLYCHAN(Channel.getName()));
            continue;
        }
        // Join either a private or public channel based on its security setting
        Joined = (Channel.getSecurity()) ? 
                joinPrivateChannel(_server, ProcessMessage, clientSocket, Channel, ChannelKeys[i]) : 
                joinPublicChannel(_server, ProcessMessage, clientSocket, Channel, ChannelKeys[i]);
    
        // If the client joined the channel, send the topic and inform members
        if (Joined) {
            if (Channel.getTopicFlag())
                _server.sendNumericReply_FixLater(clientSocket, RPL_TOPIC(Channel.getName(), Channel.getTopic()));
            else
                _server.sendNumericReply_FixLater(clientSocket, RPL_NOTOPIC(Channel.getName()));
            
            informMembers(Channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "JOIN", ProcessMessage.getParams()));
            string nickname = _server.getClientManager().getClientData(clientSocket).getNickname();
            string channelKey = "=";
            // _server.sendMsg(clientSocket, _server.getClientManager().getClientData(clientSocket).getClientInfo(), "JOIN", ChannelName);
            _server.sendNumericReply_FixLater(clientSocket, RPL_NAMREPLY(nickname , channelKey, Channel.getName(), _server.getChannelManager().createUserList(Channel.getName(), _server, clientSocket)));
            _server.sendNumericReply_FixLater(clientSocket, RPL_ENDOFNAMES(nickname, Channel.getName()));
        }
    }
    // Print user information
   _server.printUserInformation(clientSocket);
}

// void ExecuteCommands::join(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
// 				std::vector<string> ChannelNames;
// 				std::vector<string> ChannelKeys;

// 				int stat = joinParser(ChannelNames, ChannelKeys, ProcessMessage);
// 				if (stat == -1)
// 						return (_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));)
						
// 				if (stat == 0)
// 						leaveChannels(_serverReactor, ProcessMessage, clientSocket);
		
// 				for (unsigned int i = 0; i < ChannelNames.size(); i++){
// 						bool Joined = false;
// 						if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == false){
// 								Joined = createNewChannel(_serverReactor, ProcessMessage, clientSocket, ChannelNames[i]);
// 						} else {
// 								ChannelData& Channel = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]);
// 								if (!Channel.isCLient(clientSocket)) {
									
// 									if (Channel.getLimitFlag() && Channel.getClientSockets().size() >= Channel.getLimit())
// 										return (_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANNELISFULL(Channel.getName())));

// 										if (Channel.getInviteFlag()){
// 												if (!Channel.isInvited(_serverReactor.getClientManager().getClientData(clientSocket).getNickname())){
// 														string Err = ERR_INVITEONLYCHAN(Channel.getName());
// 														send(clientSocket, Err.c_str(), Err.size(), 0);
// 														throw std::exception();
// 												}
// 										}
// 										if (Channel.getSecurity() == true)
// 												Joined = joinPrivateChannel(_serverReactor, ProcessMessage, clientSocket, Channel, ChannelKeys[i]);
// 										else
// 												Joined = joinPublicChannel(_serverReactor, ProcessMessage, clientSocket, Channel, ChannelKeys[i]);
// 										if (Joined == true){
// 												if (Channel.getTopicFlag()){
// 														string Rpl =  RPL_TOPIC(Channel.getName() ,Channel.getTopic());
// 														send(clientSocket, Rpl.c_str(), Rpl.size(), 0);
// 												}
// 												else{
// 														string Rpl =  RPL_NOTOPIC(Channel.getName());
// 														send(clientSocket, Rpl.c_str(), Rpl.size(), 0);
// 												}
// 												// FIXME: use createInfoMessage and processItems to create the message
// 												informMembers(Channel.getClientSockets(), _serverReactor.createInfoMsg(clientSocket, "JOIN", ProcessMessage.getParams()), clientSocket);
// 										}
// 								}
// 						}
				
// 				// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DISPLAY CHANNELS INFORMATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// 				map<string, ChannelData>::iterator  it;
// 				std::cout << "Number of channels: " << _serverReactor.getChannelManager().getChannels().size() << std::endl;
// 				map<string, ChannelData> m = _serverReactor.getChannelManager().getChannels();
// 				for (it = m.begin(); it != m.end(); it++)
// 				{
// 						std::cout << "Channel name: " << it->second.getName() << std::endl;
// 						if (it->second.getSecurity())
// 								cout << "~~~> Channel is private " << endl;
// 						else
// 								cout << "~~~> Channel is public " << endl;
// 						set<int>::iterator cl;
// 						set<int> s = it->second.getClientSockets();
// 						for (cl = s.begin(); cl != s.end(); cl++){
// 								std::cout << "  >>> Client: " << *cl << endl;
// 						} 
						
// 						set<int>::iterator op;
// 						set<int> f = it->second.getOperators();
// 						for (op = f.begin(); op != f.end(); op++){
// 								std::cout << "  >>> operator: " << *op << endl;
// 						}
// 				}
// 		}
// }


