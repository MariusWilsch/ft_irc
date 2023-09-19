/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:20 by mwilsch           #+#    #+#             */
/*   Updated: 2023/09/19 02:52:14 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CLIENT DATA	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/

ChannelData::ChannelData( void ){
    _name = "";
    _topic = "";
    _clientSockets = set<int>();
    _operators = set<int>();
}

ChannelData::~ChannelData( void ){}

/********************** GETTERS	***********************/

string ChannelData::getName( void ) const{
    return (this->_name);
}

string ChannelData::getTopic( void ) const{
    return (this->_topic );
}

set <int>   ChannelData::getClientSockets( void ) const{
    return (this->_clientSockets);
}

set <int>   ChannelData::getOperators( void ) const{
    return (this->_operators);
}

/********************** SETTERS	***********************/

void    ChannelData::setName(string name){
    this->_name = name;
}

void    ChannelData::setTopic(string topic){
    this->_topic = topic;
}

void ChannelData::addClient(int clientSocket){
    this->_clientSockets.insert(clientSocket);
}

void ChannelData::addOperator(int clientSocket){
    this->_operators.insert(clientSocket);
}

/************************* ClientData methods ***************************/

bool ChannelData::isOperator(int clientSocket){
    if (_operators.find(clientSocket) != _operators.end())
        return (true);
    return (false);
}

bool ChannelData::isCLient(int clientSocket){
    if (_clientSockets.find(clientSocket) != _operators.end())
        return (true);
    return (false);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CLIENT MANAGER	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/

ChannelManager::ChannelManager(){
    _channels = map<string, ChannelData>();
};

ChannelManager::~ChannelManager(){};

ChannelData& ChannelManager::getChannelByName(string name){
    map<string, ChannelData>::iterator it = _channels.find(name);
    return (it->second);
}

void ChannelManager::addChannel(string name, ChannelData channelData){
    _channels.insert(std::pair<string, ChannelData>(name, channelData));
}