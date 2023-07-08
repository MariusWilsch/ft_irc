/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplexingPseudoCode.c++                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:16:52 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 18:18:35 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/event.h>

using namespace std;

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @note 
 * @return int 
 */

int pError(string str)
{
	perror(str.c_str());
	return 1;
}

int	serverSocketSetup( int port )
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!serverSocket)
	{
		perror("socket");
		return 1;
	}
	// (Optional) Change socket behaviour with setsockopt to reuse the address
	int enable = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
	{
		perror("setsockopt");
		return 1;
	}
	// Populate a sockaddr_in struct with the server address details
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	// Bind the socket to the address and port
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		perror("bind");
		return 1;
	}
	// Print the server IP address
	char ipBuffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(serverAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);
	printf("Server IP address: %s\n", ipBuffer);
	// Listen for incoming connections
	if (listen(serverSocket, 20) == -1)
	{
		perror("listen");
		return 1;
	}
	// Set the server socket to non-blocking mode
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl");
		return 1;
	}
	return serverSocket;
}

/**
 * @brief
 * 
 * @param serverSocket 
 * @note 
 */
void handleNewConnection( int serverSocket, int kq )
{
	cout << "New client connection" << endl;
	// 1. Accept the connection
	
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	
	int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocket == -1)
		pError("accept");
	// 2. Set the client socket to non-blocking mode
	
	int flags = fcntl(clientSocket, F_GETFL, 0); // Ensures that that previous flags are not overwritten
	if (flags == -1)
		pError("fcntl");
	flags |= O_NONBLOCK; // Adding the O_NONBLOCK flag to previous flags
	if (fcntl(clientSocket, F_SETFL, flags) == -1)
		pError("fcntl");
	// 3. Add the client socket to the kqueue
	
	struct kevent clientEvent;
	EV_SET(&clientEvent, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	
	if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
		pError("kevent");
	printf("New client connected: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
}

/**
 * @brief handleIncomingData is mainly for reading and processing data received from a client.
 * 
 * @param fd 
 * @note It's important to note that in handleIncomingData, you are not immediately sending data. You are processing the received data and making decisions based on it.
 */
void handleIncomingData( int fd , int kq )
{
	cout << "Incoming data from the client" << endl;

	char buffer[1024];

	// 1. Read incoming data
	ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead == 0)
	{
		cout << "Client disconnected" << endl;
		// 1. Remove the client socket from the kqueue.
		struct kevent clientEvent;
		EV_SET(&clientEvent, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
			pError("kevent");
		// 2. Close the client socket.
		close(fd);
		// 3. Remove the client from any channels it is part of.
		return ;
	}
	if (bytesRead == -1)
	{
		perror("recv");
		return ;
	}
	buffer[bytesRead] = '\0';
	cout << "Received: " << buffer << endl;
	string message = buffer;
	size_t found = message.find("\r\n");
	if (found != std::string::npos) {
    std::cout << "\\r\\n found at index: " << found << '\n';
	} else {
    std::cout << "\\r\\n not found\n";
	}
	
	// 2. Parse the data to understand what it represents (e.g., a message, a command, etc.).

	// 3. Process the data accordingly. For example, if it's a message intended for a channel, store it temporarily in a data structure for later distribution to all clients in the channel.
}

/**
 * @brief handleReadyForWriting is for sending data to a client when its socket is ready to accept data.
 * 
 * @param fd 
 * @note This function is called when a client's socket is ready to have data written to it without blocking
 */
void handleOutgoingData( int fd )
{
	cout << "Outgoing data to the client" << endl;
	// 1. Check if there is any data that needs to be sent to this client (for example, messages in a channel that the client is part of).

	// 2. Write this data to the client's socket.
}

/**
 * @brief Pseudo code for multiplexing
 * 
 * @param serverSocket The socket which listens for incoming connections
 */
void multiplexing( int serverSocket )
{
	int kq = kqueue(); // Create a kqueue
	if (kq == -1)
		pError("kqueue");
	struct kevent serverEvent; // Create a kevent struct for the server socket
	EV_SET(&serverEvent, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL); // Add the server socket to the kqueue
	if (kevent(kq, &serverEvent, 1, NULL, 0, NULL) == -1) // Register the server socket
		pError("kevent");
	while (true) // Main loop
	{
		struct kevent eventList[32]; // Create a list of events
		int eventCount = kevent(kq, NULL, 0, eventList, 32, NULL); // Wait for events
		if (eventCount == -1) 
			pError("kevent");
		for (int i = 0; i < eventCount; i++)
		{
			int fd = eventList[i].ident; // Identifies the affected socket
			int filter = eventList[i].filter; // Identifies the type of event
			// EVFILTER_READ is set when a socket is ready to read
			if (fd == serverSocket && filter == EVFILT_READ)
				handleNewConnection(serverSocket, kq);
			if (fd != serverSocket && filter == EVFILT_READ)
				handleIncomingData(fd, kq);
			// EVFILTER_WRITE is set when a socket is ready to write
			if (fd != serverSocket && filter == EVFILT_WRITE)
				handleOutgoingData(fd);
		}
	}
}

int main( int argc, char **argv )
{
		if (argc != 2)
		{
			cout << "Usage: " << argv[0] << " <port>" << endl;
			return 1;
		}
		int serverSocket = serverSocketSetup(atoi(argv[1]));
		if (serverSocket == 1)
		{
			cout << "Error setting up server socket" << endl;
			return 1;
		}
		multiplexing(serverSocket);
		return 0;
}