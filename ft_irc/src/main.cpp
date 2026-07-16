#include <iostream>
#include "Channel.hpp"
#include <list>

int main(void) {
	std::cout << "this is a great IRC." <<std::endl;

	Client client;
	// std::list<Client> clientList;
	Channel channel("bonjour");

	std::cout << "added" << std::endl;
	channel.addClient(client);
	for (std::list<Client>::iterator it = channel._clientList.begin(); it != channel._clientList.end(); ++it) {
		std::cout << (*it).getName() << std::endl;
	}

	channel.setOperator(client);

	std::cout << channel.isClientOperator(client, channel) << std::endl;
	std::cout << "erased" << std::endl;
	channel.removeClient(client);
	for (std::list<Client>::iterator it = channel._clientList.begin(); it != channel._clientList.end(); ++it) {
		std::cout << (*it).getName() << std::endl;
	}
}

/*
#include <list>

std::list<Client> ClientList;

std::list<Client>::iterator it;
for (it = ClientList.begin(); it != .end(); it++)

.empty*/