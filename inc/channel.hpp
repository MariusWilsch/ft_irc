/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:01:33 by mwilsch           #+#    #+#             */
/*   Updated: 2023/09/30 13:21:38 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

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

/*
	UPDATE:
		The members & operators of the channel needs to be stored by nick name and socket ID.
		
*/
class ChannelData {
	private:
		string			_name;
		string			_topic;
		string			_key;
		bool			_security;
		bool			_inviteOnly;
		bool			_topicSet;
		set <int> 		_clientSockets;
		set <int> 		_operators;
		set <string> 	_inviteList;

	public:
		ChannelData( void );
		~ChannelData( void );
		
		/*			GETTERS			*/
		string 			getName( void ) const;
		string 			getTopic( void ) const;
		bool			getSecurity( void ) const;
		string			getKey( void ) const;
		bool			getTopicFlag ( void ) const;
		bool			getInviteFlag ( void )  const;
		set <int> 		getClientSockets( void ) const;
		set <int> 		getOperators( void ) const;
		set <string>	getInviteList( void ) const;

		/*			SETTERS			 */
		void 			setName( string name );
		void 			setTopic( string topic );
		void			setSecurity( bool s);
		void			setKey (string key);
		void			setInviteFlag(bool stat);
		void			setTopicFlag(bool stat);
		void			addClient( int clientSocket);
		void			addOperator( int clientSocket);
		void			addGuest( string nickName);

		/* 			ClientData methods			*/
		bool			isOperator(int clientSocket);
		bool			isCLient(int clinetSocket);
		bool			isInvited(string nickName);
		void    		removeClient(set<int>::iterator cl);
		void			removeOperator(set<int>::iterator op);
};

class ChannelManager {
	private:
		map<string, ChannelData>	_channels;

	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
			ChannelManager( void );
			~ChannelManager( void );
			
		/*			GETTERS			*/
		ChannelData&				getChannelByName( string name );
		map<string, ChannelData>&	getChannels( void );

		/*			SETTERS			*/
		void	addChannel( string name, ChannelData &channelData );

		/*			METHODS			*/
		bool			channelExistence(string name);
		bool			itsChannel( string name );
};