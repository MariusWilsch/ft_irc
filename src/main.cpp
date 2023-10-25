/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:59:41 by mwilsch           #+#    #+#             */
/*   Updated: 2023/10/25 12:26:33 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <signal.h>

// void	f(){
// 	system("leaks ircserv");
// }

int	main( int argc, char **argv )
{
	// atexit(f);
<<<<<<< HEAD
=======
	signal(SIGPIPE, SIG_IGN);
>>>>>>> 59e91648f6cace039a5b3355eb06abe7e33bc5c9
	if (argc != 3)
	{
		std::cout << "Usage: ./server <port> <password>" << std::endl;
		return (1);
	}
	ServerReactor server(atoi(argv[1]), argv[2]);
	server.run();
}