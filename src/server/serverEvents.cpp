/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverEvents.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:17:06 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/10 17:18:24 by mwilsch          ###   ########.fr       */
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

	while (1)
	{
		bytesRead = recv(clientSocket, &buffer, 1023, 0);
		if (bytesRead == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			writeServerError("recv", "Failed to receive message", errno);
		if (bytesRead == 0)
		{
			updateMoinitoring(clientSocket, EVFILT_READ, EV_DELETE);
			_clientManager.removeClient(clientSocket);
			_channelManager.removeFromChannels(clientSocket);
			return ;
		}
		message.append(buffer, bytesRead);
		memset(buffer, 0, sizeof(buffer));
		if (message.find("\n") != string::npos)
			break;
	}


	// cout << "In ServerReactor::recieveIncomingMessage: " << message << endl;
	
	Message processMessage(message, _properties);
	// TODO: Implement command execution

	//****************************************************************************************/
	// initialize all the clients by giving them a default authentication.
	// ExecuteCommands::execute(*this, processMessage, clientSocket);
}

void	ServerReactor::acceptNewClient( void )
{
	struct sockaddr_in	clientAddress;
	socklen_t						clientAddressSize;
	int								clientSocket;
	
	std::cout << "Accepting new client" << std::endl;
	clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);


	string clientIP = inet_ntoa(clientAddress.sin_addr);
  std::cout << "Client IP Address: " << clientIP << std::endl;


	if (clientSocket == -1)
		writeServerError("accept", "Failed to accept new client", errno);
	// TODO: Check if above max clients
	setToNonBlocking(clientSocket);
	updateMoinitoring(clientSocket, EVFILT_READ, EV_ADD);

// Convert char * to stirng 
	
	_clientManager.addClient(clientSocket, clientIP);
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
		}
	}
	cout << "Server: " << this->_serverName << " is shutdown." << endl;
}