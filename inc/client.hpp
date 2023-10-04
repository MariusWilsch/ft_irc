/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/04 16:16:18 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

/**
 * @brief Class to handle client-level operations
 * 
 * @param _clientSocket The client socket
 * @param _mode mode is set to 0 per RFC 2812
 * @param _isRegistered An array of booleans to know if the client is registered
 * @param _realname The realname of the client
 * @param _nickname The nickname of the client
 * @param _username The username of the client
 * @param _unused Set to '*' per RFC 2812
 * @param _userType The type of user (USER, SERVER_OP, CHANNEL_OP)
 * 
 */
class ClientData {
	private:
		int									_clientSocket;
		int									_mode;
		string								_realname;
		string								_nickname;
		string								_username;
		string								_unused;
		string								_password;
		bool								_registration;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientData( void );
		ClientData( int clientSocket, unsigned int clientsNumber);
		~ClientData();

		/*			MEMBER FUNCTIONS			*/

		

		/*			GETTERS			*/

		int		getClientSocket( void ) const;
		int		getMode() const;
		string	getRealname() const;
		string	getNickname() const;
		string	getUsername() const;
		string 	getUnused() const;
		bool	getRegistration() const;
		string	getPassword() const;
		


		/*			SETTERS			 */

		void	setmode(int mode);
		void	setRealname(string realname);
		void	setNickname(string nickname);
		void	setUsername(string username);
		void	setUnused(string unused);
		void	setRegistration( bool b);
		void	setPassword(string pass);

};

/**
 * @brief Class to handle client-level operations
 * 
 * @param _usedNicknames A set of used nicknames
 * @param _activeClientsBySocket A map of active clients by socket
 * 
 */
class ClientManager {
	private:
		// set <string>						_usedNicknames;
		map <int, ClientData>				_ClientsBySocket;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientManager( void );
		~ClientManager();

		/*			MEMBER FUNCTIONS			*/
		
		void								addClient( int clientSocket);
		void								removeClient( int clientSocket );

		/*			GETTERS			*/

		ClientData&							getClientData( int clientSocket );
		map<int, ClientData>&		getClientBySocket();
		
		/*			METHODS			*/
		// Repreduce a method that takes two parameters: 1 - a set of client socket 2 - a nick name,
		// search for each the client socket from the set inside the map of _ClientsBySocket  and then check if the nick name is matched
		// if the nick name founded in the given socket id which represent a member of a channel it means that, the user is a memeber.
		bool	MatchNickName(set <int> ChannelMembers, string nickname);
		void	doesClientExist(string& nickname);
};