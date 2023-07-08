/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:50:23 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 18:32:31 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <set>
# include <map>
# include <vector>

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
		string							_password;
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
		void	printActiveClients() const;
		void	addClient(int clientSocket, const ClientData &clientData);
		void	removeClient(int clientSocket);
		// void	addMessageToBuffer(int clientSocket, char buffer[]);
		// void	authenticateClient(int clientSocket, string nickname, string username);
		// Addtional methods, i.e forward messages, etc.
};

#endif