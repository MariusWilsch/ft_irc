/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:50:48 by verdant           #+#    #+#             */
/*   Updated: 2023/07/07 17:52:36 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>

using std::string;

class	ChannelData {
	private:
		string		_name;
		string		_topic;
		set <int>	_clientSockets;
		set <int>	_operators;
		int 			_maxUsers;
	public:
		ChannelData(string name, string topic, int maxUsers);
		// void	addClient(int clientSocket);
		// void	removeClient(int clientSocket);
		void	addOperator(int clientSocket);
		void	removeOperator(int clientSocket);
};

class	ChannelManager {
	private:
		map<string, ChannelData>	_channels;
	public:
		void	addChannel(string name, const ChannelData &channelData);
		void	removeChannel(string name);
		// Additional methods, i.e addClient, removeClient, etc.
};

#endif