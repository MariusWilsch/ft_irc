/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:04:46 by mwilsch           #+#    #+#             */
/*   Updated: 2023/08/22 13:51:23 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

#include <vector>
#include <sstream>
#include <algorithm>


typedef void (*CommandHandler)();

class ClientData;
struct CommandProperties {
	int				mandatoryParams;
	bool			ignoreTrailing;
	CommandHandler	handler;
	CommandProperties();
	CommandProperties(int m, bool i,  CommandHandler h);
};


/**
 * @brief Class to parse and store a message
 * 
 * @details This class is responsible for parsing and storing a message.
 * @param _isFatal A boolean to know if the message has a fatal error
 * @param _responseCode The response code of the message
 * @param _rawMessage The raw message
 * @param _prefix The prefix of the message
 * @param _command The command of the message
 * @param _trailing The trailing of the message
 * @param _params The command params of the message
 * @param _senderData The ClientData class of the sender of the message
 * @param _properties The properties of the command @see src/serverExtractData.cpp
 * 
 * @note The message is parsed in the constructor
 * @note The message is parsed in the following format: [':' prefix ' '] command [' ' params] [' :' trailing] @see IRC RFC 2812
 * 
 */
class Message {
	private:
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
		void	executeCommand(const std::string& command) {

		/*			GETTERS			*/
		string							getCommand( void );
		std::vector<string> 			getParams( void );
		ClientData&						getSenderData( void );

		/*			SETTERS			*/

		void							setResponseCode( string code );
};