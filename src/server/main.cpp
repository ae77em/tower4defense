#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Listener.h"

int main(int argc, char *argv[]){
    try {
        if (argc != 2) {
            throw std::string("Cantidad de parámetros incorrecta.");
        }
       
        uint16_t port = atoi(argv[1]);

        Listener listener(port);
        listener.start();

        while (true) {
            if (std::cin.get() == 'q') {
                break;
            }
        }

        listener.shutdown();
        listener.join();
    } catch (std::string ex) {
        std::cerr << ex << std::endl;
    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;;
    } catch (...) {
        std::cerr << "Ocurrió un error desconocido en el servidor.";
    }

    return EXIT_SUCCESS;
}
