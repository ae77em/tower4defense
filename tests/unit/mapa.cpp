#include "catch.h"
#include "../../src/common/model/Map.h"


TEST_CASE("Creacion de mapa", "[map-new]") {
    SECTION("mapa vacio empieza con praderas") {
        model::Map mapa(10, 10);
        REQUIRE(mapa.tile(0, 0) == '.');
    }

    SECTION("acceso fuera de rango causa excepcion") {
        model::Map mapa(10, 10);

        REQUIRE_NOTHROW(mapa.tile(9, 9) == '.');
        REQUIRE_THROWS(mapa.tile(10, 10) );
    }

    SECTION("dimensiones correctas") {
        model::Map m(0, 0);
        CHECK(m.dimensions().x == 0);
        CHECK(m.dimensions().y == 0);

        m = model::Map(10, 10);
        CHECK(m.dimensions().x == 10);
        CHECK(m.dimensions().y == 10);
    }
}

TEST_CASE("Modificacion de mapa", "[map-set]") {
    model::Map m(10, 10);

    SECTION("") {
        m.setTile('!', 1, 1);
        REQUIRE(m.tile(1, 1) == '!');
    }
}
