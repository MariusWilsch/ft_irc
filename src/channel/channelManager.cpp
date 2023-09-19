/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:45:26 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/19 18:29:37 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ExecuteCommands.hpp"

bool    whiteCheck(string str)
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
            return false;
    }
    return true;
}

void ExecuteCommands::join(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    std::cout << "Start executing the join command " << std::endl;
    // ! Split up the paramters, so they can be checked individually
    // * Declare ftwo sets to store the channel name and the key , if the channel deosn't have key then the the componenet index will be NULL
    
    set<string> ChannelNames;
    set<string> ChannelKeys;

    set<string>::iterator it1 = ChannelNames.begin();
    set<string>::iterator it2 = ChannelKeys.begin();

    
    // Insert the channel names and keys in the sets
    for (unsigned int i = 0; ProcessMessage.getParams().size(); i++){
        if (whiteCheck(ProcessMessage.getParams()[0])){
            continue;
        }
        else if (ProcessMessage.getParams()[i][0] == '#' || ProcessMessage.getParams()[i][0] == '&'){
            ChannelNames.insert(ProcessMessage.getParams()[i] + 1);
            it1++;
        }
        else{
            ChannelKeys.insert(ProcessMessage.getParams()[i]);
            it2++;
        }
    }
    
    if (ChannelKeys.size() > ChannelNames.size()){
        string buffer = "error(461): ";
        buffer.append(" Not enough parameters");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }

    // DISPLAY THINGS THEN W'll see
    std::cout << "The channels names" << std::endl;
    for (unsigned int i = 0; i < ChannelNames.size(); i++){
        std::cout << "Channel name: " << *it1 << std::endl;
        it1++;
    }
    std::cout << "The channels keys" << std::endl;
    for (unsigned int i = 0; i < ChannelKeys.size(); i++){
        std::cout << "Channel key: " << *it2 << std::endl;
        it2++;
    }
}
