/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:18:42 by verdant           #+#    #+#             */
/*   Updated: 2023/07/07 18:52:59 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <unistd.h>
# include <fcntl.h>
# include <set>
# include <map>

class ClientData;
class ClientManager;

using std::string;
using std::set;
using std::map;
using std::cout;
using std::endl;

class	ServerReactor {
	private:
		int									_serverSocket;
		int									_kq;
		bool								_isShutdown;
		string							_connectionPassword;
		int									_maxClients;
		ClientManager				_clientManager;
		ChannelManager			_channelManager;
		// Reactor Reactor;
	public:
		ServerReactor(int port, int maxClients, string connectionPassword);
		~ServerReactor();
		int		setupServerSocket( int port );
		void	setBlocking(int fd);
		void	acceptNewClient();
		void	handleIncomingMessage(int clientSocket);
		void	handleOutgoingMessage(int clientSocket);
		void	writeError(string functionName, string errorMessage);
		void	run();
		// void	shutdown();	
};

#endif