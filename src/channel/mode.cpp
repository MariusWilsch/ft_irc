/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 12:48:29 by ahammout         ###   ########.fr       */
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

    if (ProccessMessage.getParams().size() != 2){
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

    //! Inform other members about this action.
    // string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
    // message.append(" has changed mode: ");
    // message.append(ProccessMessage.getParams()[1]);
    // message.append("\n");
    // ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);
}

void    ChannelSecureMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

    if (ProccessMessage.getParams().size() < 1 || ProccessMessage.getParams().size() > 3){
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

    //! Inform other members about this action.

    // string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
    // message.append(" has changed mode: ");
    // message.append(ProccessMessage.getParams()[1]);
    // message.append("\n");
    // ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);

}

void    ChannelTopicMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

    if (ProccessMessage.getParams().size() < 1 || ProccessMessage.getParams().size() > 3){
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

    //! Inform other members about this action.
    // string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
    // message.append(" has changed mode: ");
    // message.append(ProccessMessage.getParams()[1]);
    // message.append("\n");
    // ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);
}

void ChanneLimitMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

    if (ProccessMessage.getParams().size() < 1 || ProccessMessage.getParams().size() > 3){
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
        // What if the client try this:  mode #ChannelName +l a !
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

    //! Inform other members about this action.
    // string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
    // message.append(" has changed mode: ");
    // message.append(ProccessMessage.getParams()[1]);
    // message.append("\n");
    // ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message);
}

// Test the size of parameter with nc punch: server.
void    ChannelOperatorPrivilege(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    ChannelData &channel = _serverReactor.getChannelManager().getChannelByName(ProccessMessage.getParams()[0]);

    if (ProccessMessage.getParams().size() != 3){
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
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channel.getName()));
        throw std::exception();
    }
    if (ProccessMessage.getParams()[1].compare("+o") == 0)
        channel.addOperator(ClSocket);
    else if (ProccessMessage.getParams()[1].compare("-o") == 0)
        channel.removeOperator(ClSocket);
    
    // !Check if there is a need to inform other members about this action or not?
}

void     ExecuteCommands::mode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    if (ProccessMessage.getParams().size() >= 2){
        if (!_serverReactor.doesChannelExist(ProccessMessage.getParams()[0])){
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]));
            throw std::exception();
        }
        string mode = ProccessMessage.getParams()[1];
        // Handle the mode of limeChat when join command is sended by the user.
        // if (mode.compare("+s") == 0)
        //     return ;
        if (mode.compare("+i") == 0 || mode.compare("-i") == 0){
            inviteOnly(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+k") == 0 || mode.compare("-k") == 0){
            ChannelSecureMode (_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+t") == 0 || mode.compare("-t") == 0){
            ChannelTopicMode(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+l") == 0 || mode.compare("-l") == 0){
            ChanneLimitMode(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+o") == 0 || mode.compare("-o") == 0){
            ChannelOperatorPrivilege(_serverReactor, ProccessMessage, clientSocket);
        }
        else{
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_UNKNOWNMODE(mode));
            throw std::exception();
        }
    }
    else{
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
        throw std::exception();
    }
}