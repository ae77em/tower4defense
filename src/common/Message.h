#ifndef TP4_TOWERDEFENSE_MESSAGE_H
#define TP4_TOWERDEFENSE_MESSAGE_H

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

    /*
     * Sets data contained in Json format in the object Json used.
     * */
    void setData(Json::Value& data);

    /*
     * Gets data contained in Json format in the object Json used.
     * */
    Json::Value& getData();

    /*
     * Get data contained in the Json objetct in string format.
     * */
    std::string getJsonString();

    /*
     * Serializes the data Json object into a string, and returns it.
     * */
    std::string serialize();

    /*
     * Serializes/loads the data in the string passed by parameter
     * into the data Json object.
     * */
    bool deserialize(std::string messageData);

    /*
     * Serializes/loads the data in the cstring passed by parameter
     * into the data Json object.
     * */
    bool deserialize(char *messageData);

    /*
     * Returns the data into the Json object in string format.
     * */
    std::string toString();


    /*
     * Print the data into the Json object in string format.
     * */
    void print();

    /*
     * Empty the Json object of all data.
     * */
    void cleanup();
};

#endif // TP4_TOWERDEFENSE_MESSAGE_H
