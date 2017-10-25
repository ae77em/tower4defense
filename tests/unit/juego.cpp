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
}
