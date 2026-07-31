/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:46:21 by afons             #+#    #+#             */
/*   Updated: 2026/07/31 16:16:58 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/Server.hpp"
#include "../core/User.hpp"
#include "../core/Channel.hpp"
#include <stdexcept>
#include <iostream>

std::vector<std::string> split_mode(std::string listMode) {
	std::vector<std::string> listString;
	std::string::iterator it = listMode.begin();

	while(it != listMode.end()) {
		while (it != listMode.end() && (*it == '+' || *it == '-')) {
			++it;
		}
		std::string::iterator pos_tmp = it;
		while (it != listMode.end() && *it != '+' && *it != '-') {
			++it;
		}
		std::string string = listMode.substr(pos_tmp - listMode.begin() - 1, it - pos_tmp + 1);
		listString.push_back(string); 
	}
	return listString;
}

void Server::mode(Channel &channel, std::vector<std::string> listMode, User *user) {
	(void)user;

	std::map<std::string, Channel>::iterator it = _channels.find(channel.getName());
	if (it == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	//PARSING
	std::vector<std::string> modestring = split_mode(listMode.at(0));

	// LAUNCH MODE
	std::vector<std::string>::iterator it_modestring = modestring.begin();
	for(; it_modestring != modestring.end(); ++it_modestring) {
		
	}
}
