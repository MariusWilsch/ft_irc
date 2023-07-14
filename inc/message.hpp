/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:04:46 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/14 17:12:06 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

#include <vector>
#include <sstream>

enum Registering {
	PASS,
	NICK,
	USER,
	REGISTERED
};

class ClientData;
struct CommandProperties {
	int		mandatoryParams;
	bool	ignoreTrailing;
	CommandProperties();
	CommandProperties(int m, bool i);
};


		// enum RecipientType { CLIENT, CHANNEL }
		// 	 _recipientType; // Maybe add SERVER? for commands like OPER or AUTHENTICATE
		//ClientData* _clientRecipient;
		//ChannelData* _channelRecipient;

/**
 * @brief 
 * 
 */
class Message {
	private:
		// int								_senderSocket;
		bool							_isFatal;
		string							_responseCode; // Maybe make this an int?
		string							_rawMessage;
		string							_prefix;
		string							_command;
		string							_trailing;
		std::vector<string>				_params;
		ClientData&						_senderData;
		map <string, CommandProperties> _properties;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		// Message( void );
		Message( string rawMessage, ClientData& senderData );
		~Message( void );
		
		/*			EXTRACT			*/
	
		void							createPropertiesMap( void );
		void							extractCommand( void );
		void							extractTrailing( void );
		void							extractParams(char delimiter);

		/*			???			*/

		void	printData( void );

		/*			GETTERS			*/
		string							getCommand( void );
		std::vector<string> 			getParams( void );
		ClientData&						getSenderData( void );

		/*			SETTERS			*/

		void							setResponseCode( string code );
};