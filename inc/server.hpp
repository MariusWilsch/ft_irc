/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:04:15 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/11 15:46:25 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

/**
 * @brief Class to handle server-level operations
 * 
 */
class ServerReactor {
	private:
		int			_serverSocket;
		int			_kq;
		bool		_isShutdown;
		string	_connectionPassword;
		// ClientManager	_clientManager;
		// ChannelManager	_channelManager;
	public:
		/*			Class Default Functions			*/
		
		ServerReactor();
		ServerReactor( int port, string connectionPassword );
		~ServerReactor();
		/*			Socket & Multiplexing			*/

		int		setupServerSocket( int port );
		void	setBlocking(int fd);
		void	updateMoinitoring(int clientSocket, int filter, int flags);
		

		/*			EVENTS			*/
		void	acceptNewClient();
		void	recieveIncomingMessage(int clientSocket);
		//void	sendMessageToClient(int clientSocket, string message);
		 
		/*			SERVER T0 CLIENT COMMUICATION			*/ // TODO: Think of a better way to pass parameters. Maybe a class?
			// Or should I do this in the client manager?
			// Format // :<nick>!<user>@<host> <command> <channel> :<optionalMessage>
		void	sendAcknowledgement(int clientSocket, string nick, string user, string host, string command, string channel, string optionalMessage);
			// Format // :<server> <numericCode> <targetNick> <parameters> :<message>
		void	sendNumericReply(int clientSocket, string numericCode, string targetNick, string parameters, string message);

		/*			MAIN			*/
		void	run();
		
			/*			UTILS			*/
		void	writeServerError(string functionName, string errorMessage, int errorNumber);
};