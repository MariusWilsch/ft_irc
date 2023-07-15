/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:01:33 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/15 14:05:02 by mwilsch          ###   ########.fr       */
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
	public:
	/*			CLASS DEFAULT FUNCTIONS			*/
	
		ChannelData( void );
		~ChannelData( void );
	
	/*			GETTERS			*/


	/*			SETTERS			 */


};

/**
 * @brief 
 * 
 * @param _channels The map of channels
 */
class ChannelManager {
	private:
		map<string, ChannelData> _channels;
	public:
	/*			CLASS DEFAULT FUNCTIONS			*/
	
		ChannelManager( void );
		~ChannelManager( void );
		
	/*			???			*/
	
};	