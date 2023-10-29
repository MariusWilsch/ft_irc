/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/29 15:40:51 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

/*			CLASS DEFAULT FUNCTIONS			*/

Message::~Message( void ) {};

Message::Message( string rawMessage, map <string, CommandProperties> properties ) : _properties(properties) {

	_isFatal = false;
	rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\n'), rawMessage.end());
	rawMessage.erase(std::remove(rawMessage.begin(), rawMessage.end(), '\r'), rawMessage.end());
	if (rawMessage.empty())
		return ;

	if (rawMessage[0] == ':') {
		_prefix = rawMessage.substr(1, rawMessage.find(' ', 0) - 1);
		rawMessage.erase(0, rawMessage.find(' ') + 1);
	}	
	std::istringstream iss(rawMessage);
	string token;
	if (std::getline(iss, token, ' ') && !token.empty())
		_command = token;
	std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
	if (_properties.count(_command) == 0) {
		_isFatal = true;
		return ;
	}

	while (std::getline(iss, token, ' ')) {
		if (token.empty())
			continue;
		if (token.find(':') != string::npos) {
			string trailing = rawMessage.substr(rawMessage.find(':') + 1);
			if (!trailing.empty()) {
				_params.push_back(trailing);
				break;
			}
		}
		if (token != ":")
			_params.push_back(token);
	}
	// printData();
}

/*			GETTERS			*/

string Message::getCommand( void ) const { 
	return _command; 
}

std::vector <string> Message::getParams( void ) const {
	return _params;
}

bool	Message::getFatal( void ) const {
	return (this->_isFatal);
}


/*			MEMBER FUNCTIONS			*/

// void Message::printData( void )
// {
// 	std::cout << "Size of params vec: " << _params.size() << endl;
// 	std::cout << "Prefix: " << _prefix << std::endl;
// 	std::cout << "Command: " << _command << std::endl;
// 	std::cout << "Params: ";
// 	for (std::vector<string>::iterator it = _params.begin(); it != _params.end(); ++it)
// 	{
// 		std::cout << "|" << *it << "| ";
// 	}
// 	std::cout << std::endl;
// }

