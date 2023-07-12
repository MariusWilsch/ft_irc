/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:04:46 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/12 14:01:28 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

#include <vector>
#include <sstream>

/**
 * @brief 
 * 
 */
class Message {
	private:
		string	_rawMessage;
		string	_prefix;
		string	_command;
		string	_params;
		string	_trailing;
		string	_sender;
		int			_tokStart;
		int			_tokLength;
		std::vector<string> _tokens;
		//int		_senderSocket;
		//enum tokenType {PREFIX, COMMAND, PARAMS, TRAILING};
		enum RecipientType { CLIENT, CHANNEL }
			 _recipientType; // Maybe add SERVER? for commands like OPER or AUTHENTICATE
		//ClientData* _clientRecipient;
		//ChannelData* _channelRecipient;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		Message( void );
		Message( string rawMessage );
		~Message( void );
		/*			???			*/
	
		void split(char delimiter);
		//int	getTokenLength( char identifier )
};