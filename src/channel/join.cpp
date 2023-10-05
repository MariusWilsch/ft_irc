/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:13:30 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/05 18:09:40 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 
#include"ExecuteCommands.hpp"

int seperateNamesKeys(std::vector<string> &ChannelNames, std::vector<string> &ChannelKeys, Message &ProcessMessage){
    if (ProcessMessage.getParams().size() == 0)
        return (-1);
    string param = ProcessMessage.getParams()[0];
    param.erase(remove(param.begin(), param.end(), '\n'), param.end());
    if (ProcessMessage.getParams().size() == 1 && param.compare("0") == 0){
        return (0);
    }
    if (ProcessMessage.getParams().size() < 1 || ExecuteCommands::whiteCheck(ProcessMessage.getParams()[0]))
        return (-1);
    for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++)
    {
        param = ProcessMessage.getParams()[i];
        param.erase(std::remove(param.begin(), param.end(), ' '), param.end());
        param.erase(std::remove(param.begin(), param.end(), '\n'), param.end());
        unsigned int c = 0;
        for (unsigned int i = 0; i < param.size(); i++){
            if (param[i] == ',')
                c++;
        }
        if (c == 0){
            if (param[0] == '#'){
                param.erase(0, 1);
                ChannelNames.push_back(param);
            }
            else if (!ExecuteCommands::whiteCheck(param)){
                ChannelKeys.push_back(param);
            }
        }
        else{
            for (unsigned int j = 0; j <= c; j++){
                unsigned int  e = param.find(',');
                string sub = param.substr(0, e);
                sub.erase(std::remove(sub.begin(), sub.end(), ' '), sub.end());
                param = param.substr(e + 1);
            
                if (sub[0] == '#'){
                    sub.erase(0, 1);
                    ChannelNames.push_back(sub);
                }
                else if (!ExecuteCommands::whiteCheck(sub)){
                    ChannelKeys.push_back(sub);
                }
            }
        }
    }   
    if (ChannelKeys.size() > ChannelNames.size())
        return (-1);
    for (unsigned int i = ChannelKeys.size(); i < ChannelNames.size(); i++){
        ChannelKeys.push_back("");
    }
    return (1);
}

void ExecuteCommands::join(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    std::vector<string> ChannelNames;
    std::vector<string> ChannelKeys;

    int stat = seperateNamesKeys(ChannelNames, ChannelKeys, ProcessMessage);
    if (stat == -1){
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
    if (stat == 0){
        map<string, ChannelData>::iterator  it;
        map<string, ChannelData> &m = _serverReactor.getChannelManager().getChannels();
        for (it = m.begin(); it != m.end();)
        {
            set<int>::iterator RemoveIt;
            RemoveIt = it->second.getClientSockets().find(clientSocket);
            if (RemoveIt != it->second.getClientSockets().end())
                it->second.removeClient(*RemoveIt);
            RemoveIt = it->second.getOperators().find(clientSocket);
            if (RemoveIt != it->second.getOperators().end())
                it->second.removeOperator(*RemoveIt);
            if (it->second.getOperators().size() == 0 && it->second.getClientSockets().size() == 0){
                _serverReactor.getChannelManager().getChannels().erase(it++);
            }
            else
                ++it;
        }
    }

    for (unsigned int i = 0; i < ChannelNames.size(); i++){
        bool Joined = false;
        // Create the channel if it's not exist, and make the current client as the operator of the channel.
        if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == false){
            ChannelData    NewChannel;
            NewChannel.setName(ChannelNames[i]);
            NewChannel.addClient(clientSocket);
            NewChannel.addOperator(clientSocket);
            if (ChannelKeys.size() >= 1 && !ChannelKeys[i].empty()){
                NewChannel.setKey(ChannelKeys[i]);
                NewChannel.setSecurity(true);
            }
            _serverReactor.getChannelManager().addChannel(ChannelNames[i], NewChannel);
            Joined = true;
        }
        // Channel is exist, Check Security cases, and inform all the channels clients.
        else {
            ChannelData& Channel = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]);
            if (!Channel.isCLient(clientSocket)){
                if (Channel.getInviteFlag()){
                    if (!Channel.isInvited(_serverReactor.getClientManager().getClientData(clientSocket).getNickname())){
                        // Don't accecpt it's joining to channel.
                        //! REPLACE THIS BY AN APPROPRIATE NUMERIC REPLY.
                        string buffer = "error: ";
                        buffer.append(" this channel is Invite only");
                        buffer.append("\n");
                        send(clientSocket, buffer.c_str(), buffer.size(), 0);
                        throw std::exception();
                    }
                }
                // The channel is private.
                if (Channel.getSecurity() == true){
                    if (ChannelKeys.size() >= 1 && (ChannelKeys[i].c_str() != NULL)){
                        if (Channel.getKey().compare(ChannelKeys[i]) == 0)
                        {
                            // The key is valid: add the client to channelClients as a normal client.
                            Channel.addClient(clientSocket);
                            Joined = true;
                        }
                        else{
                            // The key not matched: informe the specific client that the password is wrong by sending ERR_BADCHANNELKEY.
                            string Err = ERR_BADCHANNELKEY(Channel.getName());
                            send(clientSocket, Err.c_str(), Err.size(), 0);
                            throw std::exception();
                        }
                    }
                }
                // The channel is public.
                else{
                    if (ChannelKeys.size() >= 1 && (ChannelKeys[i].empty())){
                            Channel.addClient(clientSocket);
                            Joined = true;
                        }
                    else{
                        string  Err = ERR_BADCHANNELKEY(Channel.getName());
                        send(clientSocket, Err.c_str(), Err.size(), 0);
                        throw std::exception();
                    }
                }
                if (Joined == true){
                    if (Channel.getTopicFlag()){
                        string Rpl =  RPL_TOPIC(Channel.getName() ,Channel.getTopic());
                        send(clientSocket, Rpl.c_str(), Rpl.size(), 0);
                    }
                    else{
                        string Rpl =  RPL_NOTOPIC(Channel.getName());
                        send(clientSocket, Rpl.c_str(), Rpl.size(), 0);
                    }
                    string message = _serverReactor.getClientManager().getClientData(clientSocket).getUsername();
                    message.append(" has joined the Channel # ");
                    message.append(Channel.getName());
                    message.append("\n");
                    set <int> s = Channel.getClientSockets();
                    for (set<int>::iterator it = s.begin() ; it !=  s.end() ; it++){
                        if (*it != clientSocket)
                            send(*it, message.c_str(), message.size(), 0);
                    }
                }
            }
        }
    }
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DISPLAY CHANNELS INFORMATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    map<string, ChannelData>::iterator  it;
    std::cout << "Number of channels: " << _serverReactor.getChannelManager().getChannels().size() << std::endl;
    map<string, ChannelData> m = _serverReactor.getChannelManager().getChannels();
    for (it = m.begin(); it != m.end(); it++)
    {
        std::cout << "Channel name: " << it->second.getName() << std::endl;
        if (it->second.getSecurity())
            cout << "~~~> Channel is private " << endl;
        else
            cout << "~~~> Channel is public " << endl;
        set<int>::iterator cl;
        set<int> s = it->second.getClientSockets();
        for (cl = s.begin(); cl != s.end(); cl++){
            std::cout << "  >>> Client: " << *cl << endl;
        } 
        
        set<int>::iterator op;
        set<int> f = it->second.getOperators();
        for (op = f.begin(); op != f.end(); op++){
            std::cout << "  >>> operator: " << *op << endl;
        }
    }
}
