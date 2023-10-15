/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/15 13:39:12 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include <netdb.h>
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
	int										_clientSocket;
	int											_mode;
		string								_clientIP;
		string								_realname;
		string								_nickname;
		string								_username;
		string								_unused;
		string								_password;
		vector<bool>					_Registration;
		/*			CLASS DEFAULT FUNCTIONS			*/
	public:
		ClientData( void );
		ClientData( int clientSocket);
		~ClientData();

		/*			MEMBER FUNCTIONS			*/

		

		/*			GETTERS			*/

		int						getClientSocket( void ) const;
		int						getMode() const;
		string				getRealname() const;
		string				getNickname() const;
		string				getUsername() const;
		string 				getUnused() const;
		vector<bool>&	getRegistration();
		string				getPassword() const;
		string				getClientIP() const;
		string				getClientInfo() const;
		


		/*			SETTERS			 */

		void			setmode(int mode);
		void			setRealname(string realname);
		void			setNickname(string nickname);
		void			setUsername(string username);
		void			setUnused(string unused);
		void			setRegisteration( bool b, int index);
		void			setPassword(string pass);
		void			setClientIP(string clientIP);

		bool	isRegistered();

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
		map <int, ClientData>				_ClientsBySocket;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		ClientManager( void );
		~ClientManager();

		/*			MEMBER FUNCTIONS			*/
		
		void								addClient( int clientSocket, string clientIP);
		void								removeClient( int clientSocket );

		/*			GETTERS			*/

		ClientData&							getClientData( int clientSocket );
		map<int, ClientData>&		getClientBySocket();
		
		/*			METHODS			*/
		// Repreduce a method that takes two parameters: 1 - a set of client socket 2 - a nick name,
		// search for each the client socket from the set inside the map of _ClientsBySocket  and then check if the nick name is matched
		// if the nick name founded in the given socket id which represent a member of a channel it means that, the user is a memeber.
		int				MatchNickName(set <int> ChannelMembers, string nickname);
		int				getClientSocketByNick(string& nickname);
		string		createUserList(set<int> channelMembers);
};