#ifndef TP4_TOWERDEFENSE_REQUEST_H
#define TP4_TOWERDEFENSE_REQUEST_H

#include "Message.h"
#include "Point.h"
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

    double getAsDouble(std::string key);

    Point getAsPoint(std::string xKey, std::string yKey);
};


#endif //TP4_TOWERDEFENSE_REQUEST_H
