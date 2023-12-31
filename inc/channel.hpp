/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:01:33 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/25 16:52:00 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

class ServerReactor;

/**
 * @brief 
 * 
 * @param _name The name of the channel
 * @param _topic The topic of the channel
 * @param _clientSockets The set of client sockets in the channel
 * @param _operators The set of operators in the channel
 * @param _key this attribute is used for private channels
 * @param _security this  boolean is used to indicate if the channel is private or public.
 * 
 */

class ChannelData {
	private:
		bool			_securityFlag;
		bool			_inviteOnlyFlag;
		bool			_topicFlag;
		bool			_limitFlag;
		bool			_topicRestriction;
	
		string			_name;
		string			_topic;
		string			_key;
		size_t			_limit;
		set <int> 		_clientSockets;
		set <int> 		_operators;
		set <string> 	_inviteList;

	public:
		ChannelData( void );
		ChannelData( const string& channelName, const int socketFD );
		~ChannelData( void );
		
		/*			GETTERS			*/
		bool			getSecurity( void ) const;
		bool			getTopicFlag ( void ) const;
		bool			getInviteFlag ( void )  const;
		bool			getLimitFlag ( void ) const;
		bool    		getTopicRestriction( void ) const;
		string 			getName( void ) const;
		string 			getTopic( void ) const;
		string			getKey( void ) const;
		size_t			getLimit( void ) const;
		set <int> 		getClientSockets( void ) const;
		set <int> 		getOperators( void ) const;
		set <string>	getInviteList( void ) const;

		/*			SETTERS			 */
		void 			setName( string name );
		void 			setTopic( string topic );
		void			setKey (string key);
		void			setLimit(size_t limit);
		void			setSecurity( bool stat);
		void			setInviteFlag( bool stat );
		void			setTopicFlag( bool stat );
		void			setLimitFlag( bool stat );
		void			setTopicRestriction(bool stat);
		void			addClient( int clientSocket);
		void			addOperator( int clientSocket);
		void			addGuest( string nickName);

		/* 			ClientData methods			*/
		bool			isOperator(int clientSocket);
		bool			isCLient(int clinetSocket);
		bool			isInvited(string nickName);
		void			removeClient(int SocketID);
		void			removeOperator(int SocketID);

};

class ChannelManager {
	private:
		map<string, ChannelData>	_channels;

	public:
		ChannelManager( void );
		~ChannelManager( void );

		/*			GETTERS			*/
		map<string, ChannelData>&	getChannels( void );
		ChannelData&							getChannelByName( string name );

		/*			METHODS			*/
		void		addChannel( string name, ChannelData &channelData );
		void		removeChannel(string channelName);
		bool		channelExistence(string name);
		bool		itsChannel( string name );
		void		removeFromChannels(int _clientSocket);
		void    	removeGarbageChannels();
		string		createUserList(string channelName, ServerReactor &serverReactor);
};