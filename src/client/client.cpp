/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:23 by mwilsch           #+#    #+#             */
/*   Updated: 2023/09/17 19:24:17 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*			CLASS DEFAULT FUNCTIONS: CLIENT DATA			*/

ClientData::ClientData( void )
{
	_mode = 0;
	_realname = "";
	_nickname = "";
	_username = "";
	// _userType = USER;
	_unused = "*";
}

ClientData::ClientData( int clientSocket, unsigned int clientsNumber)
{
	// Give the user an initial authentication.
	string user;

	user = "User";
	user.append(std::to_string(clientsNumber));
	_clientSocket = clientSocket;
	_realname = user;
	_nickname = user;
	_username = user;
	_registration = false;
}

ClientData::~ClientData() {};

/*			GETTERS			*/
int		ClientData::getMode(){
	return (this->_mode);
}

string	ClientData::getRealname(){
	return (this->_realname);
}

string	ClientData::getNickname(){
	return (this->_nickname);
}

string	ClientData::getUsername(){
	return (this->_username);
}

string 	ClientData::getUnused(){
	return (this->_unused);
}

bool	ClientData::getRegistration(){
	return (this->_registration);
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

void	ClientData::setRegistration( bool b){
	this->_registration = b;
}
/*			CLASS DEFAULT FUNCTIONS: CLIENT MANAGER			*/

ClientManager::ClientManager( void ) {};

ClientManager::~ClientManager() {};

/*			MEMBER FUNCTIONS			*/

void	ClientManager::addClient( int clientSocket)
{
	std::cout << "Creating Client Data and adding client to map container" << std::endl;
	ClientData newClient(clientSocket, _ClientsBySocket.size());
	_ClientsBySocket.insert(std::pair<int, ClientData>(clientSocket, newClient));
}

void	ClientManager::removeClient( int clientSocket )
{
	std::cout << "Removing client from map container" << std::endl;
	_ClientsBySocket.erase(clientSocket);
}



ClientData& ClientManager::getClientData( int clientSocket )
{
	try{
		map<int, ClientData>::iterator it =  this->_ClientsBySocket.find(clientSocket);
		if (it != this->_ClientsBySocket.end())
			return (it->second);
		throw std::exception();
	}
	catch (std::exception &ex){
		std::cout << "Client doesn't exist" << std::endl;
	}
}


map<int, ClientData>&				ClientManager::getClientBySocket(){
	return (this->_ClientsBySocket);
}