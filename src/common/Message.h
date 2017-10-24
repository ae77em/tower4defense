#ifndef Message_H
#define Message_H

#include <string>
#include <jsoncpp/json/json.h>
#include <stdint.h>
#include <sstream>
#include "Serializable.h"

//#include "Configuracion.h"

typedef uint32_t message_lenght_t;

class Message : public Serializable {
private:
    Json::Value data;

public:
    Message();
    virtual ~Message();

    void setData(Json::Value& data);
    Json::Value& getData();
    std::string getJsonString();
    std::string serialize();
    bool hydrate(std::string messageData);
    bool hydrate(char *messageData);
    std::string toString();
    void print();
    void cleanup();
};

#endif // Message_H
