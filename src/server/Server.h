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

#include "ServerPlayer.h"
#include "ClientRequestHandler.h"
#include "../common/Message.h"
#include "ServerGame.h"

class ClientRequestHandler;
class ServerPlayer;
class ServerGame;


class Server : public Thread {

private:
    std::set<std::string> matches;
    std::mutex m;


    std::mutex& mutexPlayers;
    //std::vector<std::reference_wrapper<ThreadedQueue<TextMessage>>> clients;
    ThreadedQueue<Message>& queueMessagesClient;
    std::map<unsigned int,ServerPlayer*> players;
    std::vector<ServerGame*> games;

public:
    Server(std::mutex& m,ThreadedQueue<Message>& tq);

    ~Server();

    void run();

    std::string getGamesList();

    void createGameAndNotifyAll(Message &request);
    unsigned int createMatch();
    void notifyAllCreationGame(int gameId,int clientIdWhoCreatedGame);

    void addPlayerToMatch(int idMatch, ServerPlayer *sp);

    void sendGamesListToClient(int clientId);
    void setQueueRequestClient(ThreadedQueue<Message> &queue);
    void createAndRunPlayer(Socket* s);

    unsigned int CreateGame();
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
    /*
     * Método de servicio para realizar las notificaciones sin lockear.
     * Se consume desde métodos que necesitan notificar y ya activaron el lock.
     */
    void notifyAllWithoutLock(string message);

    void addPlayerToGame(int clientId,int gameId);

    void notifyTo(int clientId, string &message);
};


#endif //TP4_TOWERDEFENSE_SERVER_H
