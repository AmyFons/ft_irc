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

static void printSection(const std::string &title)
{
    std::cout << "\n=== " << title << " ===" << std::endl;
}

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

    // --- Test 1 : l'operateur invite bob (pas encore membre) sur channel +i ---
    printSection("Test 1: operateur invite un user externe sur channel +i (doit reussir)");
    try
    {
        server.invite("bob", channel, alice);
        if (channel.isInvited(bob->getFd()))
            std::cout << "OK -> bob est bien dans la liste des invites" << std::endl;
        else
            std::cout << "ECHEC -> bob n'apparait pas comme invite" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ECHEC (inattendu): " << e.what() << std::endl;
    }

    // --- Test 2 : un non-operateur tente d'inviter sur channel +i (doit echouer) ---
    printSection("Test 2: non-operateur invite sur channel +i (doit echouer)");
    {
        Channel restricted("#restricted");
        restricted.addMember(alice);
        restricted.addOperator(alice);
        restricted.addMember(charlie); // charlie = simple membre, pas op
        restricted.setInviteOnly(true);

        try
        {
            server.invite("bob", restricted, charlie);
            std::cout << "ECHEC (inattendu) -> aucune exception levee" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "OK -> exception attrapee: " << e.what() << std::endl;
        }
    }

    // --- Test 3 : invite un nickname qui n'existe pas sur le serveur ---
    printSection("Test 3: invite un nickname inexistant (doit echouer)");
    try
    {
        server.invite("ghost", channel, alice);
        std::cout << "ECHEC (inattendu) -> aucune exception levee" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "OK -> exception attrapee: " << e.what() << std::endl;
    }

    // --- Test 4 : invite un user deja membre du channel ---
    printSection("Test 4: invite un user deja membre du channel (doit echouer)");
    try
    {
        server.invite("dave", channel, alice);
        std::cout << "ECHEC (inattendu) -> aucune exception levee" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "OK -> exception attrapee: " << e.what() << std::endl;
    }

    // --- Test 5 : l'inviteur n'est pas lui-meme membre du channel ---
    printSection("Test 5: inviteur non-membre du channel (doit echouer)");
    {
        Channel other("#other");
        other.addMember(dave); // alice n'est pas dedans

        try
        {
            server.invite("bob", other, alice);
            std::cout << "ECHEC (inattendu) -> aucune exception levee" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "OK -> exception attrapee: " << e.what() << std::endl;
        }
    }

    // --- Test 6 : sur un channel NON invite-only, un simple membre peut inviter ---
    printSection("Test 6: membre simple invite sur channel non-restreint (doit reussir)");
    {
        Channel open("#open");
        open.addMember(alice);
        open.addOperator(alice);
        open.addMember(charlie); // simple membre
        // pas de setInviteOnly -> false par defaut

        try
        {
            server.invite("bob", open, charlie);
            if (open.isInvited(bob->getFd()))
                std::cout << "OK -> bob est bien invite par un membre non-op" << std::endl;
            else
                std::cout << "ECHEC -> bob n'apparait pas comme invite" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "ECHEC (inattendu): " << e.what() << std::endl;
        }
    }

    return 0;
}