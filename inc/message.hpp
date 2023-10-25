/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:04:46 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/25 12:23:39 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "server.hpp"

#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>


class		ClientData;
struct	CommandProperties;

/**
	 * @brief Class to parse and store a message
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
 */
class Message {
	private:
		bool							_isFatal;
		string							_rawMessage;
		string							_prefix;
		string							_command;
		vector<string>					_params;
		string							_trailing;
		map <string, CommandProperties>	_properties;
	public:
		/*			CLASS DEFAULT FUNCTIONS			*/
		
		Message( string rawMessage,  map <string, CommandProperties> properties );
		~Message( void );
		

		/*			GETTERS			*/
		string			getCommand( void );
		vector<string>	getParams( void );
		bool			getFatal( void );
		string			getTrailing( void );
	
		/*			SETTERS			*/

		void			setTrailing( const string& trailing );
		// void			printData( void );
};