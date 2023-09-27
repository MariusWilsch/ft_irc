/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 00:53:10 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/26 18:43:01 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

// void    ExecuteCommands::user(std::string userName);

bool    NickNameValidation(string param)
{
    int a = 0;

    for (unsigned int i = 0; i < param.size(); i++)
    {
        if (isalpha(param[i]))
            a ++;
    }
    if (a == 0)
        return (false);
    return (true);
}


void ExecuteCommands::nick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    string err;


    if (ProcessMessage.getParams().empty())
    {
        string buffer = "error(431) :No nickname given";
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
    else if (!NickNameValidation(ProcessMessage.getParams()[0]))
    {
        string buffer = "error(431): ";
        err = ProcessMessage.getParams()[0];
        err.erase(remove(err.begin(), err.end(), '\n'), err.end());
        buffer.append(err);
        buffer.append(" Erroneus nickname");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
        throw std::exception();
    }
    // SERACH IF certain user uses the nickname:
    //* If the user uses the nickname then add '_' to it, and search again.
 
    map<int, ClientData>::iterator it;
    string nickName = ProcessMessage.getParams()[0];
    string oldNick =  ProcessMessage.getParams()[0];
    
    nickName.erase(remove(nickName.begin(), nickName.end(), '\n'), nickName.end());
    if (nickName.compare(_serverReactor.getClientManager().getClientData(clientSocket).getNickname()) != 0)
    {
        for (it = _serverReactor.getClientManager().getClientBySocket().begin(); it != _serverReactor.getClientManager().getClientBySocket().end(); it++){
            if ((it->second.getNickname().compare(nickName) == 0) && (it->second.getClientSocket() != clientSocket))
            {
                string buffer = "error(433): ";
                err = nickName;
                err.erase(remove(err.begin(), err.end(), '\n'), err.end());
                buffer.append(err);
                buffer.append(" Nickname is already in use");
                buffer.append("\n");
                send(clientSocket, buffer.c_str(), buffer.size(), 0);
                nickName.append("_");
                it = _serverReactor.getClientManager().getClientBySocket().begin();
            }
        }
        //**********  INFORM ALL THE CLIENT THAT THE THIS CLIENT HAS CHANGED IT'S NICKNAME ************** /
        if (nickName.compare(oldNick) != 0)
        {
            string n;

            string message = "<fantastc-server> ";
            n = _serverReactor.getClientManager().getClientData(clientSocket).getNickname();
            n.erase(remove(n.begin(), n.end(), '\n'), n.end());
            message.append(n);
            message.append(" is now known as: ");
            // *** SET USING THE NICKNAME, 
            _serverReactor.getClientManager().getClientData(clientSocket).setNickname(nickName);
            message.append(_serverReactor.getClientManager().getClientData(clientSocket).getNickname());
            message.append("\n");
            for (it = _serverReactor.getClientManager().getClientBySocket().begin(); it != _serverReactor.getClientManager().getClientBySocket().end(); it++){
                if (it->first != clientSocket)
                    send(it->first, message.c_str(), message.size(), 0);
            }
            std::cout << "The nickName has be setted successfully: " << _serverReactor.getClientManager().getClientData(clientSocket).getNickname() << std::endl;
        }
    }
}


void     ExecuteCommands::user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    if (_serverReactor.getClientManager().getClientData(clientSocket).getRegistration() == false)
    {
        if (ProcessMessage.getParams().size() == 3 && ProcessMessage.getTrailing().size() > 1)
        {
            _serverReactor.getClientManager().getClientData(clientSocket).setUsername(ProcessMessage.getParams()[0]);
            _serverReactor.getClientManager().getClientData(clientSocket).setmode(atoi(ProcessMessage.getParams()[1].c_str()));
            _serverReactor.getClientManager().getClientData(clientSocket).setUnused(ProcessMessage.getParams()[2]);
            _serverReactor.getClientManager().getClientData(clientSocket).setRealname(ProcessMessage.getTrailing());
            _serverReactor.getClientManager().getClientData(clientSocket).setRegistration(true);
            
            // ~~~~~~~~~~~~~~~~~~~ INFORM THE SERVER  ~~~~~~~~~~~~~~~~~~~ // 
            std::cout<< "User informations: \n" << std::endl << "USERNAME: " << _serverReactor.getClientManager().getClientData(clientSocket).getUsername() << std::endl;
            std::cout << "MODE: " << _serverReactor.getClientManager().getClientData(clientSocket).getMode() << std::endl;
            std::cout << "UNUSED: " << _serverReactor.getClientManager().getClientData(clientSocket).getUnused() << std::endl;
            std::cout << "REALNAME: " << _serverReactor.getClientManager().getClientData(clientSocket).getRealname() << std::endl;
        }
        else{
            string buffer = "error(461): ";
            buffer.append(" Not enough parameters");
            buffer.append("\n");
            send(clientSocket, buffer.c_str(), buffer.size(), 0);
            throw std::exception();
        }
    }
    else 
    {
        string buffer = "error(462): ";
        buffer.append(" You may not reregister");
        buffer.append("\n");
        send(clientSocket, buffer.c_str(), buffer.size(), 0);
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
            string buffer = "error(462): ";
            buffer.append(" You may not reregister");
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
