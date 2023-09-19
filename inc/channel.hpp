/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:01:33 by mwilsch           #+#    #+#             */
/*   Updated: 2023/09/19 04:03:49 by ahammout         ###   ########.fr       */
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
 * 
 */
class ChannelData {
	private:
		string _name;
		string _topic;
		set <int> _clientSockets;
		set <int> _operators;
		
		// Add set of ejected clients.

	public:	
	ChannelData( void );
	~ChannelData( void );
	
	/*			GETTERS			*/
	string getName( void ) const;
	string getTopic( void ) const;
	set <int> getClientSockets( void ) const;
	set <int> getOperators( void ) const;

	/*			SETTERS			 */
	void setName( string name );
	void setTopic( string topic );
	void	addClient( int clientSocket);
	void	addOperator( int clientSocket);

	/* 			ClientData methods			*/
	bool		isOperator(int clientSocket);
	bool		isCLient(int clinetSocket);
};

class ChannelManager {
	private:
		map<string, ChannelData> _channels;
	public:
	/*			CLASS DEFAULT FUNCTIONS			*/
	
		ChannelManager( void );
		~ChannelManager( void );
		
	/*			GETTERS			*/
	ChannelData& getChannelByName( string name );

	/*			SETTERS			*/
	void addChannel( string name, ChannelData channelData );
	
};	