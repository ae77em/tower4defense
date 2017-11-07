#include <SDL2/SDL_timer.h>
#include <iostream>
#include "Listener.h"
#include "../common/TextMessage.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

Listener::Listener(Socket &s, SharedBuffer &b) : server(s), buffer(b) {}

Listener::~Listener() {}

void Listener::run() {
    try {
        // TODO PARA PRUEBAS - ESTO DSPS SE BORRA ////////////////////////
        //cargarBufferConDatosDePrueba();
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

/*
 * ESTO SIMULA DISTINTAS NOTIFICACIONES DEL SERVER PARA LLEVAR
 * ADELANTE UN JUEGO. ESTO VUELA CUANDO INTEGREMOS SERVER CON
 * CLIENTE DEFINITIVAMENTE.
*/
/*

void Listener::cargarBufferConDatosDePrueba() {

    */
/*double x;
    double y;
    double xFinal;
    double yFinal;*//*


    std::vector<Point> camino;
    // supongo que recorro una matriz de baldosas de CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
    camino.push_back(Point(0 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(1 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(2 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(3 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(5 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(6 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(7 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(8 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(9 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(10 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(11 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(13 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(14 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(15 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(16 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));

    Point point(0, 0);

    std::vector<ActorEnemy> horda;

    */
/*ActorEnemy enemy1;
    enemy1.setPath(camino);
    ActorEnemy enemy2;
    enemy2.setPath(camino);
    ActorEnemy enemy3;

    horda.push_back(enemy1);
    horda.push_back(enemy2);
    horda.push_back(enemy3);*//*


    for (int x = 0; x < 5; ++x){
        ActorEnemy enemy;
        enemy.setPath(camino);
        horda.push_back(enemy);
    }

    for (unsigned i = 0; i < horda.size(); ++i){
        horda.at(i).setCurrentPathPosition(-i * CARTESIAN_TILE_WIDTH / 2);
    }

    std::string putTowerRequest = MessageFactory::getPutTowerRequest(1,
                                                                     5 * CARTESIAN_TILE_WIDTH,
                                                                     6 * CARTESIAN_TILE_HEIGHT,
                                                                     true);
    Message message;
    message.deserialize(putTowerRequest);
    std::string putTowerNotification = MessageFactory::getPutTowerNotification(message);

    buffer.addData(putTowerNotification);

    std::vector<Point> auxCamino;
    bool termine = false;
    int avance = 0;
    unsigned ultimoQueEntro = 0;
    bool entraronTodos;

    while (!termine) {
        horda[ultimoQueEntro].setIsWalking(true);

        while (avance < CARTESIAN_TILE_WIDTH){
            for (unsigned h = 0; h <= ultimoQueEntro; ++h) {
                horda[h].advance();
                int xpos = horda[h].getXPosition();
                std::string movementNotification =
                        MessageFactory::getMovementNotification(h, ENEMY_ABMONIBLE, xpos,
                                                                horda[h].getYPosition(), horda[h].getCurrentDirection());

                buffer.addData(movementNotification);
            }
            ++avance;
        }

        avance = 0;

        if (!entraronTodos){
            ++ultimoQueEntro;
            // corrijo si me pasé
            if (ultimoQueEntro >= horda.size()){
                ultimoQueEntro = horda.size() - 1;
                entraronTodos = true;
            }
        }

        for (unsigned h = 0; h <= ultimoQueEntro; ++h) {
            horda[h].gotoNextPathPosition();
        }

        termine = (horda[horda.size()-1].getCurrentPosition() == (int)(camino.size() - 1));
    }

    std::cout << "salid";

}

*/
