/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:10 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 14:16:27 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"

ServerReactor::ServerReactor(int port, int maxClients, string connectionPassword)
	: _serverSocket(setupServerSocket(port)), _maxClients(maxClients), _connectionPassword(connectionPassword)
{
	_isShutdown = false;
	// Setup kqueue
	_kq = kqueue();
	if (_kq == -1)
		this->writeError("kqueue", "Failed to create kqueue");
	// Add server socket to kqueue
	struct kevent evSet;
	EV_SET(&evSet, _serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1)
		this->writeError("kevent", "Failed to add server socket to kqueue");
	// Setup client manager
	// _clientManager = ClientManager();
	// Setup channel manager
	// _channelManager = ChannelManager();
}

int	ServerReactor::setupServerSocket( int port )
{
	struct sockaddr_in	serverAddress;
	int									serverSocket;
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		this->writeError("socket", "Failed to create socket");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		this->writeError("bind", "Failed to bind socket");
	if (listen(serverSocket, 20) == -1)
		this->writeError("listen", "Failed to listen on socket");
	this->setBlocking(_serverSocket);
	return (serverSocket);
}

ServerReactor::~ServerReactor()
{
	cout << "Server shutting down" << endl;
	close(_serverSocket);
}

void	ServerReactor::writeError( string functionName, string errorMessage )
{
	std::cerr << "Error: " << functionName << ": " << errorMessage << std::endl;
	exit(1);
}

void ServerReactor::setBlocking( int socketFD )
{
	int	flags;

	flags = fcntl(socketFD, F_GETFL, 0);
	if (flags == -1)
		this->writeError("fcntl", "Failed getting flags");
	flags |= O_NONBLOCK;
	if (fcntl(socketFD, F_SETFL, flags) == -1)
		this->writeError("fcntl", "Failed setting flags");
}

void	ServerReactor::run( void )
{
	int			fd, filter, nEvents;
	struct	kevent evList[32];
	
	while (_isShutdown == false)
	{
		nEvents = kevent(_kq, NULL, 0, evList, 32, NULL);
		if (nEvents == -1)
			this->writeError("kevent", "Failed to get events");
		for (int i = 0; i < nEvents; i++)
		{
			fd = evList[i].ident;
			filter = evList[i].filter;
			if (fd == _serverSocket && filter == EVFILT_READ)
				this->acceptNewClient();
			// if (fd != _serverSocket && filter == EVFILT_READ)
			// 	this->handleIncomingMessage(fd);
			// if (fd != _serverSocket && filter == EVFILT_WRITE)
			// 	this->handleOutgoingMessage(fd);
		}
	}
}

void	ServerReactor::acceptNewClient( void )
{
	int				clientSocket;
	struct		sockaddr_in clientAddress;
	struct		kevent clientEvent;
	socklen_t	clientAddressLength;

	clientAddressLength = sizeof(clientAddress);
	clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocket == -1)
		this->writeError("accept", "Failed to accept new client");
	this->setBlocking(clientSocket);
	EV_SET(&clientEvent, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL); // Adding client to kqueue
	if (kevent(_kq, &clientEvent, 1, NULL, 0, NULL) == -1)
		this->writeError("kevent", "Failed to add client to kqueue");
	_clientManager.addClient(clientSocket, ClientData(clientSocket));
	cout << "New client connected" << endl;
}

// void	ServerReactor::handleIncomingMessage()

// void	ServerReactor::handleOutgoingMessage()
