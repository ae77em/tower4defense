#ifndef TP4_TOWERDEFENSE_SERVER_H
#define TP4_TOWERDEFENSE_SERVER_H

#include <mutex>
#include <vector>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <set>
#include "../common/Socket.h"
#include "../common/ThreadedQueue.h"
#include "../common/Thread.h"
#include "../common/TextMessage.h"
#include "../common/SocketManager.h"
#include "../common/Message.h"

#include "ServerPlayer.h"
#include "ServerGame.h"

class Server : public Thread {

private:
    std::set<std::string> matches;
    std::mutex m;

    std::mutex& mutexPlayers;

    ThreadedQueue<Message>& queueMessagesClient;
    std::map<unsigned int,ServerPlayer*> players;
    std::map<std::string,ServerGame*> games;

public:
    Server(std::mutex& m,ThreadedQueue<Message>& tq);

    void run();

    std::string getGamesList();

    void createGame(int request, string matchName);
    bool createMatch(string basicString);
    void notifyAllCreationGame(int gameId,int clientIdWhoCreatedGame);

    void addPlayerToMatch(std::string nameMatch, ServerPlayer *sp);

    void sendGamesListToClient(int clientId);
    void setQueueRequestClient(ThreadedQueue<Message> &queue);
    void createAndRunPlayer(Socket* s);

    unsigned int getAmountGames();

    /*
     * Notifica a todos los clientes el mensaje pasado por parámetro.
     * Este método activa el mutex, y lo libera al terminar.
     * messsage: mensaje que se debe enviar a los clientes.
     * */
    void notifyAll(std::string message);

    /*
     * Agrega un cliente asociado al server.
     * queue: cola encargada de manejar el envío y recepción de datos.
     * */
    void addClient(ThreadedQueue<TextMessage> &queue);

private:

    void addPlayerToGame(int clientId, std::string mName, vector<string> elementName);

    void notifyTo(int clientId, string &message);

    void removeClient(int id);

    void createGame(string basic_string);

    void startMatch(int id, string basic_string);

    void sendElementsToClient(int clientId, string matchName);
};


#endif //TP4_TOWERDEFENSE_SERVER_H
