/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/28 22:49:04 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

//! Is all the members of the channel have the permission to change the channel modes.

// ~~~~~~~~~~~~~ ONLY THE OPERATORS ARE ALLOWED TO PERFORM SUCH OPERATION ~~~~~~~~~~/ 
void    inviteOnly(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    //* Example: mode #PrivateChannel [+/-] i
    if (ProccessMessage.getParams().size() == 2){
        string mode = ProccessMessage.getParams()[1];
        mode.erase(remove(mode.begin(), mode.end(), '\n'), mode.end());
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            string channelName = ProccessMessage.getParams()[0];
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket))
            {
                if (mode.compare("+i") == 0)
                    _serverReactor.getChannelManager().getChannelByName(channelName).setInviteFlag(true);
                else if (mode.compare("-i") == 0)
                    _serverReactor.getChannelManager().getChannelByName(channelName).setInviteFlag(false);
                // INFORM THE USER THAT THE CHANNEL MODE WAS CHANGED
                string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                message.append(" has changed mode: ");
                message.append(mode);
                message.append("\n");
                set <int> s = _serverReactor.getChannelManager().getChannelByName(channelName).getClientSockets();
                for (set<int>::iterator it = s.begin() ; it !=  s.end() ; it++){
                    if (*it != clientSocket)
                        send(*it, message.c_str(), message.size(), 0);
                }
            }
            else{
                string buffer = "error(482): ";
                buffer.append(channelName);
                buffer.append(": You're not channel operator");
                buffer.append("\n");
                send(clientSocket, buffer.c_str(), buffer.size(), 0);
                throw std::exception();
            }
        }
        else {
            string buffer = "error(403): ";
            buffer.append(ProccessMessage.getParams()[0]);
            buffer.append(" :No such channel");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
        cout << "Change the mode to: " << mode << endl;
    }

}

//* Example: mode #PrivateChannel [+/-] o
void    SecureChannel(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    if (ProccessMessage.getParams().size() == 3 || ProccessMessage.getParams().size() == 2){
        string mode = ProccessMessage.getParams()[1];
        mode.erase(remove(mode.begin(), mode.end(), '\n'), mode.end());
        if (_serverReactor.getChannelManager().itsChannel(ProccessMessage.getParams()[0])){
            string channelName = ProccessMessage.getParams()[0];
            string  key;
            channelName.erase(0, 1);
            if (_serverReactor.getChannelManager().getChannelByName(channelName).isOperator(clientSocket)){
                if (ProccessMessage.getParams().size() == 3){
                    key = ProccessMessage.getParams()[2];
                    key.erase(remove(key.begin(), key.end(), '\n'), key.end());
                }
                if (mode.compare("+k") == 0 && !key.empty()){
                    _serverReactor.getChannelManager().getChannelByName(channelName).setSecurity(true);
                    _serverReactor.getChannelManager().getChannelByName(channelName).setKey(key);
                }
                else if (mode.compare("-k") == 0 && ProccessMessage.getParams().size() == 2)
                    _serverReactor.getChannelManager().getChannelByName(channelName).setSecurity(false);
            }
            else{
                string buffer = "error(482): ";
                buffer.append(channelName);
                buffer.append(": You're not channel operator");
                buffer.append("\n");
                send(clientSocket, buffer.c_str(), buffer.size(), 0);
                throw std::exception();
            }
        }
        else {
            string buffer = "error(403): ";
            buffer.append(ProccessMessage.getParams()[0]);
            buffer.append(" :No such channel");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
    }
    else{
        string buffer = "error(461): ";
        buffer.append(" Not enough parameters");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
}

void    ChannelOperatorPrivilege(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    cout << "Give the Privilege" << endl;
}

//* Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
void     ExecuteCommands::mode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    if (ProccessMessage.getParams().size() >= 2){
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS ONLY THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
        string mode = ProccessMessage.getParams()[1];
        mode.erase(remove(mode.begin(), mode.end(), '\n'), mode.end());
        if (mode.compare("+i") == 0 || mode.compare("-i") == 0){
            inviteOnly(_serverReactor, ProccessMessage, clientSocket);
        }
        else if (mode.compare("+k") == 0 || mode.compare("-k") == 0){
            SecureChannel(_serverReactor, ProccessMessage, clientSocket);
        }
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS THE USERS INSIDE THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
        else if (mode.compare("+o") == 0 || mode.compare("-o") == 0){
            ChannelOperatorPrivilege(_serverReactor, ProccessMessage, clientSocket);
        }
        else{
            string buffer = "error(472): ";
            buffer.append(mode);
            buffer.append(": is unknown mode char to me");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
    }
    else{
        string buffer = "error(461):";
        buffer.append(" Not enough parameters");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
}