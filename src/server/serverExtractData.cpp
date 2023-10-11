/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExtractData.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 14:08:52 by verdant           #+#    #+#             */
/*   Updated: 2023/10/11 16:16:44 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"


// std::vector<std::string> getArgs(std::string commandLine) {
//     std::vector<std::string> args;
//     while (commandLine.length()) {
//         std::string nextArg;
//         // if ':' is found, take the remaining chars of the string as one argument
//         if (commandLine[0] == ':') {
//             commandLine.erase(0, 1);
//             nextArg = commandLine;
//         }
//         // else take until whitespace
//         else
//             nextArg = commandLine.substr(0, commandLine.find(" "));
//         // remove argument taken from the command line
//         commandLine.erase(0, nextArg.length());
//         // skip all spaces
//         while (commandLine[0] == ' ')
//             commandLine.erase(0, 1);
//         // skip arg if empty
//         if (nextArg.length())
//             args.push_back(nextArg);
//     }
//     return args;
// }


void	Message::extractCommand( void  )
{
	size_t				pos;
	string				token;
	std::istringstream	tokenStream(_rawMessage);
	
	// Get Prefix
	// TODO: find a better way for this (maybe throw an exception) - What's the purpose of this?
	if (_isFatal == true) 
		return; 
	if (std::getline(tokenStream, token, ' '))
	{
		if (!token.empty() && token[0] == ':')
			_prefix = token;
		else
			_command = token;
	}
	// Get Command if command is empty		
	if (_command.empty() && std::getline(tokenStream, token, ' '))
		_command = token;
		

	// Remove trailing whitespaces and convert to uppercase to match command in properties map
	token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
	std::transform(token.begin(), token.end(), token.begin(), ::toupper);
	if (_properties.count(token) == 0)
	{
		_isFatal = true;
		std::cout << "Unknown command: " << token << std::endl;
	 	// TODO: throw 421 numeric reply  :<servername> 421 <nick> <command> :Unknown command
		return ;
	}
	
	// Erase prefix && command from rawMessage
	pos = _rawMessage.find(_command);
	if (pos != std::string::npos)
		_rawMessage.erase(0, pos + _command.length());
		
	_command = token;

}

void	Message::extractTrailing( void  )
{
	size_t	pos;
	
	if (_isFatal == true)
		return;
	if (_properties[_command].ignoreTrailing)
		return ;
	pos = _rawMessage.find_last_of(':');
	if (pos != std::string::npos)
	{
		// _trailing = _rawMessage.substr(pos + 1);
		_rawMessage.erase(pos);
	}
}

void	Message::extractParams( void )
{
	std::string			token;
	std::istringstream	tokenStream(_rawMessage);
	
	if (_isFatal == true)
		return;
	while (std::getline(tokenStream, token, ' '))
	{
		// cout << "|" << token << "|" << endl;
		if (!token.empty())
			_params.push_back(token);
	}
}