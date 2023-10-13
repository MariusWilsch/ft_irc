/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSetup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:00:19 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/13 13:12:07 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*
	Replace in code:
		+ The number of parameters
			+ Join: infinite params: you can join many channels at the same time, using the command join with multiple parameters.
*/

// TODO: Place this appropriately as soon as I fixed the bug

CommandProperties::CommandProperties() : mandatoryParams(0), ignoreTrailing(false) {};

CommandProperties::CommandProperties(int m, bool i) : mandatoryParams(m), ignoreTrailing(i) {};

/*			CLASS DEFAULT FUNCTIONS			*/

ServerReactor::ServerReactor( int port, std::string password ) {
	_serverName = "Ft_irc-server";
	_serverSocket = -1;
	_clientManager = ClientManager();
	_channelManager = ChannelManager();
	_isShutdown = false;
	_connectionPassword = password;
	setupServerSocket(port);
	createPropertiesMap();
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


// How to add a command to _properties, 
void	ServerReactor::createPropertiesMap( void ) {
	_properties["PASS"] = CommandProperties(1, true);
	_properties["NICK"] = CommandProperties(1, true);
	_properties["USER"] = CommandProperties(3, false);
	_properties["JOIN"] = CommandProperties(1, true); // Unlimited
	_properties["PRIVMSG"] = CommandProperties(1, false);
	_properties["OPER"] = CommandProperties(2, true);
	_properties["KICK"] = CommandProperties(1, false); // Unlimited
	_properties["INVITE"] = CommandProperties(2, true); 
	// _properties["TOPIC"] = CommandProperties(1, false);
	_properties["MODE"] = CommandProperties(2, true);
	_properties["PART"] = CommandProperties(2, true); // Unlimited

	_properties["HELP"] = CommandProperties(1, true);
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
	socklen_t						serverAddressSize;
	int					yes;

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		writeServerError("socket", "Failed to create socket", errno);
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		writeServerError("setsockopt", "Failed to set socket options", errno);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddressSize = sizeof(serverAddress);
	if (bind(_serverSocket, (struct sockaddr *)&serverAddress, serverAddressSize) == -1)
		writeServerError("bind", "Failed to bind socket", errno);

	struct hostent *he = gethostbyname("localhost");
	if (he == NULL) {
			writeServerError("gethostbyname", hstrerror(h_errno), errno);
			return;
	}
	
	char* localIP = inet_ntoa(*(struct in_addr *)he->h_addr);
	cout << "Localhost IP Address: " << localIP << endl;
		
	if (listen(_serverSocket, 10) == -1)
		writeServerError("listen", "Failed to listen on socket", errno);
	setToNonBlocking(_serverSocket);
}

void	ServerReactor::setToNonBlocking( int fd ) {
	int	flags;
	
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		writeServerError("fcntl", "Failed to get socket flags", errno);
	if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) == -1)
		writeServerError("fcntl", "Failed to set socket flags", errno);
}


ClientManager&	ServerReactor::getClientManager(){
	return (this->_clientManager);
}

ChannelManager&	ServerReactor::getChannelManager(){
	return (this->_channelManager);
}

string	ServerReactor::getServerName(){
	return (this->_serverName);
}

string					ServerReactor::getServerPassword( void ){
	return (this->_connectionPassword);
}