#include "server.hpp"

bool	ServerReactor::doesChannelExist(const string& channelName) {
	if (channelName[0] != '#') // For NC because LimeChat always sends a channel name with a # in front
		return (false);
	return (_channelManager.getChannels().count(channelName));
}

ClientData& ServerReactor::getClientDataFast(int clientSocket) {
	return (_clientManager.getClientData(clientSocket));
}

ChannelData& ServerReactor::getChannelDataFast(string& channelName) {
	return (_channelManager.getChannelByName(channelName));
}


void	ServerReactor::writeServerError( std::string function, std::string message, int error ) {
	std::cout << "Error: " << function << " - " << message << std::endl;
	std::cout << "Error: " << function << " - " << strerror(error) << std::endl;
	exit(1);
}

void ServerReactor::sendMsg(int socket, const string& clientInfo, const string &command, string param, const string &trailing) {
    string message;

	// Check if socket is valid
	if (socket == -1)
		return ;

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
}

int ServerReactor::sendMsg_FixLater(int socket, const string& reply) {
	// Check if socket is valid
	if (socket == -1)
		return -1;
	
    send(socket, reply.c_str(), reply.length(), 0);
		return (0);
}

string ServerReactor::
createMsg(ClientData& clientData, const string& command, const vector <string>& params, const string& trailing) {
	string message;

	// Append client info
	message += ":" + clientData.getClientInfo() + " ";

	// Append command
	message += command;

	// Append Channel name
	message += " " + params[0];

	// Append parameters
	if (command == "INVITE" || command == "PART" || command == "TOPIC") {
		for (size_t i = 1; i < params.size(); ++i)
				message += " :" + params[i];
	} else {
		for (size_t i = 1; i < params.size(); ++i)
				message += " " + params[i];
	}

	// Append trailing message if it exists
	if (!trailing.empty()) {
			message += " :" + trailing;
	}

	message += "\r\n";  // IRC messages end with \r\n

	return (message);
}



void ServerReactor::sendNumericReply_FixLater(int socket, const string& reply) {
	// Check if socket is valid
	if (socket == -1)
		return ;

	send(socket, reply.c_str(), reply.length(), 0);
}

