#include "Channel.hpp"

Channel::Channel(std::string name) {
	_name = name;
	_modeChannel.i = false;
	int l = 0;
}

Channel::Channel(std::string name, std::string key) {
	_name = name;
	_modeChannel.key = key;
}

Channel::~Channel() {}
