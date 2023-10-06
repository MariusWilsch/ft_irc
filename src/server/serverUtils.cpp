#include "server.hpp"

void	ServerReactor::writeServerError( std::string function, std::string message, int error ) {
	std::cout << "Error: " << function << " - " << message << std::endl;
	std::cout << "Error: " << function << " - " << strerror(error) << std::endl;
	exit(1);
}

void ServerReactor::sendMsg(int socket, const string &command, const string &param, const string &trailing) {
    string message;

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


