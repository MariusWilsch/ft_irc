/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:47:40 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/28 18:00:22 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &msg){
    if (msg.getParams().size() < 2)
        return (false);
    string param = msg.getParams()[0];
    for (unsigned int i = 0; i < msg.getParams().size(); i++){
        param = msg.getParams()[i];
        if (param.size() > 1 && param[0] == '#'){
            ChannelNames.push_back(param);
        }
        else if (!ExecuteCommands::whiteCheck(param)){
            Users.push_back(param);
        }
    }
    if (Users.empty() || Users.size() > ChannelNames.size() + 1)
        return (false);
    if (Users.size() == ChannelNames.size() + 1)
        msg.setTrailing(Users[Users.size() - 1]);
    for (size_t i = Users.size(); i < ChannelNames.size(); i++){
        Users[i] = "";
    }
    
    return (true);
}

void     ExecuteCommands::kick(ServerReactor &_server, Message &msg, int clientSocket){
    ClientData  &client = _server.getClientDataFast(clientSocket);
    std::vector<string> ChannelNames;
    std::vector<string> Users;

    int stat = kickParser(ChannelNames, Users, msg);
    if (!stat){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCommand()));
        throw std::exception();
    }
    const string &nick = client.getNickname();
    for (unsigned int i = 0; i < ChannelNames.size(); i++){
        if (!_server.doesChannelExist(ChannelNames[i])){
            _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, ChannelNames[i]));
            continue;
        }
        ChannelData &Channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
        cout << "Is client out:> " << Channel.isCLient(clientSocket) << endl;
        if (!Channel.isCLient(clientSocket)){
            _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nick, ChannelNames[i]));
            continue;
        }
        if (!Channel.isOperator(clientSocket)){
            _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(ChannelNames[i]));
            continue;
        }
        // cout << "is op result: " << Channel.isOperator(clientSocket) << endl;
        // exit(0);
        // set<int> ChannelMembers = Channel.getClientSockets();
        int kickedID = _server.getClientManager().MatchNickName(Channel.getClientSockets(), Users[i]);
        if (kickedID == -1){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(client.getNickname(), Users[i]));
            continue;
        }
        vector<string> params;
        params.push_back(ChannelNames[i]);
        params.push_back(Users[i]);
        informMembers(Channel.getClientSockets(), _server.createMsg(client, "KICK", params, msg.getTrailing()));
        Channel.removeClient(kickedID);
        if (Channel.isOperator(kickedID))
            Channel.removeOperator(kickedID);
    }
}
