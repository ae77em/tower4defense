#ifndef TP4_TOWERDEFENSE_REQUEST_H
#define TP4_TOWERDEFENSE_REQUEST_H

#include "Message.h"
#include <list>
#include <string>

class Request {
private:
    Message& requestMessage;
public:
    explicit Request(Message& m);

    std::string getAsString(std::string key);

    int getAsInt(std::string key);

    std::list<std::string> getAsStringVector(std::string key);

    bool getAsBool(std::string key);
};


#endif //TP4_TOWERDEFENSE_REQUEST_H
