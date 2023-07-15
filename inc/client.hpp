/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/15 14:04:30 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

// enum userType {USER, SERVER_OP, CHANNEL_OP};

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
		bool								_isRegistered[3];
		string								_realname;
		string								_nickname;
		string								_username;
		string								_unused;
		// userType							_userType;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientData( void );
		ClientData( int clientSocket );
		~ClientData();

		/*			MEMBER FUNCTIONS			*/

		

		/*			GETTERS			*/

		int									getClientSocket( void ) const { return _clientSocket; };

		/*			SETTERS			 */

		void								setRegisterBool( int index, bool value ) { _isRegistered[index] = value; };

		
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
		set <string>						_usedNicknames;
		map <int, ClientData>				_activeClientsBySocket;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientManager( void );
		~ClientManager();

		/*			MEMBER FUNCTIONS			*/
		
		void								addClient( int clientSocket );
		void								removeClient( int clientSocket );
		// void								checkRegisteredStatus( int clientSocket );
		bool								isNicknameInUse( string nick );

		/*			GETTERS			*/

		ClientData&				getClientData( int clientSocket );
		
};