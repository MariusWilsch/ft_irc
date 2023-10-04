/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:28 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/04 19:33:37 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


/*
    Command     :   Topic
    Parameters  :   #<channel>
    Trailing    :   [ <topic> ] 
*/
// * Example:       TOPIC #PrivateChannel :Ethics.

void     ExecuteCommands::topic(ServerReactor &_ServerReactor, Message &ProcessMessage, int clientSocket){
    cout << "Execute topic command" << endl;
    cout << "The trailing size: " << ProcessMessage.getTrailing().size() << endl;
    if (ProcessMessage.getParams().size() == 1){
        string channelName = ProcessMessage.getParams()[0];
        // channelName.erase(remove(channelName.begin(), channelName.end(), '\n'), channelName.end());
        if (_ServerReactor.getChannelManager().itsChannel(channelName)){
            channelName.erase(0, 1);
            if (_ServerReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
                if (ProcessMessage.getTrailing().size() == 0){
                    string Message = "Topic: ";
                    Message.append(_ServerReactor.getChannelManager().getChannelByName(channelName).getTopic());
                    Message.append("\n");
                    send(clientSocket, Message.c_str(), Message.size(), 0);
                    throw std::exception();
                }
                else{
                    string topic = ProcessMessage.getTrailing();
                    topic.erase(remove(topic.begin(), topic.end(), '\n'), topic.end());
                    if (ExecuteCommands::whiteCheck(topic))
                        _ServerReactor.getChannelManager().getChannelByName(channelName).setTopic("NoTopic");
                    else{
                        _ServerReactor.getChannelManager().getChannelByName(channelName).setTopic(topic);
                    }
                    string message = _ServerReactor.getClientManager().getClientData(clientSocket).getUsername();
                    message.append(" has set topic to: ");
                    message.append(topic);
                    message.append("\n");
                    ExecuteCommands::informMembers(_ServerReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message, clientSocket);
                }
            }
            else{
                string Err = ERR_NOTONCHANNEL(channelName);
                send(clientSocket, Err.c_str(), Err.size(), 0);
                throw std::exception();
            }
        }
        else{
            string Err = ERR_NOSUCHCHANNEL(channelName);
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
    }
    else{
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
}