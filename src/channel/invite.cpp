/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 06:59:30 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


// New design to remove if / else  if /else forest >.. 
    // *

	// :Archer159263!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip INVITE Archer159 :#ch1
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 06:34:14 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"


// New design to remove if / else  if /else forest >.. 
    // *

	// :Archer159263!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip INVITE Archer159 :#ch1
void     ExecuteCommands::invite(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){

    if (ProccessMessage.getParams().size() < 2){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProccessMessage.getCommand()));
        return ;
    }
    string channelName = ProccessMessage.getParams()[1];
		vector<string> params = ProccessMessage.getParams();
    if (!_serverReactor.doesChannelExist(channelName)) {
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(channelName));
        return ;
    }
    if (!_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channelName));
        return ;
    }
    if (!_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
        return ;
    }
		
		int targetSocket = _serverReactor.getClientManager().getClientSocketByNick(params[0]);
		if (targetSocket == -1)
		{ // But if the target Socket is not found we should throw ERR_NOSUCHNICK
				_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_USERONCHANNEL(params[0], channelName));
        return ;
		}
		_serverReactor.getChannelManager().getChannelByName(channelName).addGuest(params[0]);
		cout << "inviting user" << endl;
		_serverReactor.sendMsg_FixLater(targetSocket, 
			_serverReactor.createInfoMsg(_serverReactor.getClientDataFast(targetSocket), ProccessMessage.getCommand(), params));
}
