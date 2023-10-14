/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 00:53:10 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/14 14:38:19 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool    NickNameValidation(string param)
{
    int a = 0;

    for (unsigned int i = 0; i < param.size(); i++){
        if (isalpha(param[i]))
            a ++;
    }
    if (a == 0)
        return (false);
    return (true);
}

// Inform when the registration is completed,
    // + the pass command needs to be the first command. or the registration can't be done.
    // The index[0]: PASS
    // The index[1]: USER
    // The index[2]: NICK

void ExecuteCommands::nick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    ClientData  &client = _serverReactor.getClientManager().getClientData(clientSocket);
    string err;

    if (ProcessMessage.getParams().empty() || !NickNameValidation(ProcessMessage.getParams()[0])){
        string Err = ERR_NONICKNAMEGIVEN();
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
    map<int, ClientData>::iterator it;
    string nickName = ProcessMessage.getParams()[0];
    string oldNick =  client.getNickname();
    if (nickName.compare(client.getNickname()) != 0)
    {
        map <int, ClientData> &clientSet = _serverReactor.getClientManager().getClientBySocket();
        for (it = clientSet.begin(); it != clientSet.end(); it++){
            if ((it->second.getNickname().compare(nickName) == 0) && (it->second.getClientSocket() != clientSocket))
            {
                string Err = ERR_NICKNAMEINUSE(nickName);
                send(clientSocket, Err.c_str(), Err.size(), 0);
                nickName.append("_");
                it = clientSet.begin();
            }
        }
        if (nickName.compare(oldNick) != 0){
            client.setNickname(nickName);
            if (client.isRegistered())
                _serverReactor.sendMsg(clientSocket, client.getClientInfo(), "NICK", nickName);
        }
        client.setRegisteration(true, 2);
        if (client.getRegistration()[0] && client.getRegistration()[1]){
            _serverReactor.sendNumericReply(clientSocket, "001", client.getNickname(), "Welcome to the IRC Network, " + client.getNickname());
        }
    }
}

void     ExecuteCommands::user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
    ClientData  &client = _serverReactor.getClientManager().getClientData(clientSocket);
    
    if (!client.isRegistered()){
        if (ProcessMessage.getParams().size() >= 4)
        {
            _serverReactor.getClientManager().getClientData(clientSocket).setUsername(ProcessMessage.getParams()[0]);
            _serverReactor.getClientManager().getClientData(clientSocket).setmode(atoi(ProcessMessage.getParams()[1].c_str()));
            _serverReactor.getClientManager().getClientData(clientSocket).setUnused(ProcessMessage.getParams()[2]);
            _serverReactor.getClientManager().getClientData(clientSocket).setRealname(ProcessMessage.getParams()[3]);
            string nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
            _serverReactor.getClientManager().getClientData(clientSocket).setRegisteration(true, 1);
            if (client.getRegistration()[0] && client.getRegistration()[2]){
                _serverReactor.sendNumericReply(clientSocket, "001", nickname, "Welcome to the IRC Network, " + nickname);
            } 
        }
        else{
            string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
    }
    else {
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ALREADYREGISTRED());
        throw std::exception();
    }
}

// Manda
void ExecuteCommands::pass(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    ClientData  &client = _serverReactor.getClientDataFast(clientSocket);
    if (ProcessMessage.getParams().size() >= 1){
        if (!client.getRegistration()[1] && !client.getRegistration()[2]){
            client.setRegisteration(true, 0);
            if (_serverReactor.getServerPassword().compare(ProcessMessage.getParams()[0]) != 0){
                _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_PASSWDMISMATCH());
                close(clientSocket);
                throw std::exception();
            }
        }
        else{
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ALREADYREGISTRED());
            throw std::exception();
        }
    }
    else{
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(ProcessMessage.getCommand()));
        throw std::exception();
    }
}
