#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Listener.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        cerr << "Cantidad de parámetros incorrecta.";
        return 0;
    }
    try {
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
