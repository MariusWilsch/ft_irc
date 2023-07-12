/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:13:58 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/11 14:17:13 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

typedef enum e_messageType
{
    MSG,
    SERVER_LEVEL,
    CLIENT_LEVEL,
    CHANNEL_LEVEL,
    NUMERIC_REPLY,
    ERROR,

}  t_messageType;

// Maybe this class should inherit or be added to server reactor

class Message {
    private:
        t_messageType   _type;
        string          _message;
        string          _sender;
        set<string>     _recipients;
    public:
        // Member Functions to parse, format, and maybe send messages
}