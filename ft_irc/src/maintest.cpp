
#include "core/Server.hpp"
#include "core/Channel.hpp"
#include "core/User.hpp"
#include <iostream>
 
int main() {
	Server server(6667, "lol");
 
	// Users factices
	User op(1);
	op.setNickname("opUser");
	op.setRealname("real");
	op.setHostname("host");
 
	User target(2);
	target.setNickname("targetUser");
	target.setRealname("real2");
	target.setHostname("host2");
 
	User bystander(3);
	bystander.setNickname("otherUser");
	bystander.setRealname("real3");
	bystander.setHostname("host3");
 
	User inconnu(4);
	inconnu.setNickname("inconnuUser");
 
	// Channel + membres
	Channel chan("#test");
	chan.addMember(&op);
	chan.addMember(&target);
	chan.addMember(&bystander);
	chan.addOperator(&op);
 
	server.createChannel(chan);
 
	// Channel jamais enregistre cote serveur -> ERR_NOSUCHCHANNEL
	Channel fakeChan("#nope");
 
	// Test 1 : kick normal (avec raison), ne doit pas throw
	try {
		server.kick(chan, &target, "byebye", &op);
		std::cout << "Test 1 OK: kick normal reussi\n";
	} catch (std::exception &e) {
		std::cout << "Test 1 FAIL: " << e.what() << "\n";
	}
 
	// Test 2 : kick par un non-op, doit throw ERR_CHANOPRIVSNEEDED
	// (target a deja ete kicke au test 1, on cible bystander)
	try {
		server.kick(chan, &bystander, "reason", &target);
		std::cout << "Test 2 FAIL: aucune exception levee\n";
	} catch (std::runtime_error &e) {
		std::cout << "Test 2 OK: " << e.what() << "\n";
	}
 
	// Test 3 : kick sur channel inexistant cote serveur, doit throw ERR_NOSUCHCHANNEL
	try {
		server.kick(fakeChan, &op, "reason", &op);
		std::cout << "Test 3 FAIL: aucune exception levee\n";
	} catch (std::runtime_error &e) {
		std::cout << "Test 3 OK: " << e.what() << "\n";
	}
 
	// Test 4 : kick d'un user pas dans le channel, doit throw ERR_NOTONCHANNEL
	try {
		server.kick(chan, &inconnu, "reason", &op);
		std::cout << "Test 4 FAIL: aucune exception levee\n";
	} catch (std::runtime_error &e) {
		std::cout << "Test 4 OK: " << e.what() << "\n";
	}
 
	// Test 5 : kick sans raison (overload 3 args), doit fonctionner sur bystander
	try {
		server.kick(chan, &bystander, &op);
		std::cout << "Test 5 OK: kick sans raison reussi\n";
	} catch (std::exception &e) {
		std::cout << "Test 5 FAIL: " << e.what() << "\n";
	}
 
	return 0;
}
