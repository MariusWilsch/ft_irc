/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:23 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/24 17:46:05 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*			CLASS DEFAULT FUNCTIONS: CLIENT DATA			*/

ClientData::ClientData( void )
{
	_mode = 0;
	_realname = "";
	_nickname = "*";
	_username = "";
	_unused = "*";
}

ClientData::ClientData( int clientSocket) : _clientSocket(clientSocket)
{
	_realname = "";
	_nickname = "*";
	_username = "";
	for (unsigned int i = 0; i < 3; i++){
		this->_Registration.push_back(false);
	}
}

ClientData::~ClientData() {};

/*			GETTERS			*/

int		ClientData::getClientSocket( void ) const{
	return (this->_clientSocket);
}
int		ClientData::getMode() const{
	return (this->_mode);
}

string	ClientData::getRealname() const{
	return (this->_realname);
}

string	ClientData::getNickname() const{
	return (this->_nickname);
}

string	ClientData::getUsername() const{
	return (this->_username);
}

string 	ClientData::getUnused() const{
	return (this->_unused);
}

vector<bool>&	ClientData::getRegistration(){
	return (this->_Registration);
}

string	ClientData::getPassword() const{
	return (this->_password);
}

string ClientData::getClientIP() const{
	return (this->_clientIP);
}

string ClientData::getClientInfo() const {
	return (getNickname() + "!~" + getUsername() + "@" + getClientIP());
}

/*			SETTERS			 */

void	ClientData::setmode(int mode){
	this->_mode = mode;
}

void	ClientData::setRealname(string realname){
	this->_realname = realname;
}

void	ClientData::setNickname(string nickname){
	this->_nickname = nickname;
}

void	ClientData::setUsername(string username){
	this->_username = username;
}

void	ClientData::setUnused(string unused){
	this->_unused = unused;
}

void	ClientData::setRegisteration( bool b, int index){
	this->_Registration[index] = b;
}

bool	ClientData::isRegistered(){
	for (unsigned int i = 0; i < 3; i++){
		if (this->_Registration[i] == false)
			return (false);
  	}
	return (true);
}

void	ClientData::setPassword(string pass){
	this->_password = pass;
}

void	ClientData::setClientIP(string clientIP){
	this->_clientIP = clientIP;
}

/*					CLIENT MANAGER						*/

ClientManager::ClientManager( void ) {};

ClientManager::~ClientManager() {};

/*					MEMBER FUNCTIONS					*/

void	ClientManager::addClient( int clientSocket, string clientIP)
{
	std::cout << "Creating Client Data and adding client to map container" << std::endl;
	ClientData newClient(clientSocket);
	newClient.setClientIP(clientIP);
	_ClientsBySocket.insert(std::pair<int, ClientData>(clientSocket, newClient));
}

void	ClientManager::removeClient( int clientSocket )
{
	std::cout << "Removing client from map container" << std::endl;
	_ClientsBySocket.erase(clientSocket);
	close(clientSocket);
}

ClientData& ClientManager::getClientData( int clientSocket )
{
	map<int, ClientData>::iterator it =  this->_ClientsBySocket.find(clientSocket);
	return (it->second);
}

map<int, ClientData>&				ClientManager::getClientBySocket(){
	return (this->_ClientsBySocket);
}

int	ClientManager::MatchNickName(set <int> ChannelMembers, string nickname) {
	
	for (set<int>::iterator it = ChannelMembers.begin(); it != ChannelMembers.end(); it++) {
		map <int, ClientData>::iterator client = _ClientsBySocket.find(*it);
		if (client != _ClientsBySocket.end()){
			if (client->second.getNickname().compare(nickname) == 0){
				return (client->second.getClientSocket());
			}
		}
	}
	return (-1);
}

int	ClientManager::getClientSocketByNick(string& nickname) {
	
	map<int, ClientData>::iterator it;

	for (it = _ClientsBySocket.begin(); it != _ClientsBySocket.end(); ++it) {
		if (getClientData(it->first).getNickname().compare(nickname) == 0) {
			return (it->first);
		}
	}
	return (-1);
}
