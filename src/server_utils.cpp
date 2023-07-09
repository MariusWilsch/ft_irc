/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 13:10:05 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/09 13:15:04 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"

void	ServerReactor::updateMoinitoring(int clientSocket, int filter, int flags)
{
	struct kevent kev;
	EV_SET(&kev, clientSocket, filter, flags, 0, 0, NULL);
	if (kevent(_kq, &kev, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		writeError("kevent", "Failed to update monitoring");
	}
}

void	ServerReactor::writeError(string functionName, string errorMessage) {
	cerr << "Error: " << functionName << " failed: " << errorMessage << endl;
	exit(1);
}

// void	ServerReactor::writeError(string functionName, string errorMessage, int errorNumber) {
// {
// 	cerr << "Error: " << functionName << " failed: " << errorMessage << " (" << errorNumber << ")" << endl;
// 	exit(1);
// }
