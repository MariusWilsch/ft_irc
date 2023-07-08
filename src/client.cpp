/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 12:41:54 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/08 14:15:44 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/client.hpp"

/*			Client Data Implementation			*/

ClientData::ClientData(int clientSocket) : _clientSocket(clientSocket) {
	_nickname = "default";
	_username = "default";
	_password = "default";
	_role = USER;
	_isAuthenticated = false;
	cout << "New ClientData created" << endl;
}

int	ClientData::getClientSocket() const {
	return (_clientSocket);
}

/*			Client Manager Implementation			*/

ClientManager::ClientManager( void ) {
	cout << "ClientManager created" << endl;
}

void	ClientManager::addClient(int clientSocket, const ClientData &clientData) {
	_activeClientsBySocket.insert(std::pair<int, ClientData>(clientSocket, clientData));
	// Print the map
	cout << "Active Clients:" << endl;
	for (map<int, ClientData>::iterator it = _activeClientsBySocket.begin(); it != _activeClientsBySocket.end(); ++it) {
		cout << it->first << " => " << it->second.getClientSocket() << endl;
	}
}


