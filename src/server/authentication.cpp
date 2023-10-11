/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 00:53:10 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/11 21:29:23 by ahammout         ###   ########.fr       */
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
            if (client.getRegistration())
                _serverReactor.sendMsg(clientSocket, client.getClientInfo(), "NICK:", nickName);
        }
        if ((!client.getUsername().empty()) && (!client.getRegistration())){
            client.setRegistration(true);
            _serverReactor.sendNumericReply(clientSocket, "001", client.getNickname(), "Welcome to the IRC Network, " + client.getNickname());
        }
    }
}

void     ExecuteCommands::user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    if (_serverReactor.getClientManager().getClientData(clientSocket).getRegistration() == false)
    {
        if (ProcessMessage.getParams().size() >= 4)
        {
            _serverReactor.getClientManager().getClientData(clientSocket).setUsername(ProcessMessage.getParams()[0]);
            _serverReactor.getClientManager().getClientData(clientSocket).setmode(atoi(ProcessMessage.getParams()[1].c_str()));
            _serverReactor.getClientManager().getClientData(clientSocket).setUnused(ProcessMessage.getParams()[2]);
            _serverReactor.getClientManager().getClientData(clientSocket).setRealname(ProcessMessage.getParams()[3]);
            string nickname = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
            if (!nickname.empty()){
                _serverReactor.getClientManager().getClientData(clientSocket).setRegistration(true);
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
        string Err = ERR_ALREADYREGISTRED();
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
}

void ExecuteCommands::pass(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    if (ProcessMessage.getParams().size() == 1){
        if (_serverReactor.getClientManager().getClientData(clientSocket).getRegistration() == false){
             _serverReactor.getClientManager().getClientData(clientSocket).setPassword(ProcessMessage.getParams()[0]);
        }
        else{
            string Err = ERR_ALREADYREGISTRED();
            send(clientSocket, Err.c_str(), Err.size(), 0);
            throw std::exception();
        }
    }
    else{
        string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
        send(clientSocket, Err.c_str(), Err.size(), 0);
        throw std::exception();
    }
}
