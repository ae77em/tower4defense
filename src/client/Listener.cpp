#include "Listener.h"
#include "../common/TextMessage.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"
#include "../sdl/Constants.h"
#include "../sdl/enemies/Abmonible.h"
#include "../sdl/Utils.h"

Listener::Listener(Socket &s, SharedBuffer &b) : server(s), buffer(b) {}

Listener::~Listener() {}

void Listener::run() {
    try {
        // TODO PARA PRUEBAS ////////////////////////
        cargarBufferConDatosDePrueba();
        // //////////////////////////////////////////

        std::string dataFromServer;
        TextMessage message("");

        while (true) {
            dataFromServer = message.receiveFrom(server).getMessage();
            buffer.addData(dataFromServer);
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}

void Listener::cargarBufferConDatosDePrueba() {

    double x;
    double y;
    double xFinal;
    double yFinal;

    std::vector<Point> camino;
    camino.push_back(Point(0, 5));
    camino.push_back(Point(1,5));
    camino.push_back(Point(2,5));
    camino.push_back(Point(3,5));
    camino.push_back(Point(4,5));
    camino.push_back(Point(4,6));
    camino.push_back(Point(4,7));
    camino.push_back(Point(5,7));
    camino.push_back(Point(6,7));
    camino.push_back(Point(7,7));
    camino.push_back(Point(8,7));
    camino.push_back(Point(9,7));
    camino.push_back(Point(10,7));
    camino.push_back(Point(11,7));
    camino.push_back(Point(12,7));
    camino.push_back(Point(12,6));
    camino.push_back(Point(12,5));
    camino.push_back(Point(13,5));
    camino.push_back(Point(14,5));
    camino.push_back(Point(15,5));
    camino.push_back(Point(16,5));

    Point point(0, 0);

    std::string putTowerRequest = MessageFactory::getPutTowerRequest(1,
                                                                     5 * CARTESIAN_TILE_WIDTH,
                                                                     6 * CARTESIAN_TILE_HEIGHT,
                                                                     true);
    Message message;
    message.deserialize(putTowerRequest);
    std::string putTowerNotification = MessageFactory::getPutTowerNotification(message);

    buffer.addData(putTowerNotification);

    for (unsigned int i = 0; i < camino.size(); ++i) {
        point = camino.at(i);
        x = point.x * CARTESIAN_TILE_WIDTH;
        y = point.y * CARTESIAN_TILE_HEIGHT;

        xFinal = x;
        yFinal = y;

        if (i < camino.size() - 1) {
            point = camino.at(i + 1);
            xFinal = point.x * CARTESIAN_TILE_WIDTH;
            yFinal = point.y * CARTESIAN_TILE_HEIGHT;
        }

        int currentTime;

        int direction = Utils::getMovementDirection(Utils::getNextMapDisplacement(x, xFinal),
                                                    Utils::getNextMapDisplacement(y, yFinal));

        while (x != xFinal || y != yFinal) {
            std::string movementNotification = MessageFactory::getMovementNotification(ENEMY_ABMONIBLE, x, y,
                                                                                       direction);

            buffer.addData(movementNotification);

            // todo: ablandar esto
            currentTime = SDL_GetTicks();
            while ((currentTime - SDL_GetTicks()) < 0.1) {};

            x += Utils::getNextMapDisplacement(x, xFinal);
            y += Utils::getNextMapDisplacement(y, yFinal);
        }

    }
}

