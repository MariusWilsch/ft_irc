/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/12 20:46:50 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


// New design to remove if / else  if /else forest >.. 
    // *

void     ExecuteCommands::invite(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    if (_serverReactor.getClientManager().getClientData(clientSocket).getRegistration()){       
        if (ProccessMessage.getParams().size() == 2){
            
            string channelName = ProccessMessage.getParams()[1];
            if (_serverReactor.getChannelManager().itsChannel(channelName)){
                
                channelName.erase(0, 1);
                if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
                    
                    if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
                        
                        set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets();
                        
                        if (_serverReactor.getClientManager().MatchNickName(ChannelMembers, ProccessMessage.getParams()[0]) == -1){
                            _serverReactor.getChannelManager().getChannelByName(channelName).addGuest(ProccessMessage.getParams()[0]);
                            // Inform The members of the channel about this event.
                            
                        }
                        else{
                            string Err = ERR_USERONCHANNEL(ProccessMessage.getParams()[0], channelName);
                            send(clientSocket, Err.c_str(), Err.size(), 0);
                            throw std::exception();
                        }
                    }
                    else{
                        string Err = ERR_CHANOPRIVSNEEDED(channelName);
                        send(clientSocket, Err.c_str(), Err.size(), 0);
                        throw std::exception();
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
            string Err = ERR_NEEDMOREPARAMS(ProccessMessage.getCommand());
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
    }
}
