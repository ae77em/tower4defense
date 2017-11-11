#include <gtkmm.h>
#include <iostream>
#include "../common/SharedBuffer.h"
#include "../common/Socket.h"
#include "Listener.h"
#include "../common/MessageFactory.h"
#include "../common/Utils.h"
#include "../common/TextMessage.h"
#include "Sender.h"

int main(int argc, char **argv) {
    SharedBuffer toReceive;
    SharedBuffer toSend;

    if (argc < 3) {
        std::cerr << "game <host> <port>" << std::endl;
        return 0;
    }

    Utils::printAsciiGameHeader();

    /* me conecto al server, y le pido mi id de jugador */
    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    Socket server;

    server.connect(host.c_str(), port);

    TextMessage textmessage("");
    std::string dataFromServer = textmessage.receiveFrom(server).getMessage();

    Message message;
    message.deserialize(dataFromServer);
    int clientId = MessageFactory::getClientId(message);

    /* instancio el juego con el id de jugador ya obtenido */
    GameAccessWindow gameAccess(&server, toSend, toReceive);
    gameAccess.setClientId(clientId);

    /* instancio los canales de comunicación con el server */
    Listener listener(&server, gameAccess, toReceive);
    Sender sender(&server, toSend);

    /* inicio el juego */
    gameAccess.start();

    listener.start();
    sender.start();

    /* pido los datos iniciales para cargar la pantalla de acceso */
    std::string mapsRequest = MessageFactory::getExistingMapsRequest(gameAccess.getClientId());
    toSend.addData(mapsRequest);

    std::string matchesRequest = MessageFactory::getExistingMatchesRequest(gameAccess.getClientId());
    toSend.addData(matchesRequest);

    /* cierro sender, ya que no voy a usarlo más */
    toSend.setClientProcessEnded(true);
    sender.join();

    listener.join();
    gameAccess.join();

    server.shutdown();
    server.close();

    return 0;
}
