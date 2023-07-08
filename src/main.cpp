/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:12 by verdant           #+#    #+#             */
/*   Updated: 2023/07/08 14:46:51 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// Test with: telnet localhost <port>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./server <port> <password>" << std::endl;
		return (1);
	}
	ServerReactor server(atoi(argv[1]), 10, argv[2]);
	server.run();
	return (0);
}