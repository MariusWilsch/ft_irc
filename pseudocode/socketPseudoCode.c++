/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketPseudoCode.c++                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 14:15:45 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/10 11:20:44 by mwilsch          ###   ########.fr       */
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

using namespace std;

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @note 
 * @return int 
 */
int main(int argc, char **argv)
{
		if (argc != 2)
		{
			cout << "Usage: " << argv[0] << " <port>" << endl;
			return 1;
		}
		int port = atoi(argv[1]);
		
		// Create a TCP server socket
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
		// Accept incoming connections
		while (true)
		{
			int clientSocket = accept(serverSocket, NULL, NULL);
			// If there is no incoming connection, accept returns -1 and sets errno to EWOULDBLOCK or EAGAIN
			if (clientSocket == -1) {
				if (errno != EWOULDBLOCK || errno != EAGAIN) {
					perror("accept");
					return 1;
				}
				else
					continue;
			}
			else {
				cout << "New client connected" << endl;
				send(clientSocket, "Hello client!\n", 14, 0);
				close(clientSocket);
			}
		}
		close(serverSocket);
		return 0;
}