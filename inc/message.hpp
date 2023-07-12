/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:04:46 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/11 16:13:41 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

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
		enum RecipientType { CLIENT, CHANNEL } _recipientType; // Maybe add SERVER? for commands like OPER or AUTHENTICATE
		//ClientData* _clientRecipient;
		//ChannelData* _channelRecipient;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		Message( void );
		Message( string rawMessage );
		~Message( void );

		/*			???			*/
};