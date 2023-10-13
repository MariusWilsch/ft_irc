#include "server.hpp"

bool	ServerReactor::doesChannelExist(string& channelName) {
	if (!channelName.empty() && channelName[0] == '#')
    channelName.erase(channelName.begin());
	return (_channelManager.getChannels().count(channelName));
}

ClientData& ServerReactor::getClientDataFast(int clientSocket) {
	return (_clientManager.getClientData(clientSocket));
}

void	ServerReactor::writeServerError( std::string function, std::string message, int error ) {
	std::cout << "Error: " << function << " - " << message << std::endl;
	std::cout << "Error: " << function << " - " << strerror(error) << std::endl;
	exit(1);
}

int ServerReactor::sendMsg(int socket, const string& clientInfo, const string &command, const string &param, const string &trailing) {
    string message;

		// Check if socket is valid
		if (socket == -1)
			return -1;

		// Append client info
		message += ":" + clientInfo + " ";

    // Append command
    message += command;

    // Append parameters

    message += " " + param;

    // Append trailing message if it exists
    if (!trailing.empty()) {
        message += " :" + trailing;
    }

    message += "\r\n";  // IRC messages end with \r\n

    send(socket, message.c_str(), message.length(), 0);
		return (0);
}


void ServerReactor::sendNumericReply(int socket, string numericReply, const string &param, const string &trailing) {
		string message;

		// Check if socket is valid
		if (socket == -1)
			return ;

		// Append command
		message += ":" + _serverName + " " + numericReply;

		// Append parameters
		if (!param.empty()) {
				message += " " + param;
		}

		// Append trailing message if it exists
		if (!trailing.empty()) {
				message += " :" + trailing;
		}

		message += "\r\n";  // IRC messages end with \r\n

		send(socket, message.c_str(), message.length(), 0);
}

void ServerReactor::sendNumericReply_FixLater(int socket, const string& reply) {
	// Check if socket is valid
	if (socket == -1)
		return ;

	send(socket, reply.c_str(), reply.length(), 0);
}


// void ServerReactor::sendMsg(int socket, const std::string &command, const std::string &trailing, int paramCount, ...) {
//     std::string message;
//     va_list args;

//     // Append command
//     message += command;

//     // Start processing variadic arguments
//     va_start(args, paramCount);

//     for (int i = 0; i < paramCount; ++i) {
//         std::string param = va_arg(args, const char*); // Get the next parameter
//         message += " " + param;
//     }

//     va_end(args); // Clean up

//     // Append trailing message if it exists
//     if (!trailing.empty()) {
//         message += " :" + trailing;
//     }

//     message += "\r\n";  // IRC messages end with \r\n

//     send(socket, message.c_str(), message.length(), 0);
// }


