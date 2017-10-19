#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Listener.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        std::cerr << "Cantidad de parámetros incorrecta.";
        return 0;
    }

    uint16_t port = atoi(argv[1]);
    Listener listener(port);
    listener.start();

    while (true) if (std::cin.get() == 'q') break;

    listener.shutdown();
    listener.join();

    return EXIT_SUCCESS;
}
