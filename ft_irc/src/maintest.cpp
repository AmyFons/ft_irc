#include "core/Server.hpp"
#include "core/Channel.hpp"
#include "core/User.hpp"
#include <cassert>
#include <iostream>

int main() {
    Server server(6667, "lol"); // ou l'équivalent minimal nécessaire

    // Crée des users factices avec des fd bidons
    User op(1);
    op.setNickname("opUser");
    op.setRealname("real");
    op.setHostname("host");

    User target(2);
    target.setNickname("targetUser");

    User bystander(3);
    bystander.setNickname("otherUser");

    // Crée un channel et ajoute les membres
    Channel chan("#test");
    chan.addMember(&op);
    chan.addMember(&target);
    chan.addMember(&bystander);
    chan.addOperator(&op); // selon comment est faite ta classe

    server.createChannel(chan); // ou l'équivalent pour peupler _channels

    // Test 1 : kick normal, ne doit pas throw
    try {
        server.kick("#test", "targetUser", "byebye", &op);
        std::cout << "Test 1 OK: kick normal reussi\n";
    } catch (std::exception &e) {
        std::cout << "Test 1 FAIL: " << e.what() << "\n";
    }

    // Test 2 : kick par un non-op, doit throw 482
    try {
        server.kick("#test", "otherUser", "reason", &bystander);
        std::cout << "Test 2 FAIL: aucune exception levee\n";
    } catch (std::runtime_error &e) {
        std::cout << "Test 2 OK: " << e.what() << "\n";
    }

    // Test 3 : kick sur channel inexistant, doit throw 403 (et pas crasher !)
    try {
        server.kick("#nope", "someone", "reason", &op);
        std::cout << "Test 3 FAIL: aucune exception levee\n";
    } catch (std::runtime_error &e) {
        std::cout << "Test 3 OK: " << e.what() << "\n";
    }

    // Test 4 : kick d'un user pas dans le channel, doit throw 441
    try {
        server.kick("#test", "inconnuUser", "reason", &op);
        std::cout << "Test 4 FAIL: aucune exception levee\n";
    } catch (std::runtime_error &e) {
        std::cout << "Test 4 OK: " << e.what() << "\n";
    }

    return 0;
}