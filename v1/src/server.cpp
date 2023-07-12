/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:10 by verdant           #+#    #+#             */
/*   Updated: 2023/07/12 11:02:51 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"

// Default constructor
ServerReactor::ServerReactor()
{
	_isShutdown = false;
	// Setup client manager
	_clientManager = ClientManager();
	// Setup channel manager
	// _channelManager = ChannelManager();
}
// Parameterized constructor
ServerReactor::ServerReactor(int port, int maxClients, string connectionPassword)
	: _serverSocket(setupServerSocket(port)), _maxClients(maxClients), _connectionPassword(connectionPassword)
{
	// Setup kqueue
	_kq = kqueue();
	if (_kq == -1)
		this->writeError("kqueue", "Failed to create kqueue");
	// Add server socket to kqueue
	struct kevent evSet;
	EV_SET(&evSet, _serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &evSet, 1, NULL, 0, NULL) == -1)
		this->writeError("kevent", "Failed to add server socket to kqueue");
}

/**
 * @brief Run the multiplexing loop
 * 
 * @param port Listening port
 * @note SOL_SOCKET indicates that the socket option is being set or retrieved at the socket level.
 */
int	ServerReactor::setupServerSocket( int port )
{
	struct sockaddr_in	serverAddress;
	int									serverSocket, yes;
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		this->writeError("socket", "Failed to create socket");
	yes = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		this->writeError("setsockopt", "Failed to set socket options");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		this->writeError("bind", "Failed to bind socket");
	if (listen(serverSocket, 20) == -1)
		this->writeError("listen", "Failed to listen on socket");
	this->setToNonBlocking(serverSocket);
	return (serverSocket);
}

ServerReactor::~ServerReactor()
{
	cout << "Server shutting down" << endl;
	close(_serverSocket);
}

void ServerReactor::setToNonBlocking( int socket )
{
	int	flags;

	flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1)
		this->writeError("fcntl", "Failed getting flags");
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) == -1)
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
			if (fd != _serverSocket && filter == EVFILT_READ)
				this->handleIncomingMessage(fd);
			// if (fd != _serverSocket && filter == EVFILT_WRITE)
			// 	this->handleOutgoingMessage(fd);
		}
	}
}

// Format being used is: :nick!user@host COMMAND #channel :optional message
// :<nick>!<user>@<host> <command> <channel> :<optionalMessage>

void	ServerReactor::sendMSG(int sockfd, const std::string& nick, const std::string& user, const std::string& host,
             const std::string& command, const std::string& channel, const std::string& optionalMessage)
{
    std::string message = ":" + nick + "!" + user + "@" + host + " " + command + " " + channel;

    // Append the optional message if it's present
    if (!optionalMessage.empty()) {
        message += " :" + optionalMessage;
    }

    message += "\r\n";

    send(sockfd, message.c_str(), message.size(), 0);
}
void ServerReactor::sendNumericReply(int sockfd, const std::string& server, const std::string& numericCode,
                      const std::string& targetNick, const std::string& parameters, const std::string& message) 
{
    std::string reply = ":" + server + " " + numericCode + " " + targetNick + " " + parameters + " :" + message + "\r\n";
    send(sockfd, reply.c_str(), reply.size(), 0);
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
	this->setToNonBlocking(clientSocket);
	EV_SET(&clientEvent, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL); // Adding client to kqueue
	if (kevent(_kq, &clientEvent, 1, NULL, 0, NULL) == -1)
		this->writeError("kevent", "Failed to add client to kqueue");

	// // Send a JOIN message
	// this->sendMSG(clientSocket, "NickName", "UserName", "host", "JOIN", "#channel", "");
	// // Send a PART message
	// this->sendMSG(clientSocket, "NickName", "UserName", "host", "PART", "#channel", "Goodbye cruel world!");
	// // Send a KICK message
	// this->sendMSG(clientSocket, "NickName", "UserName", "host", "KICK", "#channel", "Used an offensive word!");


	// Send "Nickname is already in use"
	// sendNumericReply(clientSocket, "server.name", "433", "NickName", "*", "Nickname is already in use");
	// // Send "Welcome to the Internet Relay Network"
	// sendNumericReply(clientSocket, "server.name", "001", "NickName", "*", "Welcome to the Internet Relay Network");
	// Send RPL_TOPIC
	sendNumericReply(clientSocket, "server.name", "332", "NickName", "#channel", "Topic for #channel");

	cout << "New client connected" << endl;
}

void	ServerReactor::handleIncomingMessage( int clientSocket )
{
	char		buffer[1024];
	int			nBytes;
	string	tempBuffer;
	size_t	pos;

	pos = 0;
	while (pos != string::npos)
	{
		nBytes = recv(clientSocket, &buffer, 1023, 0);
		if (nBytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			else
				this->writeError("recv", "Failed to receive message");
		}
		if (nBytes == 0) // Client disconnected
		{
			this->updateMoinitoring(clientSocket, EVFILT_READ, EV_DELETE);
			this->_clientManager.removeClient(clientSocket);
			return ;
		}
		buffer[nBytes] = '\0';
		tempBuffer.append(buffer);
		cout << "Received: " << tempBuffer << endl;
		pos = tempBuffer.find("\r\n");
		memset(buffer, 0, 1024);
	}
	this->_clientManager.processMessage(clientSocket, tempBuffer);
}
// void	ServerReactor::handleIncomingMessage(int clientSocket )
// {
// 	char		buffer[1024];
// 	int			nBytes;
// 	string	tempMessage;
// 	size_t	pos;

// 	// nBytes = 0;
// 	pos = 0;
// 	while (pos != string::npos)
// 	{
// 		nBytes = recv(clientSocket, &buffer, 1023, 0);
// 		if (nBytes == -1)
// 		{
// 			if (errno == EAGAIN || errno == EWOULDBLOCK)
// 				break ;
// 			else
// 				this->writeError("recv", "Failed to receive message");
// 		}
// 		if (nBytes == 0)
// 		{
// 			_clientManager.removeClient(clientSocket);
// 			return ;
// 		}
// 		buffer[nBytes] = '\0';
// 		tempMessage += buffer;
// 		pos = tempMessage.find("\r\n");
// 	}
// 	tempMessage.substr(0, pos); // Remove \r\n sequence
// 	cout << "Complete message: " << tempMessage << endl;
// 	memset(buffer, 0, sizeof(buffer)); // Clear buffer
// }



// void	ServerReactor::handleOutgoingMessage()
