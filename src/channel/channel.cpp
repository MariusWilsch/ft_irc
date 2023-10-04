/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:20 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/04 17:12:46 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CLIENT DATA	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/

ChannelData::ChannelData( void ){
    _name = "";
    _topic = "";
    _key = "";
    _limit = -1;
    _securityFlag = false;
    _inviteOnlyFlag = false;
    _topicFlag = false;
    _limitFlag = false;
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

string		ChannelData::getKey( void ) const{
    return (this->_key);
}

size_t      ChannelData::getLimit( void ) const{
    return (this->_limit);
}

bool    ChannelData::getSecurity(void) const{
    return (this->_securityFlag);
}

bool		ChannelData::getTopicFlag ( void ) const{
    return (this->_topicFlag);
}

bool		ChannelData::getInviteFlag ( void )  const{
    return (this->_inviteOnlyFlag);
}

bool        ChannelData::getLimitFlag( void ) const{
    return (this->_limitFlag);
}

set <int>   ChannelData::getClientSockets( void ) const{
    return (this->_clientSockets);
}

set <int>   ChannelData::getOperators( void ) const{
    return (this->_operators);
}

set <string>	ChannelData::getInviteList( void ) const{
    return (this->_inviteList);
}

/********************** SETTERS	***********************/



void    ChannelData::setName(string name){
    this->_name = name;
}

void    ChannelData::setTopic(string topic){
    this->_topic = topic;
}

void		ChannelData::setKey (string key){
    this->_key = key;
}

void        ChannelData::setLimit(size_t limit){
    this->_limit = limit;
}

void		ChannelData::setSecurity( bool s){
    this->_securityFlag = s;
}

void    	ChannelData::setInviteFlag(bool stat){
    this->_securityFlag = stat;
}

void		ChannelData::setTopicFlag(bool stat){
    this->_topicFlag = stat;
}

void        ChannelData::setLimitFlag(bool stat){
    this->_limitFlag;
}

void ChannelData::addClient(int clientSocket){
    this->_clientSockets.insert(clientSocket);
}

void ChannelData::addOperator(int clientSocket){
    this->_operators.insert(clientSocket);
}

void		ChannelData::addGuest( string nickName){
    this->_inviteList.insert(nickName);
}

void    ChannelData::removeClient(int SocketID){
    this->_clientSockets.erase(SocketID);
}

void		ChannelData::removeOperator(int SocketID){
    this->_operators.erase(SocketID);
}


/************************* ClientData methods ***************************/

bool ChannelData::isOperator(int clientSocket){
    if (_operators.find(clientSocket) != _operators.end())
        return (true);
    return (false);
}

bool ChannelData::isCLient(int clientSocket){
    if (_clientSockets.find(clientSocket) != _clientSockets.end())
        return (true);
    return (false);
}

bool		ChannelData::isInvited(string nickName){
    if (_inviteList.find(nickName) != _inviteList.end())
        return (true);
    return (false);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL MANAGER	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/

ChannelManager::ChannelManager(){
    _channels = map<string, ChannelData>();
};

ChannelManager::~ChannelManager(){};

ChannelData& ChannelManager::getChannelByName(string name){
    map<string, ChannelData>::iterator it = _channels.find(name);
    return (it->second);
}

map<string, ChannelData>&	ChannelManager::getChannels( void ){
    return (this->_channels);
}

bool    ChannelManager::channelExistence(string name){
    map<string, ChannelData>::iterator it = _channels.find(name);
    if (it == _channels.end())
        return (false);
    return (true);
}

void ChannelManager::addChannel(string name, ChannelData &channelData){
    _channels.insert(std::pair<string, ChannelData>(name, channelData));
}

bool			ChannelManager::itsChannel( string name ){
    if (name[0] == '#')
    {
        name.erase(0, 1);
        if (this->channelExistence(name))
            return (true);
    }
    return (false);
}
