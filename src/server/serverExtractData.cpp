/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExtractData.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 14:08:52 by verdant           #+#    #+#             */
/*   Updated: 2023/07/14 17:21:55 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

void	Message::createPropertiesMap( void ) {
	if (_isFatal == true)
		return;
	_properties["PASS"] = CommandProperties(1, true);
	_properties["NICK"] = CommandProperties(1, true);
	_properties["USER"] = CommandProperties(4, false);
	_properties["JOIN"] = CommandProperties(1, true);
	_properties["PRIVMSG"] = CommandProperties(2, false);
	_properties["OPER"] = CommandProperties(2, true);
	_properties["KICK"] = CommandProperties(2, false);
	_properties["INVITE"] = CommandProperties(2, true);
	_properties["TOPIC"] = CommandProperties(1, false);
	_properties["MODE"] = CommandProperties(2, true);
}

void	Message::extractCommand( void )
{
	size_t				pos;
	string				token;
	const char			delimiter = ' ';
	std::istringstream	tokenStream(_rawMessage);

	// Get Prefix
	if (_isFatal == true)
		return;
	if (std::getline(tokenStream, token, delimiter))
	{
		if (!token.empty() && token[0] == ':')
			_prefix = token;
		else
			_command = token;
	}
	// Get Command if command is empty
	if (_command.empty() && std::getline(tokenStream, token, delimiter))
	{
		if (!token.empty())
			_command = token;
	}
	 // TODO: throw 421 numeric reply  :<servername> 421 <nick> <command> :Unknown command
	if (_properties.count(token) == 0)
	{
		_isFatal = true;
		std::cout << "Unknown command: " << token << std::endl;
	}
	// toUpper command
	std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
	// Erase prefix && command from rawMessage
	pos = _rawMessage.find(_command);
	if (pos != std::string::npos)
		_rawMessage.erase(0, pos + _command.length());
}

void	Message::extractTrailing( void )
{
	size_t	pos;
	
	if (_isFatal == true)
		return;
	if (_properties[_command].ignoreTrailing)
		return ;
	pos = _rawMessage.find_last_of(':');
	if (pos != std::string::npos)
	{
		_trailing = _rawMessage.substr(pos + 1);
		_rawMessage.erase(pos);
	}
}

void	Message::extractParams(char delimiter)
{
	std::string			token;
	std::istringstream	tokenStream(_rawMessage);
	
	if (_isFatal == true)
		return;
	while (std::getline(tokenStream, token, delimiter))
	{
		if (!token.empty())
			_params.push_back(token);
	}
}