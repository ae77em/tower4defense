#include <gtkmm.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "../common/Socket.h"

int main(int argc, char **argv) {
    Socket client;
        
    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    client.connect(host.c_str(), port);


    std::string text = "asdasdasdasdasd";
    std::cout << "Send to server: " << text << std::endl;

    uint32_t size = text.length();
    client.send(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    client.send(text.c_str(), text.length());

    char aux_response[2048] = { 0 };

    client.receive(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    client.receive(aux_response, size);
    text = std::string(aux_response);

    std::cout << "Response: " << text << std::endl;
    

    return 0;
}