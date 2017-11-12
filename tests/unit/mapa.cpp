#include "catch.h"
#include "../../src/common/modelo/Mapa.h"


TEST_CASE("Creacion de mapa", "[map-new]") {
    SECTION("mapa vacio empieza con praderas") {
        model::Mapa mapa(10, 10);
        REQUIRE(mapa.casilla(0, 0) == '.');
    }

    SECTION("acceso fuera de rango causa excepcion") {
        model::Mapa mapa(10, 10);

        REQUIRE_NOTHROW(mapa.casilla(9, 9) == '.');
        REQUIRE_THROWS( mapa.casilla(10, 10) );
    }

    SECTION("dimensiones correctas") {
        model::Mapa m(0, 0);
        CHECK(m.dimensiones().x == 0);
        CHECK(m.dimensiones().y == 0);

        m = model::Mapa(10, 10);
        CHECK(m.dimensiones().x == 10);
        CHECK(m.dimensiones().y == 10);
    }
}

TEST_CASE("Modificacion de mapa", "[map-set]") {
    model::Mapa m(10, 10);

    SECTION("") {
        m.setCasilla('!', 1, 1);
        REQUIRE(m.casilla(1, 1) == '!');
    }
}
