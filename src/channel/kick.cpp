/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:47:40 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 14:49:23 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &ProcessMessage){
    string param = ProcessMessage.getParams()[0];
    if (ProcessMessage.getParams().size() < 1)
        return (false);
    for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++){
        param = ProcessMessage.getParams()[i];
        if (param[0] == '#'){
            ChannelNames.push_back(param);
        }
        else if (!ExecuteCommands::whiteCheck(param)){
            Users.push_back(param);
        }
    }
    if (Users.size() > ChannelNames.size())
        return (false);
    for (unsigned int i = Users.size(); i < ChannelNames.size(); i++){
        Users.push_back("");
    }
    return (true);
}

/*
    Search for the channels inside the channels countainer:
        + channel doesn't exist: Generate a numeric reply of ERR and continue.
        + channel exist search for the componenet user
            + if the user is present then: kick his ass from the channel.
                + Check the trailing if it's present then inform all the members: by the kick reason
                + if the trailing is not present then JUST INFORM without specifyng the reason 
            + if the user doesn't exist then generate a specific numeric reply and continue.
*/

 // Kick #channel1 #channel2 #InvalidCh a user1 user2 user3
void     ExecuteCommands::kick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    std::vector<string> ChannelNames;
    std::vector<string> Users;
    
    int stat = kickParser(ChannelNames, Users, ProcessMessage);
    if (!stat){
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
            send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
    else{
        for (unsigned int i = 0; i < ChannelNames.size(); i++){
            if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == true){
                if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isCLient(clientSocket)){
                    if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isOperator(clientSocket)){
                        if (Users[i].c_str() != NULL){
                            set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets();
                            int kickedID = _serverReactor.getClientManager().MatchNickName(ChannelMembers, Users[i]);
                            if (kickedID != -1)
                                _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).removeClient(kickedID);
                            else{
                                string Err = ERR_USERNOTINCHANNEL(Users[i], ChannelNames[i]);
                                send(clientSocket, Err.c_str(), Err.size(), 0);
                            }
                            // NUMERIC REPLY TO INFORM ALL THE CHANNEL MEMBER. "COMMENT"
                            
                        }
                        else{
                            string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
                            send(clientSocket, Err.c_str(), Err.size(), 0);
                        }
                    }
                    else{
                        string Err = ERR_CHANOPRIVSNEEDED(ChannelNames[i]);
                        send(clientSocket, Err.c_str(), Err.size(), 0);
                        throw std::exception();
                    }
                }
                else{
                    string Err = ERR_NOTONCHANNEL(ChannelNames[i]);
                    send(clientSocket, Err.c_str(), Err.size(), 0);
                    throw std::exception();
                }
            }
            else{
                string Err = ERR_NOSUCHCHANNEL(ChannelNames[i]);
                send(clientSocket, Err.c_str(), Err.size(), 0);
            }
        }
    }
}