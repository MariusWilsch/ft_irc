/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:50:23 by verdant           #+#    #+#             */
/*   Updated: 2023/07/10 11:35:20 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <set>
# include <map>
# include <unistd.h>


using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::set;

class ClientData {
	private:
		int									_clientSocket;
		string							_nickname;
		string							_username;
		enum								Role {USER, OPERATOR};
		Role								_role;
		// set 	_	<string> channels; // how do I want to store the client's channels?
		bool								_isAuthenticated;
		// string							_messageBuffer;
	public:
		ClientData( void );
		ClientData(int clientSocket);
		// Getters
		int									getClientSocket() const;
		string							getMessageBuffer() const;
		// Setters
		// void								setClientSocket(int clientSocket);
		// void								setMessageBuffer(string messageBuffer);
};

class	ClientManager {
	private:
		set <string>							_usedNicknames;
		map <int, ClientData>			_activeClientsBySocket;
		map <string, ClientData>	_inactiveClientsByNickname;
	public:
		ClientManager( void );
		void	addClient(int clientSocket, const ClientData &clientData);
		void	removeClient(int clientSocket);
		void	processMessage(int clientSocket, string message);
		// void	printActiveClients() const;
		// void	addMessageToBuffer(int clientSocket, char buffer[]);
		// void	authenticateClient(int clientSocket, string nickname, string username);
		// Addtional methods, i.e forward messages, etc.
};

#endif

// socket;

// send(socket, ":sender_nickname PRIVMSG #channel :message\r\n", 79, 0);