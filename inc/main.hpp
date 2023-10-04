/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:07:18 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/04 20:10:50 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// This header is used to include all the headers in the project which are used in multiple files

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <string>
#include <vector>

using std::string;
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUMERIC REPLIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// #define RPL_WELCOME(nickname, user, host)

#define RPL_NOTOPIC(channel) "331: " + channel + " : No topic is set\n"
#define RPL_TOPIC(channel, topic) "332: " + channel + ": "  + topic + "\n"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ERRORS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define ERR_NONICKNAMEGIVEN() "431: No nickname given\n"
#define ERR_NICKNAMEINUSE(nick) "433: " + nick + " :Nickname is already in use\n"
#define ERR_NEEDMOREPARAMS(command) "461: " + command + " : Not enough parameters\n"

#define ERR_ALREADYREGISTRED() "462: You may not reregister\n"

#define ERR_BADCHANNELKEY(channel) "475: " + channel + " : Cannot join channel (+k)\n"
#define ERR_NOTONCHANNEL(channel) "442: " + channel + " : You're not on that channel\n"
#define ERR_USERONCHANNEL(user, channel) "443: " + user + " " + channel + " : is already on channel\n"
#define ERR_USERNOTINCHANNEL(user, channel) "441: " + user + " " + channel + " : They aren't on that channel\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482: " + channel + " : You're not channel operator\n"
#define ERR_NOSUCHCHANNEL(channel) "403: " + channel + " : No such channel\n"
#define ERR_UNKNOWNMODE(character) "472: " + character + " : is unknown mode char to me\n"
// #define ERR_NEEDMOREPARAMS() 