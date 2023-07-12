/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:18:42 by verdant           #+#    #+#             */
/*   Updated: 2023/07/12 11:02:51 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


# include "client.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <fcntl.h>
# include <cerrno>

using std::string;
using std::set;
using std::map;
using std::cout;
using std::endl;
using std::cerr;

class	ServerReactor {
	private:
		int									_serverSocket;
		int									_maxClients;
		string							_connectionPassword;
		int									_kq;
		bool								_isShutdown;
		ClientManager				_clientManager;
		// ChannelManager			_channelManager;
	public:
		ServerReactor();
		ServerReactor(int port, int maxClients, string connectionPassword);
		~ServerReactor();
		int		setupServerSocket( int port );
		void	setToNonBlocking(int fd);
		void	acceptNewClient();
		void	handleIncomingMessage(int clientSocket);
		void	writeError(string functionName, string errorMessage);
		void	updateMoinitoring(int clientSocket, int filter, int flags);
		void	run();
		// Experimental
		// Create a extra class called Message format which I call before using sendMSG which has the following members
			// string nick; // A get_address to get nick, user, host would be smart
			// string user; // A get_address to get nick, user, host would be smart
			// string host; // A get_address to get nick, user, host would be smart
			// string command;
			// string channel;
			// string optionalMessage;
		void sendMSG(int sockfd, const std::string& nick, const std::string& user, const std::string& host,
             const std::string& command, const std::string& channel, const std::string& optionalMessage);
		void sendNumericReply(int sockfd, const std::string& server, const std::string& numericCode,
                      const std::string& targetNick, const std::string& parameters, const std::string& message);

		// void 	writeError(string functionName, string errorMessage, int errorNumber);
		// void	handleOutgoingMessage(int clientSocket);
		// void	shutdown();	
};

#endif