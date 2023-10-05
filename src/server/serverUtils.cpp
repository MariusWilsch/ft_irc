#include "server.hpp"

void	ServerReactor::writeServerError( std::string function, std::string message, int error ) {
	std::cout << "Error: " << function << " - " << message << std::endl;
	std::cout << "Error: " << function << " - " << strerror(error) << std::endl;
	exit(1);
}

// void sendMessageToClient(int socket, const std::string &command, const std::vector<std::string> &parameters, const std::string &trailing = "") {
//     std::string message;

//     // Append command
//     message += command;

//     // Append parameters
//     for (const std::string &param : parameters) {
//         message += " " + param;
//     }

//     // Append trailing message if it exists
//     if (!trailing.empty()) {
//         message += " :" + trailing;
//     }

//     message += "\r\n";  // IRC messages end with \r\n

//     send(socket, message.c_str(), message.length(), 0);
// }


