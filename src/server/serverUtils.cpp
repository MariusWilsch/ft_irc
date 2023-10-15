#include "server.hpp"

bool	ServerReactor::doesChannelExist(string& channelName) {
	if (channelName[0] != '#') // For NC because LimeChat always sends a channel name with a # in front
		return (false);
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

int ServerReactor::sendMsg(int socket, const string& clientInfo, const string &command, string param, const string &trailing) {
    string message;

		// Check if socket is valid
		if (socket == -1)
			return -1;

		// Append client info
		message += ":" + clientInfo + " ";

    // Append command
    message += command;

    // Append parameters


		if (command == "NICK") {
			message += " :" + param;
		} else {
				message += " " + param;
		}
			
    // Append trailing message if it exists
    if (!trailing.empty()) {
        message += " :" + trailing;
    }

    message += "\r\n";  // IRC messages end with \r\n

    send(socket, message.c_str(), message.length(), 0);
		return (0);
}

int ServerReactor::sendMsg_FixLater(int socket, const string& reply) {
		// Check if socket is valid
		if (socket == -1)
			return -1;
	
    send(socket, reply.c_str(), reply.length(), 0);
		return (0);
}

/**
 * @brief 
 * 
 * @param clientSocket The socket of the client
 * @param command The command to send
 * @param params The parameters to send
 * @return string 
 */
string ServerReactor::
createInfoMsg(ClientData& clientData, const string& command, const vector <string>& params) {
		string message;

		// Append client info
		message += ":" + clientData.getClientInfo() + " ";

		// Append command
		message += command;

		// Append nickname
		// message += " " + clientData.getNickname();

		// Append Channel name
		message += " " + params[0];

		// Append parameters
		if (command == "MODE" || command == "INVITE" ) {
			for (size_t i = 1; i < params.size(); ++i)
					message += " :" + params[i];
		} else 

		// Append trailing message if it exists
		// if (!trailing.empty()) {
		// 		message += " :" + trailing;
		// }

		message += "\r\n";  // IRC messages end with \r\n


		// cout << "Message: " << message << endl;

		return (message);
}

void ServerReactor::sendNumericReply(int socket, string numericReply, const string &param, const string &trailing) {
		string message;

		// Check if socket is valid
		if (socket == -1)
			return ;

		// Append command
		// message += ":" + _serverName + " " + numericReply;

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

vector <string> ServerReactor::processItems(const string& channelName, const string& item2, const string& item3) {
	vector <string> vec;
	
	if (!channelName.empty()) vec.push_back(channelName);
  if (!item2.empty()) vec.push_back(item2);
  if (!item3.empty()) vec.push_back(item3);	
	return (vec);
}

void ServerReactor::printUserInformation(int clientSocket)
{
    std::map<string, ChannelData>::iterator it;
    std::cout << "Number of channels: " << _channelManager.getChannels().size() << std::endl;
    std::map<string, ChannelData> m = _channelManager.getChannels();
    for (it = m.begin(); it != m.end(); ++it) {
        std::cout << "Channel name: " << it->second.getName() << std::endl;
        if (it->second.getSecurity()) {
            std::cout << "~~~> Channel is private" << std::endl;
        } else {
            std::cout << "~~~> Channel is public" << std::endl;
        }

        std::set<int>::iterator cl;
        std::set<int> s = it->second.getClientSockets();
        for (cl = s.begin(); cl != s.end(); ++cl) {
            std::cout << "  >>> Client: " << *cl << std::endl;
        }

        std::set<int>::iterator op;
        std::set<int> f = it->second.getOperators();
        for (op = f.begin(); op != f.end(); ++op) {
            std::cout << "  >>> Operator: " << *op << std::endl;
        }
    }
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


