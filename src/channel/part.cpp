/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/05 18:40:29 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

/*
    This funciton it's about part from single or multiple client in one time, 
    + The first task here is about split the parameters and check if there is multiple channel names and
    initialize the set reference passed to the multipleChannels funciton.
*/



/*
    Command: PART
    Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
*/

bool    multipleChnnels(std::vector<string> &ChannelNames, std::vector<string> &partMessage, Message &ProcessMessage){
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
                ChannelNames.push_back([param]);
            }
            else if (!ExecuteCommands::whiteCheck(param)){
                partMessage.push_back(param);
            }
        }
        else{
            // MULTIPLE CHANNEL PARAMETER
            
        }
    }
}

void     ExecuteCommands::part(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    std::vector<string> ChannelNames;
    std::vector<string> partMessage;

    int stat = multipleChnnels(ChannelNames, partMessage, ProcessMessage);
    if (stat == -1){
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
    else{
        // After knowing that everything is good with parameters then The part process from the specified channels it's ready to be done.
        
    }
}