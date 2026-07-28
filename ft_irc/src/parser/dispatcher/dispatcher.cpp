/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/28 15:08:43 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <stdexcept>
#include <iostream>

/* Commands to manage channel and user */
static void	channelCommandsDispatch(Server &server, std::string command, User &user) {
	std::vector<std::string> parameters = Parser::getParameters();

	std::vector<Channel> listChannel;
	std::vector<std::string> listKey;
	std::cout << "COMMAND :" <<command << std::endl;
	if (command == "join") {
		listChannel = Parser::getlistChannel(parameters[0]);
		
		std::cout << listChannel.size() << std::endl;
		for (std::vector<Channel>::iterator it_listChannel = listChannel.begin(); it_listChannel != listChannel.end(); ++it_listChannel) {
			std::cout << it_listChannel->getName() << std::endl;
		}

		listKey = Parser::getlistKey(parameters[1]);
		if (parameters.size() > 1) {
			server.join(listChannel, listKey, &user); }
		else if (parameters.size() == 0)
			throw std::runtime_error("Need a channel name.");
		else
			server.join(listChannel, &user);
	}
	else if (command == "KICK") {
		Channel *channel = server.getChannelByName(parameters[0]);
		User *kicked = server.getUserByNickname(parameters[1]);
		if (parameters.size() > 2)
			server.kick(*channel, kicked, parameters[2], &user);
		else if (parameters.size() == 0)
			throw std::runtime_error("Need a channel name.");
		else if (parameters.size() == 1)
			throw std::runtime_error("Need a nickname.");
		else
			server.kick(*channel, kicked, &user);
	}
	else if (command == "PART") {
		listChannel = Parser::getlistChannel(parameters[0]);
		if (parameters.size() > 1)
			server.part(listChannel, parameters[1], &user);
		else if (parameters.size() == 0)
			throw std::runtime_error("Need a channel name.");
		else
			server.part(listChannel, parameters[1], &user);
	}
	else if (command == "INVITE") {
		Channel *channel = server.getChannelByName(parameters[1]);
		if (parameters.size() != 2)
			server.invite(parameters[0], *channel, &user);
		else
			throw std::runtime_error("Invalid command");
	}
	else if (command == "TOPIC") {
		Channel *channel = server.getChannelByName(parameters[0]);
		if (parameters.size() > 1)
			server.topic(*channel, parameters[1], &user);
		else if (parameters.size() == 0)
			throw std::runtime_error("Need a channel name.");
		else
			server.topic(*channel, &user);
	}
}

/* Commands to send a message */
static void messageCommandsDispatch(std::string command, User &user) {
	(void)user;
	std::cout << command << " is not handled for the moment." << std::endl;
	/* code */
}

/* Commands to register a user */
static void userCommandsDispatch(std::string command, User &user) {
	std::vector<std::string> parameters = Parser::getParameters();

	if (command == "user") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for USER command.");
		user.setRealname(parameters[0]);
		user.setProvidedUser(true);
	}
	else if (command == "nick") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for NICK command.");
		user.setNickname(parameters[0]);
		user.setProvidedNick(true);
	}
	else if (command == "pass") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for PASS command.");
		// if (parameters[0] != Server::getPassword())
		// 	throw std::runtime_error("Error: Invalid password");
		user.setProvidedPassword(true);
	}
	
	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword())
		user.setAuthenticated(true);
}

void dispatchCommand(Server &server, User &user) {
	std::string command = Parser::getCommand();
	Parser::buildPrefix(user);

	try
	{
		switch (Parser::getCommandListId())
		{
		case 1:
			channelCommandsDispatch(server, command, user);
			break ;
		case 2:
			messageCommandsDispatch(command, user);
			break ;
		case 3:
			userCommandsDispatch(command, user);
			break ;

		default:
			throw Parser::InvalidCommandException();
			break ;
		}
	}

	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
