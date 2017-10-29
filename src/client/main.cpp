#include "Game.h"
#include "../common/TextMessage.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/Message.h"
#include "../common/MessageFactory.h"
#include <iostream>

int main(int argc, char *argv[]) {

    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    SharedBuffer in;
    SharedBuffer out;

    Socket client;

    client.connect(host.c_str(), port);

    TextMessage message0("");
    std::string idSerialized = message0.receiveFrom(client).getMessage();

    Message message;
    message.deserialize(idSerialized);
    Json::Value &root = message.getData();

    int myId = root["clientId"].asInt();

    std::cout<<"EL server me dio el id: "<<myId<<" jajaj que feliz que soy, fui creado"<<std::endl;


    std::string messageString = MessageFactory::getGamesRequest( client.getSocket() );
    //el primer mensaje es el envio del id
    TextMessage messageMatches( messageString );
    messageMatches.sendTo(client);


/*    std::string unaAccion = "";
    unaAccion += "1" + myId;

    TextMessage message(unaAccion);
    message.sendTo(client);

    unaAccion += "";
    unaAccion += "0" + myId;
*/
    //termino conexion
    TextMessage message2("0");
    message2.sendTo(client);

    /*Game game(in, out, client.getSocket());

    Listener listener(client, in);
    Sender sender(client, out);

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();
*/
    client.shutdown();
    client.close();

    return 0;
}

