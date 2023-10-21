/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:47:40 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/15 19:19:35 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &ProcessMessage){
    string param = ProcessMessage.getParams()[0];

    if (param.size() == 0 || param.empty())
        return (false);
    for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++){
        param = ProcessMessage.getParams()[i];
        if (param[0] == '#'){
            ChannelNames.push_back(param);
        }
        else if (!ExecuteCommands::whiteCheck(param)){
            Users.push_back(param);
        }
    }
    if (Users.size() > ChannelNames.size() + 1)
				return (false);
		if (Users.size() == ChannelNames.size() + 1) {
				ProcessMessage.setTrailing(Users[Users.size() - 1]);
			cout << "Users Size: " << Users.size() << "Names: " << ChannelNames.size() << endl;
    }
    return (true);
}

/*
    Search for the channels inside the channels countainer:
        + channel doesn't exist: Generate a numeric reply of ERR and continue.
        + channel exist search for the componenet user
            + if the user is present then: kick his ass from the channel.
                + Check the trailing if it's present then inform all the members: by the kick reason
                + if the trailing is not present then JUST INFORM without specifyng the reason 
            + if the user doesn't exist then generate a specific numeric reply and continue.
*/

 // Kick #channel1 #channel2 #InvalidCh user1 user2 user3
 // !if kick command failed with a specific channel for any reason: Do the program continue it's execution or not????? 
void     ExecuteCommands::kick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    const string& nickSender = _serverReactor.getClientDataFast(clientSocket).getNickname();
    std::vector<string> ChannelNames;
    std::vector<string> Users;

    
    int stat = kickParser(ChannelNames, Users, ProcessMessage);
    if (!stat){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
        throw std::exception();
    }
    for (unsigned int i = 0; i < ChannelNames.size(); i++){
        if (!_serverReactor.doesChannelExist(ChannelNames[i])){
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(ChannelNames[i]));
            continue;
        }
        ChannelData &Channel = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]);
        if (!Channel.isCLient(clientSocket)){
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(ChannelNames[i]));
            continue;
        }
        if (!Channel.isOperator(clientSocket)){
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(ChannelNames[i]));
            continue;
        }
        if (Users[i].c_str() == NULL){
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
            continue;
        }
        set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets();
        int kickedID = _serverReactor.getClientManager().MatchNickName(ChannelMembers, Users[i]);
        if (kickedID == -1){
			_serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, Users[i]));
            continue;
        }
				vector<string> params;
				params.push_back(ChannelNames[i]);
				params.push_back(Users[i]);

				informMembers(Channel.getClientSockets(), _serverReactor.createMsg(_serverReactor.getClientDataFast(clientSocket), "KICK", params, ProcessMessage.getTrailing()));
        Channel.removeClient(kickedID);
        if (Channel.isOperator(kickedID))
            Channel.removeOperator(kickedID);
        // NUMERIC REPLY TO INFORM ALL THE CHANNEL MEMBER. "COMMENT"

    }
}

// #include "ExecuteCommands.hpp"

// bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &ProcessMessage)
// {
//     string param = ProcessMessage.getParams()[0];
//     if (ProcessMessage.getParams().size() < 1)
//         return (false);
//     for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++)
//     {
//         param = ProcessMessage.getParams()[i];
//         if (param[0] == '#')
//         {
//             ChannelNames.push_back(param);
//         }
//         else if (!ExecuteCommands::whiteCheck(param))
//         {
//             Users.push_back(param);
//         }
//     }
//     if (Users.size() > ChannelNames.size())
//         return (false);
//     for (unsigned int i = Users.size(); i < ChannelNames.size(); i++)
//     {
//         Users.push_back("");
//     }
//     return (true);
// }

// /*
//     Search for the channels inside the channels countainer:
//         + channel doesn't exist: Generate a numeric reply of ERR and continue.
//         + channel exist search for the componenet user
//             + if the user is present then: kick his ass from the channel.
//                 + Check the trailing if it's present then inform all the members: by the kick reason
//                 + if the trailing is not present then JUST INFORM without specifyng the reason
//             + if the user doesn't exist then generate a specific numeric reply and continue.
// */

// // Kick #channel1 #channel2 #InvalidCh a user1 user2 user3
// void ExecuteCommands::kick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
// {
//     std::vector<string> ChannelNames;
//     std::vector<string> Users;

//     int stat = kickParser(ChannelNames, Users, ProcessMessage);
//     if (!stat)
//     {
//         string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
//         send(clientSocket, Err.c_str(), Err.size(), 0);
//         throw std::exception();
//     }
//     else
//     {
//         for (unsigned int i = 0; i < ChannelNames.size(); i++)
//         {
//             if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == true)
//             {
//                 if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isCLient(clientSocket))
//                 {
//                     if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isOperator(clientSocket))
//                     {
//                         if (Users[i].c_str() != NULL)
//                         {
//                             set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets();
//                             int kickedID = _serverReactor.getClientManager().MatchNickName(ChannelMembers, Users[i]);
//                             if (kickedID != -1)
//                                 _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).removeClient(kickedID);
//                             else
//                             {
//                                 string Err = ERR_USERNOTINCHANNEL(Users[i], ChannelNames[i]);
//                                 send(clientSocket, Err.c_str(), Err.size(), 0);
//                             }
//                             // NUMERIC REPLY TO INFORM ALL THE CHANNEL MEMBER. "COMMENT"
//                         }
//                         else
//                         {
//                             string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
//                             send(clientSocket, Err.c_str(), Err.size(), 0);
//                         }
//                     }
//                     else
//                     {
//                         string Err = ERR_CHANOPRIVSNEEDED(ChannelNames[i]);
//                         send(clientSocket, Err.c_str(), Err.size(), 0);
//                         throw std::exception();
//                     }
//                 }
//                 else
//                 {
//                     string Err = ERR_NOTONCHANNEL(ChannelNames[i]);
//                     send(clientSocket, Err.c_str(), Err.size(), 0);
//                     throw std::exception();
//                 }
//             }
//             else
//             {
//                 string Err = ERR_NOSUCHCHANNEL(ChannelNames[i]);
//                 send(clientSocket, Err.c_str(), Err.size(), 0);
//             }
//         }
//     }
// }