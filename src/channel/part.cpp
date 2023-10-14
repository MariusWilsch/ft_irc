/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 14:18:46 by mwilsch          ###   ########.fr       */
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
		for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++) {
				string param = ProcessMessage.getParams()[i];
				// SINGLE CHANNEL PARAMETER
				if (param[0] == '#')
						ChannelNames.push_back(param);
				else if (!ExecuteCommands::whiteCheck(param))
						partMessage.push_back(param);
		}
		if (partMessage.size() != 1)
				return (-1);
		return (0);
}

void     ExecuteCommands::part(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
		if (_serverReactor.getClientManager().getClientData(clientSocket).getRegistration()){
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
								if (_serverReactor.doesChannelExist(ChannelNames[i])){
										// The channel it's exist.
										ChannelData &channelData = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]);
										if (channelData.isCLient(clientSocket)) {
												channelData.removeClient(clientSocket);
												
												if (channelData.isOperator(clientSocket))
														channelData.removeOperator(clientSocket);
														
												_serverReactor.sendMsg(clientSocket, _serverReactor.getClientDataFast(clientSocket).getClientInfo(), "PART", ChannelNames[i]);
					
												// ! Do i need to inform the channels members about this action.
												// Check if the channel empty.
												if (channelData.getClientSockets().size() == 0) // remove the channel from channel manager.
														_serverReactor.getChannelManager().removeChannel(ChannelNames[i]);
										else {
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
	}
}