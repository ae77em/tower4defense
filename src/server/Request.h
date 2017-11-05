#ifndef TP4_TOWERDEFENSE_REQUEST_H
#define TP4_TOWERDEFENSE_REQUEST_H


#include "../common/Message.h"

class Request {
private:
    Message& requestMessage;
public:
    Request(Message& m);

    std::string getAsString(std::string key);

    int getAsInt(std::string key);

};


#endif //TP4_TOWERDEFENSE_REQUEST_H
