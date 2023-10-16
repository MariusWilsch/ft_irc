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