/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 12:41:54 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/09 14:14:35 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/client.hpp"

/*			Client Data Implementation			*/

ClientData::ClientData( void ) {
	_clientSocket = -1;
	_nickname = "default";
	_username = "default";
	_password = "default";
	_role = USER;
	_isAuthenticated = false;
}

ClientData::ClientData(int clientSocket) : _clientSocket(clientSocket) {} ;

int	ClientData::getClientSocket() const {
	return (_clientSocket);
}


/*			Client Manager Implementation			*/

ClientManager::ClientManager( void ) {
	// _usedNicknames = set<string>();
	// _activeClientsBySocket = map<int, ClientData>();
	// _inactiveClientsByNickname = map<string, ClientData>();
	cout << "ClientManager created" << endl;
}



// void	ClientManager::printActiveClients( void ) const 
// {
// 	cout << "Active Clients:" << endl;
// 	for (map<int, ClientData>::iterator it = _activeClientsBySocket.begin(); it != _activeClientsBySocket.end(); ++it) {
// 		cout << it->first << " => " << it->second.getClientSocket() << endl;
// 	}
// }

void	ClientManager::removeClient(int clientSocket) {
	_activeClientsBySocket.erase(clientSocket); // Is this all I need to do?
	close(clientSocket);
	cout << "Client removed" << endl;
}





	// TODO: check if client already exists by nickname in inactive clients map
		// Client Exists
			// if client provides correct nickname and password, move client from inactive to active
			// if client provides incorrect nickname and password, return error message and reprint prompt
		// Client Does Not Exist
			// prompt client to provide nickname, password, username to create new client
			// check if nickname is already taken
				// if nickname is taken, return error message and reprint prompt
			// if nickname is not taken, ask client to authenticate by providing password and nickname
				// if client provides incorrect password, return error message and reprint prompt
				// if client provides correct password, add client to active clients map
void	ClientManager::addClient(int clientSocket, const ClientData &clientData) {
	// _activeClientsBySocket.insert(std::pair<int, ClientData>(clientSocket, clientData));
	cout << "Not implemented yet. See " << endl;
}


