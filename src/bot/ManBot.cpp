/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManBot.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 22:52:24 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/25 12:12:04 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "ManBot.hpp"

/* The BOT will be called /MAN
    + this bot will give the manual of all the used command in ft_irc by order of The "The user"
        + The bot command is: /MAN;
        - /MAN without parameters:
            + give the usage of the bot command
        - /MAN Parameters are:
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

string ManBot::BotManual(){
    string manual;

    manual = " * MAN SUPPORT BOT *** FT_IRC  COMMANDS: ";
    manual.append("| * PASS * NICK * USER * JOIN * PRIVMSG * KICK * INVITE * TOPIC * MODE * PART |");
    manual.append("> * 'ANY OTHER FORMAT WILL NOT BE ACCEPTABLE SO YOU NEED TO ONE FROM THE SUPPORTED COMMANDS'.");
    return (manual);
}

string ManBot::PassManual(){
    string manual;

    manual = " * PASS COMMAND MANUAL >>  SYNOPSIS: PASS [password]: ";
    manual.append(" DESCRIPTION: The PASS command is used to set a 'connection password'.  The");
    manual.append(" optional password can and MUST be set before any attempt to register");
    manual.append(" the connection is made.  Currently this requires that user send an");
    manual.append(" PASS command before sending the NICK/USER combination.");
    return (manual);
}

string ManBot::NickManual(){
    string manual = " * NCIK COMMAND MANUAL >> SYNOPSIS: NICK [nickname]: ";
    manual.append(" DESCRIPTION:  NICK command is used to give user a nickname or change the existing one.");
    return (manual);
}

string ManBot::UserManual(){
    string manual =  " * USER COMMAND MANUAL >> SYNOPSIS: USER [mode] [unused] [realname]: ";
    manual.append(" DESCRIPTION:   The USER command is used at the beginning of connection to specify");
    manual.append(" the username, hostname and realname of a new user.");
    return (manual);
}

string ManBot::JoinManual(){
    string manual = " * JOIN COMMAND MANUAL >> SYNOPSIS: JOIN [#channel, #channel, #channel, ...] [key, key, key, ...] / [ 0 ]:  ";
    manual.append(" DESCRIPTION: The JOIN command is used by a user to request to start listening to the specific channel.");
    return (manual);
}

string ManBot::PrivmsgManual(){
    string manual = " * PRIVMSG COMMAND MANUAL >> SYNOPSIS: PRIVMSG [msgtarget] :[text to be sent]: ";
    manual.append(" DESCRIPTION: PRIVMSG is used to send private messages between users, as well as to send messages to channels.");
    return (manual);
}

string ManBot::KickManual(){
    string manual = " * KICK COMMAND MANUAL >> SYNOPSIS: KICK [#channel, #channel, #channel, ...] [user, user, user, ...] [comment]: ";
    manual.append(" DESCRIPTION: The JOIN command is used by a user to request to start listening to the specific channel.");
    return (manual);
}

string ManBot::InviteManual(){
    string manual = " * INVITE COMMAND MANUAL >> SYNOPSIS: INVITE [nickname] [#channel]: ";
    manual.append(" DESCRIPTION: INVITE command is used to invite a user to a channel.");
    return (manual);
}

string ManBot::TopicManual(){
    string manual =  " * TOPIC COMMAND MANUAL >> SYNOPSIS: TOPIC [#channel] [topic]: ";
    manual.append(" DESCRIPTION: The TOPIC command is used to change or view the topic of a channel.");
    return (manual);
}

string ManBot::ModeManual(){
    string manual = " * MODE COMMAND MANUAL >> SYNOPSIS: MODE [#channel] [+/-] [ i, k, t, l, o ]";
    manual.append(" DESCRIPTION: The MODE command is provided so that users may query and change the characteristics of a channel.");
    return (manual);
}

string ManBot::PartManual(){
    string manual = " * PART COMMAND MANUAL >> SYNOPSIS: PART [#channel, #channel, #channel, ...] [Part message]: ";
    manual.append(" DESCRIPTION: The PART command causes the user sending the message to be removed");
    manual.append(" from the list of active members for all given channels listed in the parameter string.");
    return (manual);
}

string  strToupper(string str){
    for(unsigned int i = 0; i < str.size(); i++){
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = toupper(str[i]);
    }
    return (str);
}

void    ManBot::man(ServerReactor &_server, Message &ProcessMessage, int clientSocket){
    std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"  , "PART"};
    string    (*FunctionPointers[10])() = {ManBot::PassManual, ManBot::NickManual, ManBot::UserManual, ManBot::JoinManual, ManBot::PrivmsgManual, ManBot::KickManual, ManBot::InviteManual, ManBot::TopicManual, ManBot::ModeManual, ManBot::PartManual};
    string message = ":" + _server.getClientDataFast(clientSocket).getClientInfo() + " ManBot: ";
    if (ProcessMessage.getParams().size() == 0)
        _server.sendMsg(clientSocket, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", "BOT", ManBot::BotManual().c_str());
    else{
        bool    flag = false;
        for (unsigned int i = 0; i < 10; i++){
            if (strToupper(ProcessMessage.getParams()[0]).compare(commands[i].c_str()) == 0){
                string manual = FunctionPointers[i]();
                _server.sendMsg(clientSocket, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", "BOT", manual.c_str());
                flag = true;
                break;
            }
        }
        if (!flag){
            _server.sendMsg(clientSocket, _server.getClientDataFast(clientSocket).getClientInfo(), "PRIVMSG", "BOT", ManBot::BotManual().c_str());
            throw std::exception();
        }
    }
}