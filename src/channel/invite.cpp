/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:58:39 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 11:55:41 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

// New design to remove if / else  if /else forest >.. 

	// :Archer159263!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip INVITE Archer159 :#ch1

// FIXME: Use the ServerException class to throw exceptions

// void ExecuteCommands::invite(ServerReactor &_server, Message &Message, int clientSocket) {
//     try {
//         if (Message.getParams().size() < 2) {
//             throw ServerException(ERR_NEEDMOREPARAMS(Message.getCommand()));
//         }

//         vector<string> params = Message.getParams();
//         string channelName = params[1];
//         ChannelData& channel = _server.getChannelManager().getChannelByName(channelName);

//         if (!_server.doesChannelExist(params[1])) {
//             throw ServerException(ERR_NOSUCHCHANNEL(channelName));
//         }

//         if (!channel.isCLient(clientSocket)) {
//             throw ServerException(ERR_NOTONCHANNEL(channelName));
//         }

//         if (!channel.isOperator(clientSocket)) {
//             throw ServerException(ERR_CHANOPRIVSNEEDED(channelName));
//         }

//         int targetFD = _server.getClientManager().getClientSocketByNick(params[0]);
//         if (targetFD == -1) {
//             throw ServerException(ERR_USERONCHANNEL(params[0], channelName));
//         }

//         channel.addGuest(params[0]);
//         cout << "inviting user" << endl;
//         _server.sendMsg_FixLater(targetFD, _server.createInfoMsg(_server.getClientDataFast(targetFD), Message.getCommand(), params));

//     } catch (const ServerException &e) {
//         _server.sendNumericReply_FixLater(clientSocket, e.what());
//     }
// }


// void     ExecuteCommands::invite(ServerReactor &_server, Message &Message, int clientSocket){

//     if (Message.getParams().size() < 2){
//         _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(Message.getCommand()));
//         return ;
//     }
		
// 		vector<string>	params = Message.getParams();
//     string					channelName = params[1];
// 		ChannelData&		channel = _server.getChannelManager().getChannelByName(channelName);
		
//     if (!_server.doesChannelExist(params[1])) {
//         _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(channelName));
//         return ;
//     }
		
//     if (!channel.isCLient(clientSocket)){
//         _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(channelName));
//         return ;
//     }
		
//     if (!channel.isOperator(clientSocket)){
//         _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(channelName));
//         return ;
//     }
		
// 		int targetFD = _server.getClientManager().getClientSocketByNick(params[0]);
// 		if (targetFD == -1)
// 		{ // But if the target Socket is not found we should throw ERR_NOSUCHNICK
// 				_server.sendNumericReply_FixLater(clientSocket, ERR_USERONCHANNEL(params[0], channelName));
//         return ;
// 		}
// 		channel.addGuest(params[0]);
// 		cout << "inviting user" << endl;
// 		_server.sendMsg_FixLater(targetFD, _server.createInfoMsg(_server.getClientDataFast(targetFD), Message.getCommand(), params));
// }
