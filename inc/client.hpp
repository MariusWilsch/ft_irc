/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/11 16:01:14 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

enum userType {USER, SERVER_OP, CHANNEL_OP};

/**
 * @brief Class to handle client-level operations
 * 
 */
class ClientData {
	private:
		int									_clientSocket;
		int									_mode;
		string							_realname;
		string							_nickname;
		string							_username;
		userType						_userType;
		const string				_unused;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientData( void );
		ClientData( int clientSocket );
		~ClientData();

		/*			GETTERS			*/


		/*			SETTERS			 */

		
};

/**
 * @brief Class to handle client-level operations
 * 
 */
class ClientManager {
	private:
		set <string>							_usedNicknames;
		map <int, ClientData>			_activeClientsBySocket;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientManager( void );
		~ClientManager();

		/*			???			*/
		
};