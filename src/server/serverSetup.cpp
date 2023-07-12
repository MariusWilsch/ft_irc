/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSetup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:00:19 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/12 11:10:20 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*			CLASS DEFAULT FUNCTIONS			*/

ServerReactor::ServerReactor( void ) {
	_isShutdown = false;
	_serverSocket = -1;
	_clientManager = ClientManager();
	//_channelManager = ChannelManager();
}

ServerReactor::ServerReactor( int port, std::string password ) {
	_connectionPassword = password;
	setupServerSocket(port);
	
	if (_serverSocket == -1)
		writeServerError("socket", "Failed to create socket", errno);
	_kq = kqueue();
	if (_kq == -1)
		writeServerError("kqueue", "Failed to create kqueue", errno);
	updateMoinitoring(_serverSocket, EVFILT_READ, EV_ADD);
}

ServerReactor::~ServerReactor( void ) {
	std::cout << "Server shutting down..." << std::endl;
	close(_serverSocket);
}

// TODO: Put this into a utils file
void	ServerReactor::writeServerError( std::string function, std::string message, int error ) {
	std::cout << "Error: " << function << " - " << message << std::endl;
	std::cout << "Error: " << function << " - " << strerror(error) << std::endl;
	exit(1);
}

/*			SOCKET & MULTIPLEXING			*/


void	ServerReactor::updateMoinitoring( int fd, int filter, int flags ) {
	struct kevent	evSet;
	
	EV_SET(&evSet, fd, filter, flags, 0, 0, NULL);
	if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1)
		writeServerError("kevent", "Failed to update monitoring", errno);
}


void	ServerReactor::setupServerSocket( int port ) {
	struct sockaddr_in	serverAddress;
	int									yes;
	
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		writeServerError("socket", "Failed to create socket", errno);
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		writeServerError("setsockopt", "Failed to set socket options", errno);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(_serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		writeServerError("bind", "Failed to bind socket", errno);
	if (listen(_serverSocket, 10) == -1)
		writeServerError("listen", "Failed to listen on socket", errno);
	setToNonBlocking(_serverSocket);
}

void	ServerReactor::setToNonBlocking( int fd ) {
	int flags;
	
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		writeServerError("fcntl", "Failed to get socket flags", errno);
	if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) == -1)
		writeServerError("fcntl", "Failed to set socket flags", errno);
}





