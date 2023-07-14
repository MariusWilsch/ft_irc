/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/14 17:06:14 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

// enum userType {USER, SERVER_OP, CHANNEL_OP};

/**
 * @brief Class to handle client-level operations
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