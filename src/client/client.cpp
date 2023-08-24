/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:02:23 by mwilsch           #+#    #+#             */
/*   Updated: 2023/08/24 21:23:49 by verdant          ###   ########.fr       */
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

ClientData::ClientData( int clientSocket ) : _clientSocket(clientSocket) {};

ClientData::~ClientData() {};

/*			GETTERS			*/


/*			SETTERS			 */


/*			CLASS DEFAULT FUNCTIONS: CLIENT MANAGER			*/

ClientManager::ClientManager( void ) {};

ClientManager::~ClientManager() {};

/*			MEMBER FUNCTIONS			*/

void	ClientManager::addClient( int clientSocket )
{
	std::cout << "Creating Client Data and adding client to map container" << std::endl;
	ClientData newClient(clientSocket);
	_ClientsBySocket.insert(std::pair<int, ClientData>(clientSocket, newClient));
}

void	ClientManager::removeClient( int clientSocket )
{
	std::cout << "Removing client from map container" << std::endl;
	_ClientsBySocket.erase(clientSocket);
}



