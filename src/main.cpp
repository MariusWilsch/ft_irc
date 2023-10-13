/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:59:41 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/13 15:58:43 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*
	* parsing issues:
		1 - Empty element in the end of the parameters vector
		2 - When the client leaves the server, needs to remove it from all the channels also !
		3 - !PROBLEME:  When the server recieves lot of messages from clients, the program complete it's execution and stops without any crach~
		4 - Focus in NOTICE the clients when some important event happens!

		5 - Now to perform any operation inside the server the client needs to be registred inside the server.
*/

// FIXME: 462 You may not reregister

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