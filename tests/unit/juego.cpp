#include "catch.h"
#include "ThreadedQueue.h"
#include "Juego.h"
#include "Message_Text.h"

TEST_CASE("Game model communicates with client", "[juego]") {
    Juego juego;

    SECTION("building a tower notifies client") {
        ThreadedQueue<TextMessage> q;
        juego.subscribirCliente(q);

        juego.agregarTorre(1, 0);
        REQUIRE(q.pop().getMessage() == "nueva torre en 1,0");
    }

    SECTION("building a tower notifies all clients") {
        ThreadedQueue<TextMessage> q1, q2;

        juego.subscribirCliente(q1);
        juego.subscribirCliente(q2);

        juego.agregarTorre(1, 0);
        REQUIRE(q1.pop().getMessage() == "nueva torre en 1,0");
        REQUIRE(q2.pop().getMessage() == "nueva torre en 1,0");
    }
}
