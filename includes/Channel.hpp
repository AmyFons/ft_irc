#pragma once

#include <string>

struct mode {
	bool i; //for invitations
	std::string topic;
	std::string key;
	// o : liste de clients
	int l; //for limits of members
};

class Channel {
	public:
		Channel(std::string name);
		Channel(std::string name, std::string key);
		~Channel();

	private:
		std::string _name;
		mode _modeChannel;
		//Liste de client
};
