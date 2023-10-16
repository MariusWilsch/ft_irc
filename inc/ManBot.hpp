/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManBot.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 22:52:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/16 22:53:07 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANBOT_HPP
#define MANBOT_HPP

#include <iostream>

#include <ExecuteCommands.hpp>

class ManBot{
    public:
    static void man(ServerReactor &_server, Message &ProcessMessage, int clientSocket);
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