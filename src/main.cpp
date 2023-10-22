/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:59:41 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/22 01:36:51 by ahammout         ###   ########.fr       */
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

// ! The user list creation is not working correctly if a user left the channel and tries to join multiple channels again
// TODO if user1 joins "join #ch1,#ch2" and then parts from #ch1 and then tries to join again #ch1,#ch2 then it doesn't work

	// ! If the channel creator leaves he lives his creator status which needs to represented in the user list creation (FIXED)

// * Fix the trailing message issue in part issue with multiple channels
// * Check all the mode command successful cases and numeric replies from RFC2812
// * Check all the PRIVMSG command successful cases and numeric replies from RFC2812
// * Go over all subjects commands successfull cases and numeric replies/server replies from RFC2812 again
// * Eval


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
				// 1.0 Works with bullshit value after channel name | Good! 
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




// * Testing all Commands again!

// TODO 
	// Numeric Replies
		// 461 is missing the nickname in the message or * if nickname is not known //! :YourServerName 461 <YourNick> PASS :Not enough parameters
		// 462 is missing the nickname in the message or * if nickname is not known //! :YourServerName 461 <YourNick> PASS :Not enough parameters
	// Inform Members in MODE (Possibly others as well)
			// For example when one writes // mode #ch1 +i hallo it sends the "hallo" to the channel members as well // !Not Good!

	//* Authentication

	// PASS
		// NUMERIC REPLIES
			// 1.0 ERR_NEEDMOREPARAMS | Good! //! (Kind of missing nickname in the message)
			// 1.1 ERR_ALREADYREGISTRED | Good! //! (Kind of missing nickname in the message)
		// Correct Input
			// 1.0 PASS <password> | Good!
			// 1.1 PASS <password> bullshitValue | Good!

	// NICK
		// NUMERIC REPLIES
			// 1.0 ERR_NONICKNAMEGIVEN | Good!
			// 1.1 ERR_ERRONEUSNICKNAME | Not Good! //TODO (Works with non alphanumerical characters)
			// 1.2 ERR_NICKNAMEINUSE | Not Good! //TODO It RPL_WELCOME (001) again && I think server confirmation message is wrong
		// Correct Input
			// 1.0 NICK <nickname> | Not Good | It's not sending this in limechat: "You are now known as changedNick"
				//* Our: hallo!~user@127.0.0.1 NICK :hallo // Expected: :hallo!~user@127.0.0.1 NICK hallo
			// 1.1 NICK <nickname> bullshitValue | Good!

	// USER
		// NUMERIC REPLIES
			// 1.0 ERR_NEEDMOREPARAMS | Good! //! (Kind of missing nickname in the message)
			// 1.1 ERR_ALREADYREGISTRED | Good!

	// * Channel commands

		// JOIN
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS  Good! //* In other servers it even join without a channel name as well
				// 1.1 ERR_BANNEDFROMCHAN | Not required!
				// 1.2 ERR_INVITEONLYCHAN | Good!
				// 1.3 ERR_BADCHANNELKEY | Good! //? The input: "mode #ch1 +k" is working | Should throw 696 or ignore the input
				// 1.4 ERR_CHANNELISFULL | Good!
				// 1.5 ERR_BADCHANMASK | Not required!
				// 1.6 ERR_NOSUCHCHANNEL | Then it creates a channel with that name and joins it
				// 1.7 ERR_TOOMANYCHANNELS | Not required!
				// 1.8 ERR_TOOMANYTARGETS | Not required!
				// 1.9 ERR_UNAVAILRESOURCE | Not required!
			
			// Correct Input
				// 1.0 JOIN <channel> | Good!
				// 1.1 JOIN <channel> bullshitValue | Good! (Also when the channel already exits)
				// 1.2 JOIN <channel1>,<channel2> | Good!
			
		// PART
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_NOSUCHCHANNEL | Not Good! //TODO If the client input ommits the # then it does not throw 403 | e.g "part abc"
				// 1.2 ERR_NOTONCHANNEL | Good! 
			
			// Correct Input
				// 1.0 PART <channel> | Good!
				// 1.1 Boradcast the part message to all the channel members | Good!
				// 1.2 PART <channel1>,<channel2> | Good!
				// 1.3 PART <channel1> partMessage | Good!
				// 1.4 PART <channel1>,<channel2> partMessage | Good!
		
		// MODE
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_KEYSET | Not required! //? IS THIS REALLY NOT REQUIRED?
				// 1.2 ERR_NOCHANMODES | Not required!
				// 1.3 ERR_CHANOPRIVSNEEDED | Good! //TODO Missing the nickname in the message
				// 1.4 ERR_USERNOTINCHANNEL | Good!
				// 1.5 ERR_UNKNOWNMODE | Good!
			// RPL_...
				// 1.0 RPL_CHANNELMODEIS | Not implemented! //? IS THIS REQUIRED?
				// 1.1 RPL_BANLIST | Not required! // * mode channel +b (+b is not required)
				// 1.2 RPL_ENDOFBANLIST | Not required! // * mode channel +b (+b is not required)
				// 1.3 RPL_EXCEPTLIST | Not required! // * mode channel +e (+e is not required)
				// 1.4 RPL_ENDOFEXCEPTLIST | Not required! // * mode channel +e (+e is not required)
				// 1.5 RPL_INVITELIST | Not required! // * mode channel +I (+I is not required)
				// 1.6 RPL_ENDOFINVITELIST | Not required! 
				// 1.7 RPL_UNIQOPIS | Not required! //* mode channel +O (+O is not required)
			// Correct Input
				// 1.0 MODE <channel> | Good!
				// 1.1 MODE <channel> <mode> bullshitValue | //! Not Good!
				// 1.2 MODE <channel> +o <nickname> | Good!
			
		// TOPIC
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_NOTONCHANNEL | Good!
				// 1.2 ERR_CHANOPRIVSNEEDED | Good!
				// 1.3 ERR_NOSUCHCHANNEL | Good! //* Works here even when # is missing unlike in PART
			// Correct 
				// 1.0 TOPIC <channel> | Good!
					// NO_TOPIC | Good!
					// RPL_TOPIC | Good!
				// Broadcase the topic to all the channel members | Good!
			
		// INVITE
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_NOSUCHNICK | Good! 
				// 1.2 ERR_NOTONCHANNEL | Good!
				// 1.3 ERR_USERONCHANNEL | Good!
				// 1.4 ERR_CHANOPRIVSNEEDED | Good!
				// 1.5 RPL_INVITING | Good!
				// 1.6 RPL_AWAY | Not required!
			// Correct
				// 1.0 INVITE <nickname> <channel> | Good!
				// 1.1 INVITE <nickname> <channel> bullshitValue | Good!

		// KICK
			// NUMERIC REPLIES
				// 1.0 ERR_NEEDMOREPARAMS | Good!
				// 1.1 ERR_NOSUCHCHANNEL | Good!
				// 1.2 ERR_BADCHANMASK | Not required!
				// 1.3 ERR_CHANOPRIVSNEEDED | Good!
				// 1.4 ERR_USERNOTINCHANNEL | //! Not Good! Throws 401 instead of 441 | e.g kick #channelThatExists notInChannelNickname
				// 1.5 ERR_NOTONCHANNEL | Good!
			// Correct
				// 1.0 KICK <channel> <nickname> | Good!
				// 1.1 KICK <channel> <nickname> bullshitValue | Good!
				// 1.2 KICK <channel> <nickname> :trailingMessage | Good!
				// 1.3 KICK <channel> <nickname> bullshitValue :trailingMessage | Good!
				// 1.4 Broadcast the kick message to all the channel members | Good!



		// PRIVMSG
			
			// Correct
				// Sends message to channel even if the user left the channel | //! Not Good!







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