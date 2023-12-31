/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:07:18 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/29 14:12:59 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <netdb.h>

using std::string;
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ REPLIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define RPL_WELCOME(nickname) "001 "  + nickname + " :Welcome to the IRC Network, " + nickname + "\r\n"
#define RPL_NAMREPLY(nickname, channelType, channel, users) "353 " + nickname + " " + channelType + " " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + " :End of /NAMES list." + "\r\n"
#define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " : No topic is set\r\n"
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_INVITING(nickSender, channel, nickReciever) "341 " + nickSender + " " + nickReciever + " " + channel + "\r\n"
#define RPL_CHANNELMODEIS(channel, modeParams) "324 " + channel + " " + modeParams + " " + "\r\n"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ERRORS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(nick, channel) "475 " + nick + " " + channel + " : Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) "461: " + nick + " " + command + " : Not enough parameters\r\n"
#define ERR_UNKNOWNCOMMAND(nickname, wrongCommand) "421 " + nickname + " " + wrongCommand + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN() "431: * No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick) "432: * " + nick + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nick) "433: " + nick + " :Nickname is already in use\r\n"
#define ERR_ALREADYREGISTRED() "462: You may not reregister\r\n"
#define ERR_USERNOTINCHANNEL(nickSender, nickTarget, channel) "441 " +  nickSender + " " + nickTarget + " " + channel + " : They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " : You're not on that channel\r\n"
#define ERR_USERONCHANNEL(user, channel) "443: " + user + " " + channel + " : is already on channel\r\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482: " + channel + " : You're not channel operator\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_UNKNOWNMODE(character) "472: " + character + " : is unknown mode char to me\r\n"
#define ERR_NOTREGISTERED(nickname, command) "451 " + nickname + " " + command + " :You must finish connecting with another nickname first.\r\n"
#define ERR_PASSWDMISMATCH() "464 * :Password incorect\r\n"
#define ERR_NORECIPIENT(target) "411 " + target + " :No recipient given (PRIVMSG)\r\n"
#define ERR_NOTEXTTOSEND(target) "412 " + target + " :No text to send\r\n"
#define ERR_NOSUCHNICKCHANNEL(nickSender, nickTarget) "401 " + nickSender + " " + nickTarget + " :No such nick/channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + nick + " " + channel + " :Cannot send to channel\r\n"
