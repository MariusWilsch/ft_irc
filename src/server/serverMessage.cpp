/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/14 16:34:22 by mwilsch          ###   ########.fr       */
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
		
		if (_rawMessage.empty())
			return ;

		cout << "|" << _rawMessage << "|" << endl;


		if (_rawMessage.at(0) == ':') {
				_prefix = _rawMessage.substr(1, _rawMessage.find(' ', 0) - 1);
				_rawMessage.erase(0, _rawMessage.find(' ') + 1);
		}
		std::istringstream iss(_rawMessage);
		string token;
		if (std::getline(iss, token, ' ') && !token.empty())
				_command = token;
		std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
		if (_properties.count(_command) == 0) {
				_isFatal = true; // send numeric reply
				return ;
		}
		while (std::getline(iss, token, ' ')) {
				if (token.find(':') != string::npos) {
					_params.push_back(_rawMessage.substr(_rawMessage.find(':') + 1, _rawMessage.length()));
					break ;
				}
				if (token.find(',')) {
						// cout << "Token1" << token << endl;
						std::istringstream iss2(token);
						string token2;
						while (std::getline(iss2, token2, ',')) {
								if (!token2.empty())
										_params.push_back(token2);
						}
						continue ;
				}
				if (!token.empty())
						_params.push_back(token);
		}
		// Print extracted data
		// printData();

}
// Message::Message( string rawMessage, map <string, CommandProperties> properties ) : _rawMessage(rawMessage), _properties(properties) {

// 		_isFatal = false;
// 		// Earse \n // TODO: I later need to figure out what to do with \r\n at the end of messages
		
// 		_rawMessage.erase(std::remove(_rawMessage.begin(), _rawMessage.end(), '\n'), _rawMessage.end());
// 		_rawMessage.erase(std::remove(_rawMessage.begin(), _rawMessage.end(), '\r'), _rawMessage.end());
		

// 		cout << "|" << _rawMessage << "|" << endl;

// 		// Parse raw message
// 				// extractCommand( ); Refactoring
// 				// extractTrailing( );Refactoring
// 				// extractParams( );Refactoring

// 		// Get Prefix if exists
// 		if (_rawMessage.at(0) == ':') {
// 				_prefix = _rawMessage.substr(1, _rawMessage.find(' ', 0) - 1);
// 				_rawMessage.erase(0, _rawMessage.find(' ') + 1);
// 		}
// 		// Get Trailing if exists
// 		// if (_rawMessage.find(':') != string::npos) {
// 		// 		_trailing = _rawMessage.substr(_rawMessage.find(':') + 1, _rawMessage.length());
// 		// 		_rawMessage.erase(_rawMessage.find(':'), _rawMessage.length());
// 		// }
// 		// Get Command
// 		std::istringstream iss(_rawMessage);
// 		string token;
// 		if (std::getline(iss, token, ' ') && !token.empty())
// 				_command = token;
// 		// Convert _command to uppercase
// 		std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
// 		if (_properties.count(_command) == 0) {
// 				_isFatal = true; // send numeric reply
// 				_responseCode = "421";
// 				return ;
// 		}
// 		while (std::getline(iss, token, ' ')) {
// 				if (token.find(',')) {
// 						std::istringstream iss2(token);
// 						string token2;
// 						while (std::getline(iss2, token2, ',')) {
// 								if (!token2.empty())
// 										_params.push_back(token2);
// 						}
// 						continue ;
// 				}
// 				if (!token.empty())
// 						_params.push_back(token);
// 		}
// 		// if (!_properties[_command].ignoreTrailing && _trailing.empty()) {
// 		// 	_trailing = _params[_properties[_command].mandatoryParams];
// 		// }

		
// 		// if (_command != "JOIN")
// 		// 	_params.resize(_properties[_command].mandatoryParams);
		
// 		// Print extracted data
// 		printData();

// }

/*			GETTERS			*/

string Message::getCommand( void ) { 
	return _command; 
}

std::vector <string> Message::getParams( void ) {
	if (_params.empty())
		return (std::vector <string> (1, ""));
	return _params;
}

bool	Message::getFatal( void ){
	return (this->_isFatal);
}

// string	Message::getTrailing( void ){
// 	return (this->_trailing);
// }

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
	// std::cout << "Trailing: " << _trailing << std::endl;
}

