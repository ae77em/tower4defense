#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Listener.h"
#include "../common/Utils.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        std::cerr << "Cantidad de parámetros incorrecta.";
        return 0;
    }

    Utils::printAsciiGameHeader();

    uint16_t port = atoi(argv[1]);

    std::mutex mPlayers;
    Listener listener(port,mPlayers);
    listener.start();

    while (true) if (std::cin.get() == 'q') break;

    listener.shutdown();
    listener.join();

    std::cout << "MainServer: se termino todo" << std::endl;

    return EXIT_SUCCESS;
}
