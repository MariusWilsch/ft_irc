/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:17:01 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/27 15:47:57 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

// Cut new line from the strings of all the element of the vector.

// ~~~~~~~~~~~~~ ONLY THE OPERATORS ARE ALLOWED TO PERFORM SUCH OPERATION ~~~~~~~~~~/ 
void    inviteOnly(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
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


/*
    mode effect on channel:
        + To join the channel need to check the secure flag of the channel if it's on "1" means it's necessary to provide a key
        + if the flag is off "0" means there is no need to provide a key to join the channel.
*/
void    SecureChannel(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket){
    //* Example: mode #PrivateChannel [+/-] o
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

void     ExecuteCommands::mode(ServerReactor &_serverReactor, Message &ProccessMessage, int clientSocket)
{
    // Check the syntax of the parameters befor executing the command
    //* Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
    //* Example: mode #PrivateChannel [+/-] i
    // for (unsigned int i = 0; i < ProccessMessage.getParams().size(); i++){
    //     cout << ProccessMessage.getParams()[i] << endl;
    // }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS ONLY THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    // Invite list mode : [+i] [-i]
    string mode = ProccessMessage.getParams()[1];
    mode.erase(remove(mode.begin(), mode.end(), '\n'), mode.end());
    if (mode.compare("+i") == 0 || mode.compare("-i") == 0){
        inviteOnly(_serverReactor, ProccessMessage, clientSocket);
    }
    else if (mode.compare("+k") == 0 || mode.compare("-k") == 0){
        SecureChannel(_serverReactor, ProccessMessage, clientSocket);
    }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MODES THAT AFFECTS THE USERS INSIDE THE CHANNEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    else{
        string buffer = "error(472): ";
        buffer.append(mode);
        buffer.append(": is unknown mode char to me");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
}