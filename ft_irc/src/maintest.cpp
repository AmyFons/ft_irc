#include "core/Server.hpp"
#include "core/Channel.hpp"
#include "core/User.hpp"
#include <iostream>

// NOTE: Ces tests supposent le comportement "standard" attendu pour INVITE en IRC :
//  - l'inviteur doit etre membre du channel (sinon ERR_NOTONCHANNEL)
//  - si le channel est +i, seul un operateur peut inviter (sinon ERR_CHANOPRIVSNEEDED)
//  - si le nickname cible n'existe pas sur le serveur -> ERR_NOSUCHNICK
//  - si le nickname cible est deja membre du channel -> ERR_USERONCHANNEL
//  - sinon, l'utilisateur cible est ajoute a la liste des invites du channel
// Adapte les attentes si ton implementation reelle differe.

// static void printSection(const std::string &title)
// {
//     std::cout << "\n=== " << title << " ===" << std::endl;
// }

int main()
{
    Server server(6667, "password");

    // --- Setup: on enregistre les users cote Server pour que getUserByNickname() fonctionne ---
    server.addUnauthenticatedUser(1); // alice (operatrice)
    server.addUnauthenticatedUser(2); // bob (cible de l'invitation)
    server.addUnauthenticatedUser(3); // charlie (membre simple, non-op)
    server.addUnauthenticatedUser(4); // dave (deja membre du channel, pour test USERONCHANNEL)

    User *alice   = server.getUserById(1);
    User *bob     = server.getUserById(2);
    User *charlie = server.getUserById(3);
    User *dave    = server.getUserById(4);

    alice->setNickname("alice");
    bob->setNickname("bob");
    charlie->setNickname("charlie");
    dave->setNickname("dave");

    Channel channel("#test");
    channel.addMember(alice);
    channel.addOperator(alice);
    channel.addMember(charlie);
    channel.addMember(dave);
    channel.setInviteOnly(true); // +i active pour tester les restrictions

    server.addChannel(channel);
    std::vector<std::string> modestring;
    modestring.push_back("+----il+l");
    // modestring.push_back("i");
    server.mode(channel, modestring, alice);
    return 0;
}