/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:23 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/04 17:28:59 by mwilsch          ###   ########.fr       */
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

bool	ClientData::getRegistration() const{
	return (this->_registration);
}

string	ClientData::getPassword() const{
	return (this->_password);
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

void	ClientData::setPassword(string pass){
	this->_password = pass;
}

/*					CLIENT MANAGER						*/

ClientManager::ClientManager( void ) {};

ClientManager::~ClientManager() {};

/*					MEMBER FUNCTIONS					*/

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
	map<int, ClientData>::iterator it =  this->_ClientsBySocket.find(clientSocket);
	return (it->second);
}

map<int, ClientData>&				ClientManager::getClientBySocket(){
	return (this->_ClientsBySocket);
}

bool	ClientManager::MatchNickName(set <int> ChannelMembers, string nickname) {
	for (set<int>::iterator it = ChannelMembers.begin(); it != ChannelMembers.end(); it++) {
		map <int, ClientData>::iterator client = _ClientsBySocket.find(*it);
		if (client != _ClientsBySocket.end()){
			if (client->second.getNickname().compare(nickname) == 0){
				return (true);
			}
		}
	}
	return (false);
}

void	ClientManager::doesClientExist(string& nickname) {
	
	map<int, ClientData>::iterator it;



	for (it = _ClientsBySocket.begin(); it != _ClientsBySocket.end(); it++) {
		cout << "|" << getClientData(it->first).getNickname() << "|" << endl;

		// if (getClientData(it->first).getNickname().compare(nickname) == 0) {
		// 	cout << "Nickname was found inside of map container" << endl;
		// 	break ;
		// }
	}
	cout << "Couldn't find the nickname within the map" << endl;
}