/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:30:13 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/17 15:49:12 by ahammout         ###   ########.fr       */
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
		int				_clientSocket;
		int				_mode;
		string			_clientIP;
		string			_realname;
		string			_nickname;
		string			_username;
		string			_unused;
		string			_password;
		vector<bool>	_Registration;

	public:
		ClientData( void );
		ClientData( int clientSocket);
		~ClientData();

		/*			GETTERS			*/
		int				getClientSocket( void ) const;
		int				getMode() const;
		string			getRealname() const;
		string			getNickname() const;
		string			getUsername() const;
		string 			getUnused() const;
		string			getPassword() const;
		string			getClientIP() const;
		string			getClientInfo() const;
		vector<bool>&	getRegistration();

		/*			SETTERS			 */
		void			setmode(int mode);
		void			setRealname(string realname);
		void			setNickname(string nickname);
		void			setUsername(string username);
		void			setUnused(string unused);
		void			setRegisteration( bool b, int index);
		void			setPassword(string pass);
		void			setClientIP(string clientIP);

		/*			MEMBER FUNCTIONS			*/
		bool			isRegistered();

};

/**
 * @brief Class to handle client-level operations
 * 
 * @param _usedNicknames A set of used nicknames
 * @param _activeClientsBySocket A map of active clients by socket
 */
class ClientManager {
	private:
		map <int, ClientData>				_ClientsBySocket;
	
	public:		
		ClientManager( void );
		~ClientManager();

		/*				GETTERS					*/
		map<int, ClientData>&	getClientBySocket();
		ClientData&				getClientData( int clientSocket );
		
		/*			MEMBER FUNCTIONS			*/	
		void		addClient( int clientSocket, string clientIP);
		void		removeClient( int clientSocket );
		int			MatchNickName(set <int> ChannelMembers, string nickname);
		int			getClientSocketByNick(string& nickname);
		string		createUserList(set<int> channelMembers);
};