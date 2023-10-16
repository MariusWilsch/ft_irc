/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 01:15:31 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool    partParser(std::vector<string> &ChannelNames, std::vector<string> &partMessage, Message &ProcessMessage){
	vector<string> params = ProcessMessage.getParams();
	
	if (params.size() == 0)
			return (false);
	if (params[0].empty())
			return (false);
	
	for (unsigned int i = 0; i < params.size(); i++) {
		string param = ProcessMessage.getParams()[i];
		if (param[0] == '#')
				ChannelNames.push_back(param);
		else if (!ExecuteCommands::whiteCheck(param))
				partMessage.push_back(param);
	}
	return (true);
}

// :Archer123!~Aissam@5c8c-aff4-7127-3c3-1c20.230.197.ip PART #chTest111 : I don't like those kind of random channels
// ! if the user leaves a channel and the part message parameter is present then need to inform the other members like in the above example.
	// ? How to inform them?
	// * there is vector for partMessage is exist inside the part function, this vector  will contain one element or more, if it's size is biger than 0 then take the first element
	// * and send with the part message to inform other members.
	void     ExecuteCommands::part(ServerReactor &_server, Message &ProcessMessage, int clientSocket) {
		
	std::vector<string> ChannelNames;
	std::vector<string> partMessage;

	int stat = partParser(ChannelNames, partMessage, ProcessMessage);
	if (stat == false){
		string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
		send(clientSocket, Err.c_str(), Err.size(), 0);
		throw std::exception();
	}
	for (unsigned int i = 0; i < ChannelNames.size(); i++) {
		if (!_server.doesChannelExist(ChannelNames[i])){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(ChannelNames[i]));
			continue ;
		}
		ChannelData &channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
		if (!channel.isCLient(clientSocket)){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(ChannelNames[i]));
			continue ;
		}
		channel.removeClient(clientSocket);
		if (channel.isOperator(clientSocket))
			channel.removeOperator(clientSocket);
		if (channel.getClientSockets().size() == 0) // remove the channel from channel manager.
			_server.getChannelManager().removeChannel(ChannelNames[i]);

		// ! the problem is that, not all the command have to send their parameters in the informative message to other users.
		// ? How to send an informative message to all the members with an appropriate parameters and message.
		// * 1 - generate the list of parameter before sending it to createInfoMsg function
		// * 2 - the funtion takes three parameters the first two parameters are good, the thirt parameter need to containe the appropriate parameters.
		
		std::vector<string> params;
		params.push_back(ProcessMessage.getParams()[0]);
		if (partMessage.size())
			params.push_back(partMessage[0].insert(0, ":"));
		informMembers(channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "PART", params));
		// informMembers(channel.getClientSockets(), _server.createInfoMsg(_server.getClientDataFast(clientSocket), "PART", ProcessMessage.getParams()));
	}
}
