/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/04 18:46:51 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

/*			CLASS DEFAULT FUNCTIONS			*/

Message::~Message( void ) {};

Message::Message( string rawMessage, map <string, CommandProperties> properties ) : _rawMessage(rawMessage), _properties(properties) {

	_isFatal = false;
	// Earse \n // TODO: I later need to figure out what to do with \r\n at the end of messages
	
	_rawMessage.erase(std::remove(_rawMessage.begin(), _rawMessage.end(), '\n'), _rawMessage.end());
	_rawMessage.erase(std::remove(_rawMessage.begin(), _rawMessage.end(), '\r'), _rawMessage.end());
	

	cout << "|" << _rawMessage << "|" << endl;

	// Parse raw message
	extractCommand( );
	extractTrailing( );
	extractParams( );

	// Print extracted data
	// printData();

	// Execute command
	// [...]
}

/*			GETTERS			*/

string Message::getCommand( void ) { 
	return _command; 
}

std::vector <string> Message::getParams( void ) {
	if (_params.empty())
		return (std::vector <string>());
	return _params;
}

bool	Message::getFatal( void ){
	return (this->_isFatal);
}

string	Message::getTrailing( void ){
	return (this->_trailing);
}

/*			SETTERS			*/

// void Message::setResponseCode( string responseCode )  { 
// 	_isFatal = true;
// 	_responseCode = responseCode; 
// }

/*			MEMBER FUNCTIONS			*/

void Message::printData( void )
{
	std::cout << "Size of params vec: " << _params.size() << endl;
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::cout << "Params: ";
	for (std::vector<string>::iterator it = _params.begin(); it != _params.end(); ++it)
	{
		std::cout << "|" << *it << "| ";
	}
	std::cout << std::endl;
	std::cout << "Trailing: " << _trailing << std::endl;
}
