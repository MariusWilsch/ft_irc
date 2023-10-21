/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:59:41 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/15 02:52:36 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*
	ALL IS GOOOOOD MAN!!
	1 - NEED TO TEST EVERYTHING IN THE MANDATORY PART FOR NOW!
	2 - TEST THE FILE TRANSFER AND THE BOT (SEND THE MANUAL TO THE USER).
	3 - READY TO EVALUATION.!!!
*/


// ! Fix privmsg issue:
/*
	privmsg #c : Hello! Helloo! i'm sending a message here! 
	412 Archer :No text to send
*/

// FIXME:
// Issue list
	// authentification
		// 1. Pass | Good!
		// 2. Nick | Good!
		// 3. User | Good!
	// Channel
		// 1. Join |
			// 1.0 Works with wrong Input | Good!
			// 1.0.1 If the channel already exits and the user sends sth like join #ch1 bullshitValue then it throws
				// Error(475):  Cannot join channel (+k) even though it should just send it | Fixed
			// 1.1 ERR_NEEDMOREPARAMS | Good!
			// 1.2 ERR_BANNEDFROMCHAN | Not required!
			// 1.3 In ERR_INVITEONLYCHAN | The channel is not present in the message (Fixed)
			// 1.4 ERR_BADCHANNELKEY | The channel is not present in the message (Fixed)
			// 1.5 ERR_CHANNELISFULL | The channel is not present in the message (Fixed)
			// 1.6 ERR_BADCHANMASK | Not required!
			// 1.7 ERR_NOSUCHCHANNEL | The channel is not present in the message (Wired bc we just create it, lol)
			// 1.8 ERR_TOOMANYCHANNELS | Not required!
			// 1.9 ERR_TOOMANYTARGETS | Not required!
			// 1.10 ERR_UNAVAILRESOURCE | Not required!
			// 1.11 RPL_TOPIC | Doesn't send the full topic or the wrong param (Fixed)
			// 1.12 RPL_NAMES | Good!
		// 2. Part | ...
			// 1.0 Succesfull cases:
				// 1.0.1 Works with bullshit value after channel name | GOOD! 
				// 1.0.2 Works with multiple channels | GOOD!
				// 1.0.3 Part trailing message is not implemented // TODO 
			// 1.1 ERR_NEEDMOREPARAMS | Doesn't throw numeric reply | In Parser we have only : (Fixed)
			// 1.2 ERR_NOSUCHCHANNEL | The channel is not present in the message (FIXED)
			// 1.3 ERR_NOTONCHANNEL |  The channel is not present in the message (FIXED)
		// 3. Topic | ...
			// 1.0 with this example input "topic #ch1 add some words here" it only adds "add" and forgets about the rest
		// 4. Invite | ..
			// 1.0 If more than 3 necessary inputs then throws 403 | example Client Input: invite nickname #ch1 dafdsafdsaf 
			// Numeric Replies
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_NOSUCHNICK | Good!
				// 1.2 ERR_NOTONCHANNEL | Good!
				// 1.3 ERR_USERONCHANNEL | Good!
				// 1.4 ERR_CHANOPRIVSNEEDED | Kind of good but in RFC2812 it says this only takes in effect when the channel is set into invite-only mode
				// 1.5 RPL_INVITING | Good!
				// 1.6 RPL_AWAY | Not required!
		// 5. Kick | ...
			// Succesfull cases:
				// 1.0 Works with bullshit value after channel name | Not Good! 
				// 1.1 Works with multiple channels | GOOD!
				// 1.2 Kick trailing message | GOOD!
				// 1.3 Send to all the channel members the kick message | GOOD!
			// Numeric Replies
				// 1.0 ERR_NEEDMOREPARAMS | Works with missing channel but with input like kick #ch1 it throws 401 instead of 461 // Does not work with both missing
				// 1.1 ERR_NOSUCHCHANNEL | Good!
				// 1.2 ERR_BADCHANMASK | Not required!
				// 1.3 ERR_CHANOPRIVSNEEDED | Good!
				// 1.4 ERR_USERNOTINCHANNEL | Throws 401 instead of 441
				// 1.5 ERR_NOTONCHANNEL | Throws 401 instead of 442
		// 6. Mode | Not tested yet

 // Other problems
	// In Mode when I write some bullshit after the correct parameters it's also sending that to the Client

int	main( int argc, char **argv )
{
	if (argc != 3)
	{
		std::cout << "Usage: ./server <port> <password>" << std::endl;
		return (1);
	}
	ServerReactor server(atoi(argv[1]), argv[2]);
	server.run();
}