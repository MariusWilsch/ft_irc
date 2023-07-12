/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/12 14:58:54 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

/*			CLASS DEFAULT FUNCTIONS			*/


// 1. Find token start and length

Message::Message( void ) {
	_rawMessage = "";
	_prefix = "";
	_command = "";
	_params = "";
	_trailing = "";
	_sender = "";
	_tokStart = 0;
	_tokLength = 0;
	_recipientType = CLIENT;
}


Message::Message( string rawMessage ) : _rawMessage(rawMessage) {
	
	// 1. Split rawMessage into tokens using space as delimiter
	split(' ');
	for (int i = 0; i < _tokens.size(); i++)
		std::cout << _tokens[i] << std::endl;
	// 2. Check if first token is a prefix
}

Message::~Message( void ) {};

/*			???			*/



// If there is a trailing message don't split it into tokens

void	Message::split(char delimiter)
{
	string	token;
	std::istringstream tokenStream(_rawMessage);

	while (std::getline(tokenStream, token, delimiter))
	{
		if 
		if (!token.empty())
			_tokens.push_back(token);
	}
}