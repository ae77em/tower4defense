#ifndef TP4_TOWERDEFENSE_SERVER_H
#define TP4_TOWERDEFENSE_SERVER_H


#include <mutex>
#include <vector>
#include "../common/Socket.h"

class Server {

private:
    std::mutex m;
    std::vector<Socket*> clients;

public:
    Server();
    ~Server();
    void notifyAll(std::string message);
    void addClient(Socket &client);
};


#endif //TP4_TOWERDEFENSE_SERVER_H
