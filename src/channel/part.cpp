/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/08 16:56:56 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

/*
    This funciton it's about part from single or multiple channels in one time, 
    + The first task here is about split the parameters and check if there is multiple channel names and
    initialize the set reference passed to the multipleChannels funciton.
*/

bool    partParser(std::vector<string> &ChannelNames, std::vector<string> &partMessage, Message &ProcessMessage){
    if (ProcessMessage.getParams().size() == 0)
        return (-1);
    if (ProcessMessage.getParams().size() < 1 || ExecuteCommands::whiteCheck(ProcessMessage.getParams()[0]))
        return (-1);
    for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++){
        string param = ProcessMessage.getParams()[i];
        param.erase(remove(param.begin(), param.end(), '\n'), param.end());
        unsigned int c = 0;
        for (unsigned int i = 0; i < param.size(); i++){
            if (param[i] == ',')
                c++;
        }
        if (c == 0){
            // SINGLE CHANNEL PARAMETER
            if (param[0] == '#'){
                param.erase(0, 1);
                ChannelNames.push_back(param);
            }
            else if (!ExecuteCommands::whiteCheck(param)){
                partMessage.push_back(param);
            }
        }
        else{
            // MULTIPLE CHANNEL PARAMETER
            for (unsigned int j = 0; j <= c; j++){
                unsigned int  e = param.find(',');
                string sub = param.substr(0, e);
                sub.erase(std::remove(sub.begin(), sub.end(), ' '), sub.end());
                param = param.substr(e + 1);
                if (sub[0] == '#'){
                    sub.erase(0, 1);
                    ChannelNames.push_back(sub);
                }
                else if (!ExecuteCommands::whiteCheck(sub)){
                    partMessage.push_back(sub);
                }
            }
        }
    }
    if (partMessage.size() != 1)
        return (-1);
    return (0);
}

void     ExecuteCommands::part(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    std::vector<string> ChannelNames;
    std::vector<string> partMessage;

    int stat = partParser(ChannelNames, partMessage, ProcessMessage);
    if (stat == -1){
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
    else{
        // After knowing that everything is good with parameters then The part process from the specified channels it's ready to be done.
        // The part command is available for all the members of the channel to use.
        for (unsigned int i = 0; i < ChannelNames.size(); i++){
            if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == true){
                // The channel it's exist.
                if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isCLient(clientSocket)){
                    _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).removeClient(clientSocket);
                    if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isOperator(clientSocket)){
                        _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).removeOperator(clientSocket);
                    }
                    // ! Do i need to inform the channels members about this action.
                    // Check if the channel empty.
                    // if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).)
                    if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets().size() == 0){
                        // remove the channel from channel manager.
                        _serverReactor.getChannelManager().removeChannel(ChannelNames[i]);
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