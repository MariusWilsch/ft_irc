/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/04 21:52:58 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool    isDecimal(string str){
    for (unsigned int i = 0; i < str.size(); i++){
        if (!(str[i] >= '0' && str[i] <= '9'))
            return (false);
    }
    return (true);
}
//! Is all the members of the channel have the permission to change the channel modes.

// ~~~~~~~~~~~~~ ONLY THE OPERATORS ARE ALLOWED TO PERFORM SUCH OPERATION ~~~~~~~~~~/ 
void    inviteOnly(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    //* Example: mode #PrivateChannel [+/-] i
    if (ProccessMessage.getParams().size() == 2){
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            string channelName = ProccessMessage.getParams()[0];
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
                if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket))
                {
                    if (ProccessMessage.getParams()[1].compare("+i") == 0)
                        _serverReactor.getChannelManager().getChannelByName(channelName).setInviteFlag(true);
                    else if (ProccessMessage.getParams()[1].compare("-i") == 0)
                        _serverReactor.getChannelManager().getChannelByName(channelName).setInviteFlag(false);
                    // INFORM THE USER THAT THE CHANNEL MODE WAS CHANGED
                    string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                    message.append(" has changed mode: ");
                    message.append(ProccessMessage.getParams()[1]);
                    message.append("\n");
                    ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message, clientSocket);
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
        else {
            string Err = ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]);
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

//* Example: mode #PrivateChannel [+/-] k
void    ChannelSecureMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    if (ProccessMessage.getParams().size() == 3 || ProccessMessage.getParams().size() == 2){
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            string channelName = ProccessMessage.getParams()[0];
            string  key;
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){    
                if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
                    if (ProccessMessage.getParams().size() == 3){
                        key = ProccessMessage.getParams()[2];
                        key.erase(remove(key.begin(), key.end(), '\n'), key.end());
                    }
                    if (ProccessMessage.getParams()[1].compare("+k") == 0 && !key.empty()){
                        _serverReactor.getChannelManager().getChannelByName(channelName).setSecurity(true);
                        _serverReactor.getChannelManager().getChannelByName(channelName).setKey(key);
                    }
                    else if (ProccessMessage.getParams()[1].compare("-k") == 0 && ProccessMessage.getParams().size() == 2){
                        _serverReactor.getChannelManager().getChannelByName(channelName).setSecurity(false);
                        _serverReactor.getChannelManager().getChannelByName(channelName).setKey("");
                    }
                    string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                    message.append(" has changed mode: ");
                    message.append(ProccessMessage.getParams()[1]);
                    message.append("\n");
                    ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message, clientSocket);
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
        else {
            string Err = ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]);
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

//* Example: mode #PrivateChannel [+/-] t
void    ChannelTopicMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    if (ProccessMessage.getParams().size() == 3 || ProccessMessage.getParams().size() == 2){
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            string channelName = ProccessMessage.getParams()[0];
            string topic;
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket))
            {
                if (ProccessMessage.getParams().size() == 3){
                    topic = ProccessMessage.getParams()[2];
                    topic.erase(remove(topic.begin(), topic.end(), '\n'), topic.end());
                }
                if (ProccessMessage.getParams()[1].compare("+t") == 0 && !topic.empty()){
                    _serverReactor.getChannelManager().getChannelByName(channelName).setTopicFlag(true);
                    _serverReactor.getChannelManager().getChannelByName(channelName).setTopic(topic);
                }
                else if (ProccessMessage.getParams()[1].compare("-t") == 0){
                    _serverReactor.getChannelManager().getChannelByName(channelName).setTopicFlag(false);
                    _serverReactor.getChannelManager().getChannelByName(channelName).setTopic("");
                }
                string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                message.append(" has changed mode: ");
                message.append(ProccessMessage.getParams()[1]);
                message.append("\n");
                ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message, clientSocket);
            }
            else{
                string Err = ERR_NOTONCHANNEL(channelName);
                send(clientSocket, Err.c_str(), Err.size(), 0);
                throw std::exception();
            }
        }
        else{
            string Err = ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]);
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


// THis mode is about give the channel a limit of members that can join in the sametime:
// pov: if this mode is not setted then the channel will not have any limit inside it
//* Example: mode #ChannelName [+/-] l
void ChanneLimitMode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    cout << "The channel limit" << endl;
    if (ProccessMessage.getParams().size() == 3 || ProccessMessage.getParams().size() == 2){
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            int     limit;
            string  channelName = ProccessMessage.getParams()[0];
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
                if (ProccessMessage.getParams().size() == 3){
                    if (isDecimal(ProccessMessage.getParams()[2]))
                        limit = std::atoi(ProccessMessage.getParams()[1].c_str());
                    else{
                        string Err = ERR_NEEDMOREPARAMS(ProccessMessage.getCommand());
                        send(clientSocket, Err.c_str(), Err.size(), 0);
                        throw std::exception();
                    }
                }
                if (ProccessMessage.getParams()[1].compare("+l") == 0 && limit){
                    _serverReactor.getChannelManager().getChannelByName(channelName).setLimitFlag(true);
                    _serverReactor.getChannelManager().getChannelByName(channelName).setLimit(limit);
                }
                else if (ProccessMessage.getParams()[1].compare("-l") == 0){
                    _serverReactor.getChannelManager().getChannelByName(channelName).setLimitFlag(false);
                    _serverReactor.getChannelManager().getChannelByName(channelName).setLimit(-1);
                }
                string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                message.append(" has changed mode: ");
                message.append(ProccessMessage.getParams()[1]);
                message.append("\n");
                ExecuteCommands::informMembers(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), message, clientSocket);
            }
            else{
                string Err = ERR_NOTONCHANNEL(channelName);
                send(clientSocket, Err.c_str(), Err.size(), 0);
                throw std::exception();
            }
   
        }
        else{
            string Err = ERR_NOSUCHCHANNEL(ProccessMessage.getParams()[0]);
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

// * Command: mode #ChannelName [+/i] o <user>
    // Number of parameters = 3;
void    ChannelOperatorPrivilege(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    cout << "Give the Privilege" << endl;
    if (ProccessMessage.getParams().size() == 3){
        string channelName = ProccessMessage.getParams()[0];
        channelName.erase(0, 1);
        if (_serverReactor.getChannelManager().getChannelByName(channelName).isCLient(clientSocket)){
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
                string nickName = ProccessMessage.getParams()[2];
                int ClSocket = _serverReactor.getClientManager().MatchNickName(_serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets(), nickName);
                if (ProccessMessage.getParams()[1].compare("+o") == 0)
                    _serverReactor.getChannelManager().getChannelByName(channelName).addOperator(ClSocket);
                else if (ProccessMessage.getParams()[1].compare("-o") == 0)
                    _serverReactor.getChannelManager().getChannelByName(channelName).removeOperator(ClSocket);
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
        string Err = ERR_NEEDMOREPARAMS(ProccessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
}

//* Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
void     ExecuteCommands::mode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    if (ProccessMessage.getParams().size() >= 2){
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS ONLY THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
        string mode = ProccessMessage.getParams()[1];
        if (mode.compare("+i") == 0 || mode.compare("-i") == 0){
            inviteOnly(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+k") == 0 || mode.compare("-k") == 0){
            ChannelSecureMode (_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+t") == 0 || mode.compare("-t") == 0){
            ChannelTopicMode(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+l") == 0 || mode.compare("-t") == 0){
            ChanneLimitMode(_serverReactor, ProccessMessage, clientSocket);
        }
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS THE USERS INSIDE THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
        else if (mode.compare("+o") == 0 || mode.compare("-o") == 0){
            ChannelOperatorPrivilege(_serverReactor, ProccessMessage, clientSocket);
        }
        else{
            string Err = ERR_UNKNOWNMODE(mode);
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