/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 16:41:15 by mwilsch          ###   ########.fr       */
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

void     ExecuteCommands::part(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {
		
	std::vector<string> ChannelNames;
	std::vector<string> partMessage;

	int stat = partParser(ChannelNames, partMessage, ProcessMessage);
	if (stat == -1){
			string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
			send(clientSocket, Err.c_str(), Err.size(), 0);
			throw std::exception();
	}
	// After knowing that everything is good with parameters then The part process from the specified channels it's ready to be done.
	// The part command is available for all the members of the channel to use.
	for (unsigned int i = 0; i < ChannelNames.size(); i++) {
		// If the channel doesn't exist then send ERR_NOSUCHCHANNEL
		if (!_server.doesChannelExist(ChannelNames[i])){
				_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(ChannelNames[i]));
				continue ;
		}
		ChannelData &channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
		// If the client isn't a member of the channel then send ERR_NOTONCHANNEL
		if (!channel.isCLient(clientSocket)){
				_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(ChannelNames[i]));
				continue ;
		}
		// Send the part message to all the members of the channel.
		informMembers(channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "PART", ProcessMessage.getParams()));
		// Remove the client from the channel.
		channel.removeClient(clientSocket);
		if (channel.isOperator(clientSocket))
				channel.removeOperator(clientSocket);
		// Check if the channel empty.
		if (channel.getClientSockets().size() == 0) // remove the channel from channel manager.
			_server.getChannelManager().removeChannel(ChannelNames[i]);
		}
}
