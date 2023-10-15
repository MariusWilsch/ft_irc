/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/15 12:59:23 by mwilsch          ###   ########.fr       */
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
				if (token == ":")
						_params.push_back(token);
				if (token.empty())
						continue;
				if (token.size() != 1 && token.find(':') != string::npos) {
						_params.push_back(_rawMessage.substr(_rawMessage.find(':') + 1));
						break;
				}
				if (token.find(',') != string::npos) {
						std::istringstream iss2(token);
						string token2;
						while (std::getline(iss2, token2, ',')) {
								if (!token2.empty())
										_params.push_back(token2);
						}
						continue;
				}
				if (token != ":")
					_params.push_back(token);
		}
	
		// Print extracted data
		printData();
}

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

