/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HelpBot.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 12:38:51 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/07 16:46:01 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPBOT_HPP
#define HELPBOT_HPP

#include <iostream>
#include <ExecuteCommands.hpp>

class HelpBot{
    public:
    static void Help(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket);
    static string BotManual();
    static string PassManual();
    static string NickManual();
    static string UserManual();
    static string JoinManual();
    static string PrivmsgManual();
    static string KickManual();
    static string InviteManual();
    static string TopicManual();
    static string ModeManual();
    static string PartManual();
};

#endif