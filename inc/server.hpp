#pragma once


#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "main.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "message.hpp"

#include "ExecuteCommands.hpp"

struct CommandProperties {
	int				mandatoryParams;
	bool			ignoreTrailing;
	
	CommandProperties();
	CommandProperties(int m, bool i);
};



/**
	 * @brief Class to handle server-level operations
	 * 
	 * @details This class is responsible for handling the server socket, multiplexing and events.
	 * @param _serverSocket The server socket
	 * @param _kq The kqueue
	 * @param _isShutdown A boolean to know if the server is shutdown
	 * @param _connectionPassword The password to connect to the server
	 * @param _clientManager The client manager where all the clients are stored
	 * @param _channelManager The channel manager where all the channels are stored
	 * @param _serverName The name of the server.
 */

class ServerReactor {
	private:
		int								_serverSocket;
		int								_kq;
		string							_serverIP;
		bool							_isShutdown;
		string							_connectionPassword;
		string							_serverName;
		ClientManager					_clientManager;
		ChannelManager					_channelManager;
		map <string, CommandProperties> _properties;
	public:
		/*			Class Default Functions			*/
		
		ServerReactor( int port, string connectionPassword );
		~ServerReactor();
		
		/*			Socket & Multiplexing			*/

		void	setupServerSocket( int port );
		void	setToNonBlocking(int fd);
		void	updateMoinitoring(int clientSocket, int filter, int flags);
		
		

		/*			EVENTS			*/
		
		void	acceptNewClient();
		void	recieveIncomingMessage( int clientSocket );
		 
		/*			SERVER T0 CLIENT COMMUICATION			*/

		string		createMsg(ClientData& clientData, const string &command, const vector <string> &params, const string &trailing = "");
		void		sendMsg(int socket, const string& clientInfo ,const string &command, string param, const string &trailing = "");
		int 		sendMsg_FixLater(int socket, const string &message);
		void		sendNumericReply_FixLater(int socket, const string& reply);

		/* 					GETTERS					*/
		ClientManager&			getClientManager( void );
		ChannelManager&			getChannelManager( void );
		string					getServerName( void );
		string					getServerPassword( void );

		/*			SMART GETTERS			*/
		bool				doesChannelExist(const string& channelName );
		ClientData& getClientDataFast( int clientSocket );
		ChannelData& getChannelDataFast( string& channelName );

		/*			MAIN			*/
		void	run();

		/*			UTILS			*/
			
		void	writeServerError(string functionName, string errorMessage, int errorNumber);
		void	createPropertiesMap( void );
};
