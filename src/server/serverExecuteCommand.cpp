/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExecuteCommand.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 14:33:23 by verdant           #+#    #+#             */
/*   Updated: 2023/07/14 17:38:31 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "message.hpp"

void	ServerReactor::execPass(Message& message)
{
	// std::cout << "Pass: " << message.getParams()[0] << std::endl;
	// std::cout << "Pass: " << _connectionPassword << std::endl;
	// _connectionPassword.compare(message.getParams()[0]) == 0 ? message.setResponseCode("001") : message.setResponseCode("464");
	// message.getSenderData().setRegisterBool(PASS, true);

	std::cout << "pass function" << std::endl;
}


#include <cctype>


void	ServerReactor::execNick(Message& message)
{
	// string nickname = message.getParams()[0];

	// // Check if nickname is valid

	// std::cout << "Len: " << nickname.length() << std::endl;

	// if (nickname.length() == 0)
	// 	return (message.setResponseCode("431"));
	// if (nickname.length() > 9)
	// 	return (message.setResponseCode("432"));
	// for (size_t i = 0; i < nickname.length(); i++)
	// {
	// 	if (!isalnum(nickname[i]))
	// 		return (message.setResponseCode("432"));
	// }
	// // Check if nickname is already in use
	// if (_clientManager.isNicknameInUse(nickname) == true)
	// 	return (message.setResponseCode("433"));
	// Check if nickname is already registered
	// Check if nickname is already registered to another user
	// Check if nickname is already

	std::cout << "nickname function" << std::endl;
}



void	ServerReactor::execute(Message& message)
{
	string cmd[3] = {"PASS", "NICK", "USER"};
	
	
	std::cout << "Mem add" << &_cmds[0] << std::endl;
	
	(this->*_cmds[0])(message);

	// TODO: HANDLE isfatal and response code
	
	// for (int i = 0; i < 2; i++)
	// {
	// 	if (message.getCommand() == cmd[i])
	// 	{
	// 		(this->*_cmds[0])(message);
	// 		// std::cout << "Command: " << message.getCommand() << std::endl;
	// 		return ;
	// 	}
	// }
}