/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverEvents.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:17:06 by mwilsch           #+#    #+#             */
/*   Updated: 2023/09/27 15:37:39 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*			EVENTS			*/

// TODO: Refactor this to be more readable and use more member variables
void	ServerReactor::recieveIncomingMessage( int clientSocket )
{
	int			bytesRead;
	char		buffer[1024];
	string		message;
	size_t		pos;

	pos = 0;
	while (pos != string::npos)
	{
		bytesRead = recv(clientSocket, &buffer, 1023, 0);
		if (bytesRead == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			writeServerError("recv", "Failed to receive message", errno);
		if (bytesRead == 0)
		{
			updateMoinitoring(clientSocket, EVFILT_READ, EV_DELETE);
			_clientManager.removeClient(clientSocket);
			return ;
		}
		buffer[bytesRead] = '\0';
		message.append(buffer);
		pos = message.find("\r\n");
		memset(buffer, 0, bytesRead);
	}
	std::cout << "Message: " << message << std::endl;
	Message processMessage(message, _properties);
	// TODO: Implement command execution

	//****************************************************************************************/
	// initialize all the clients by giving them a default authentication.
	ExecuteCommands::execute(*this, processMessage, clientSocket);
}

void	ServerReactor::acceptNewClient( void )
{
	struct sockaddr_in	clientAddress;
	socklen_t						clientAddressSize;
	int								clientSocket;
	
	std::cout << "Accepting new client" << std::endl;
	clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocket == -1)
		writeServerError("accept", "Failed to accept new client", errno);
	// TODO: Check if above max clients
	setToNonBlocking(clientSocket);
	updateMoinitoring(clientSocket, EVFILT_READ, EV_ADD);
	_clientManager.addClient(clientSocket);
}


void	ServerReactor::run( void )
{
	struct kevent	eventList[32];
	int				eventCount, fd, filter;
	
	while (_isShutdown == false)
	{
		eventCount = kevent(_kq, NULL, 0, eventList, 32, NULL);
		if (eventCount == -1)
			writeServerError("kevent", "Failed to retrieve events", errno);
		for (int i = 0; i < eventCount; i++)
		{
			fd = eventList[i].ident;
			filter = eventList[i].filter;
			if (fd == _serverSocket && filter == EVFILT_READ)
				acceptNewClient();
			if (fd != _serverSocket && filter == EVFILT_READ)
				recieveIncomingMessage(fd);
			// if (fd != _serverSocket && filter == EVFILT_WRITE)
			// 	handleOutgoingMessage(fd);
		}
	}
	cout << "Server: " << this->_serverName << " is shutdown." << endl;
}