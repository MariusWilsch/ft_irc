/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:18:42 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 14:10:07 by mwilsch          ###   ########.fr       */
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
# include <unistd.h>
# include <fcntl.h>


using std::string;
using std::set;
using std::map;
using std::cout;
using std::endl;

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
		ServerReactor(int port, int maxClients, string connectionPassword);
		~ServerReactor();
		int		setupServerSocket( int port );
		void	setBlocking(int fd);
		void	acceptNewClient();
		// void	handleIncomingMessage(int clientSocket);
		// void	handleOutgoingMessage(int clientSocket);
		void	writeError(string functionName, string errorMessage);
		void	run();
		// void	shutdown();	
};

#endif