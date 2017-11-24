#ifndef TP4_TOWERDEFENSE_MESSAGE_H
#define TP4_TOWERDEFENSE_MESSAGE_H

#include <string>
#include <jsoncpp/json/json.h>
#include <stdint.h>
#include <sstream>
#include "Serializable.h"

class Message : public Serializable {
private:
    Json::Value data;

public:
    Message();
    virtual ~Message();

    void setData(Json::Value& data);
    Json::Value& getData();

    //TODO: document difference between these functions
    std::string getJsonString();
    std::string serialize();
    std::string toString();
    void print();

    bool deserialize(std::string &messageData);

    /*
     * Empty the Json object of all data.
     * */
    void cleanup();
};

#endif // TP4_TOWERDEFENSE_MESSAGE_H
