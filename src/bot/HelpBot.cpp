/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HelpBot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 18:42:38 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/08 17:06:38 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HelpBot.hpp"

/* The BOT will be called /HELP
    + this bot will give the manual of all the used command in ft_irc by order of The peaky blinder "The user"!
        + The bot command is: /HELP;
        - /HELP without parameters:
            + give the usage of the bot command
        - /HELP Parameters are:
            + PASS
            + NICK
            + USER
            + JOIN
            + PRIVMSG
            + KICK
            + INVITE
            + TOPIC
            + MODE
            + PART
*/

string HelpBot::BotManual(){
    string manual;

    manual = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/ HELP SUPPORT BOT /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n* introduction:";
    manual.append(" This BOT will give you the manual of all the used commands inside this server\n  To use this BOT follow those rules\n");
    manual.append("     +The main command for this bot is /HELP\n");
    manual.append("     +The command can take one of tose parameters\n");
    manual.append("         * PASS\n         * NICK\n         * USER\n         * JOIN\n         * PRIVMSG\n         * KICK\n         * INVITE\n         * TOPIC\n         * MODE  \n         * PART\n");
    manual.append(" * 'ANY OTHER FORMAT WILL NOT BE ACCEPTABLE SO YOU NEED TO RESPECT THE ABOVE RULES'.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::PassManual(){
    string manual;

    manual = " HELP SUPPORT BOT:     PASS COMMAND MANUAL\n\n";
    manual.append("     NAME:    PASS\n\n     SYNOPSIS: PASS [password]\n\n");
    manual.append("     DESCRIPTION: The PASS command is used to set a 'connection password'.  The\n");
    manual.append("     optional password can and MUST be set before any attempt to register\n");
    manual.append("     the connection is made.  Currently this requires that user send an\n");
    manual.append("     PASS command before sending the NICK/USER combination.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::NickManual(){
    string manual = " HELP SUPPORT BOT:     NCIK COMMAND MANUAL\n\n";
    manual.append("     NAME:    NICK\n\n     SYNOPSIS: NICK [nickname]\n\n");
    manual.append("     DESCRIPTION:  NICK command is used to give user a nickname or change the existing one.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::UserManual(){
    string manual =  " HELP SUPPORT BOT:     USER COMMAND MANUAL\n\n";
    manual.append("     NAME:    USER\n\n     SYNOPSIS: USER [mode] [unused] [realname]\n\n");
    manual.append("     DESCRIPTION:   The USER command is used at the beginning of connection to specify\n");
    manual.append("     the username, hostname and realname of a new user.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::JoinManual(){
    string manual = " HELP SUPPORT BOT:     JOIN COMMAND MANUAL\n\n";
    manual.append("     NAME:    JOIN\n\n     SYNOPSIS: JOIN [#channel, #channel, #channel, ...] [key, key, key, ...] / [ 0 ]\n\n");
    manual.append("     DESCRIPTION: The JOIN command is used by a user to request to start listening to the specific channel.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::PrivmsgManual(){
    string manual = " HELP SUPPORT BOT:     PRIVMSG COMMAND MANUAL\n\n";
    manual.append("     NAME:    PRIVMSG\n\n     SYNOPSIS: PRIVMSG [msgtarget] :[text to be sent]\n\n");
    manual.append("     DESCRIPTION: PRIVMSG is used to send private messages between users, as well as to send messages to channels.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::KickManual(){
    string manual = " HELP SUPPORT BOT:     KICK COMMAND MANUAL\n\n";
    manual.append("     NAME:    KICK\n\n     SYNOPSIS: KICK [#channel, #channel, #channel, ...] [user, user, user, ...] [comment]\n\n");
    manual.append("     DESCRIPTION: The JOIN command is used by a user to request to start listening to the specific channel.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::InviteManual(){
    string manual = " HELP SUPPORT BOT:     INVITE COMMAND MANUAL\n\n";
    manual.append("     NAME:    INVITE\n\n     SYNOPSIS: INVITE [nickname] [#channel]\n\n");
    manual.append("     DESCRIPTION: INVITE command is used to invite a user to a channel.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::TopicManual(){
    string manual =  " HELP SUPPORT BOT:     TOPIC COMMAND MANUAL\n\n";
    manual.append("     NAME:    TOPIC\n\n     SYNOPSIS: TOPIC [#channel] [topic]\n\n");
    manual.append("     DESCRIPTION: The TOPIC command is used to change or view the topic of a channel.\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::ModeManual(){
    string manual = " HELP SUPPORT BOT:     MODE COMMAND MANUAL\n\n";
    manual.append("     NAME:    MODE\n\n     SYNOPSIS: MODE [#channel] [+/-] [ i, k, t, l, o ]\n\n");
    manual.append("     DESCRIPTION: The MODE command is provided so that users may query and change the characteristics of a channel.\n\n");
    manual.append("     The following options are available:\n\n");
    manual.append("     [ + | - ] i           Set/remove the channel's invite only mode.\n\n");
    manual.append("     [ + | - ] k           Set/remove the channel's Key [Password].\n\n");
    manual.append("     [ + | - ] l           Set/remove the channel's members limit.\n\n");
    manual.append("     [ + | - ] t           Set/remove the channel's topic.\n\n");
    manual.append("     [ + | - ] o           Give/take operator privilege to/from a client.\n\n");
    manual.append("\nEND\n");
    return (manual);
}

string HelpBot::PartManual(){
    string manual = " HELP SUPPORT BOT:     PART COMMAND MANUAL\n\n";
    manual.append("     NAME:    PART\n\n     SYNOPSIS: PART [#channel, #channel, #channel, ...] [Part message]\n\n");
    manual.append("     DESCRIPTION: The PART command causes the user sending the message to be removed\n");
    manual.append("     from the list of active members for all given channels listed in the\n");
    manual.append("     parameter string.\n");
    manual.append("\nEND\n");
    return (manual);
}

string  strToupper(string str){
    for(unsigned int i = 0; i < str.size(); i++){
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = toupper(str[i]);
    }
    return (str);
}

void    HelpBot::Help(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"  , "PART"};
    string    (*FunctionPointers[10])() = {HelpBot::PassManual, HelpBot::NickManual, HelpBot::UserManual, HelpBot::JoinManual, HelpBot::PrivmsgManual, HelpBot::KickManual, HelpBot::InviteManual, HelpBot::TopicManual, HelpBot::ModeManual, HelpBot::PartManual};

    if (ProcessMessage.getParams().size() == 0)
        // USE  privmsg to send the manual to the client.
        send(clientSocket,  HelpBot::BotManual().c_str(),  HelpBot::BotManual().size(), 0);
    else{
        bool    flag = false;
        for (unsigned int i = 0; i < 10; i++){
            if (strToupper(ProcessMessage.getParams()[0]).compare(commands[i].c_str()) == 0)
            {
                string manual = FunctionPointers[i]();
                    // USE  privmsg to send the manual to the client.
                send (clientSocket, manual.c_str(), manual.size(), 0);
                flag = true;
                break;
            }
        }
        if (!flag){
            string err = "╳╳╳ You have a bad input: you can follow this manual for more informations!\n";
            err.append(HelpBot::BotManual().c_str());
            // Send it using privmsg;
            send(clientSocket, err.c_str(), err.size(), 0);
                throw std::exception();
        }
    }
}