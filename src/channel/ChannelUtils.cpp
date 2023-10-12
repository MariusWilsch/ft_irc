/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 20:19:29 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/12 20:41:10 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

// Repreduce a function that will take a channel data structure and the information that is needed to be send to the members.
// * This is the format of the message that is needed to be send to members when something happens inside the channel.

//* JOIN INFORMATIVE REPLY.
// :AMSKLDN!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#ChannelNadia 
//* NICK INFORMATIVE REPLY.
// :nickname!~n@5c8c-aff4-7127-3c3-1c20.230.197.ip NICK :djkfgnr

// :klfjndsfew!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :##thisIsAchannel.


//* MODE CHANGE TOPIC
// :klfjndsfew!~t@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #Ch12 +t 