/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:04:15 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/15 13:59:25 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "message.hpp"

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


/**
 * @brief Class to handle server-level operations
 * 
 * @details This class is responsible for handling the server socket, multiplexing and events.
 * @param _serverSocket The server socket
 * @param _kq The kqueue
 * @param _isShutdown A boolean to know if the server is shutdown
 * @param _connectionPassword The password to connect to the server
 * @param _clientManager The client manager where all the clients are stored
 * @param _channelManager The channel manager where all the channels are stored
 */
class ServerReactor {
	private:
		int				_serverSocket;
		int				_kq;
		bool			_isShutdown;
		string			_connectionPassword;
		ClientManager	_clientManager;
		// ChannelManager	_channelManager;
	public:
		/*			Class Default Functions			*/
		
		ServerReactor();
		ServerReactor( int port, string connectionPassword );
		~ServerReactor();
		/*			Socket & Multiplexing			*/

		void	setupServerSocket( int port );
		void	setToNonBlocking(int fd);
		void	updateMoinitoring(int clientSocket, int filter, int flags);
		

		/*			EVENTS			*/
		void	acceptNewClient();
		void	recieveIncomingMessage( int clientSocket );
		//void	sendMessageToClient(int clientSocket, string message);
		 
		/*			SERVER T0 CLIENT COMMUICATION			*/  
		
	
		/*			COMMAND IMPLEMENTATION			*/

		/*			MAIN			*/
		void	execute(Message &message);
		void	run();
		
			/*			UTILS			*/
		void	writeServerError(string functionName, string errorMessage, int errorNumber);
};
