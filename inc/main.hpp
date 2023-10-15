/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:07:18 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/15 12:58:05 by mwilsch          ###   ########.fr       */
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
// #include <array>
#include <netdb.h>

using std::string;
using std::map;
using std::set;
using std::vector;
// usign std::array;
using std::cout;
using std::endl;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUMERIC REPLIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

// This numeric reply needs to be send when the client connects to the server first time.
// :server 001 <nick> :Welcome to the <network> Network, <nick>[!<user>@<host>]
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ REPLIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define RPL_WELCOME(nickname) "001 "  + nickname + " :Welcome to the IRC Network, " + nickname + "\r\n"
#define RPL_NAMREPLY(nickname, channelType, channel, users) "353 " + nickname + " " + channelType + " " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + " :End of /NAMES list." + "\r\n"
#define RPL_NOTOPIC(channel) "331: " + channel + " : No topic is set\n"
#define RPL_TOPIC(channel, topic) "332: " + channel + ": "  + topic + "\n"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ERRORS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define ERR_UNKNOWNCOMMAND(nickname, wrongCommand) "421 " + nickname + " " + wrongCommand + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() "431: No nickname given\n"
#define ERR_NICKNAMEINUSE(nick) "433: " + nick + " :Nickname is already in use\n"
#define ERR_NEEDMOREPARAMS(command) "461: " + command + " : Not enough parameters\n"
#define ERR_ALREADYREGISTRED() "462: You may not reregister\n"
#define ERR_BADCHANNELKEY(channel) "475: " + channel + " : Cannot join channel (+k)\n"
#define ERR_NOTONCHANNEL(channel) "442: " + channel + " : You're not on that channel\n"
#define ERR_USERONCHANNEL(user, channel) "443: " + user + " " + channel + " : is already on channel\r\n"
#define ERR_USERNOTINCHANNEL(user, channel) "441: " + user + " " + channel + " : They aren't on that channel\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482: " + channel + " : You're not channel operator\n"
#define ERR_NOSUCHCHANNEL(channel) "403: " + channel + " : No such channel\n"
#define ERR_CHANNELISFULL(channel) "471: " + channel + " :Cannot join channel (+l)\n"
#define ERR_UNKNOWNMODE(character) "472: " + character + " : is unknown mode char to me\n"
#define ERR_INVITEONLYCHAN(channel) "473: " + channel + " :Cannot join channel (+i)\n"
#define ERR_NOTREGISTERED(nickname, command) "451 " + nickname + " " + command + " :You must finish connecting with another nickname first.\r\n"
#define ERR_PASSWDMISMATCH() "464 * :Password incorect\r\n"

#define ERR_NORECIPIENT(target) "411 " + target + " :No recipient given (PRIVMSG)\r\n"
#define ERR_NOTEXTTOSEND(target) "412 " + target + " :No text to send\r\n"
#define ERR_NOSUCHNICKCHANNEL(nickSender, nickTarget) "401 " + nickSender + " " + nickTarget + " :No such nick/channel\r\n"





// PRIVMSG Numeric replies to add here.

// #define ERR_NEEDMOREPARAMS() 