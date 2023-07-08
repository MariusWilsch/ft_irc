/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:50:23 by verdant           #+#    #+#             */
/*   Updated: 2023/07/07 17:54:07 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <set>
# include <map>

using std::string;

class ClientData {
	private:
		int			_clientSocket;
		string	_nickname;
		string	_username;
		enum		_role {USER, OPERATOR}
		// set 	_	<string> channels; // how do I want to store the client's channels?
		bool		_isAuthenticated;
	public:
		ClientData(int clientSocket, string nickname, string username);
};

class	ClientManager {
	private:
		map<int, ClientData>	_clients;
		int										_maxClients;
	public:
		void	addClient(int clientSocket, const ClientData &clientData);
		void	removeClient(int clientSocket);
		// void	authenticateClient(int clientSocket, string nickname, string username);
		// Addtional methods, i.e forward messages, etc.
};

#endif