/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:50:23 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 14:12:10 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <set>
# include <map>

using std::cout;
using std::endl;
using std::string;
using std::map;
// using std::set;

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
	public:
		ClientData(int clientSocket);
		// Getters
		int									getClientSocket() const;
};

class	ClientManager {
	private:
		map <int, ClientData>			_activeClientsBySocket;
		map <string, ClientData>	_inactiveClientsByNickname;
	public:
		ClientManager( void );
		void	addClient(int clientSocket, const ClientData &clientData);
		// void	removeClient(int clientSocket);
		// void	authenticateClient(int clientSocket, string nickname, string username);
		// Addtional methods, i.e forward messages, etc.
};

#endif