#include "catch.h"
#include "../../src/common/modelo/Mapa.h"


TEST_CASE("Creacion de mapa", "[map-new]") {
    SECTION("mapa vacio empieza con praderas") {
        Mapa mapa(10, 10);
        REQUIRE(mapa.casilla(0, 0) == '.');
    }

    SECTION("acceso fuera de rango causa excepcion") {
        Mapa mapa(10, 10);

        REQUIRE_NOTHROW(mapa.casilla(9, 9) == '.');
        REQUIRE_THROWS( mapa.casilla(10, 10) );
    }

    SECTION("dimensiones correctas") {
        Mapa m(0, 0);
        CHECK(m.dimensiones().x == 0);
        CHECK(m.dimensiones().y == 0);

        m = Mapa(10, 10);
        CHECK(m.dimensiones().x == 10);
        CHECK(m.dimensiones().y == 10);
    }
}
