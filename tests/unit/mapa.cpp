#include "catch.h"
#include "../../src/common/modelo/Mapa.h"


TEST_CASE("Creacion de mapa", "[map-new]") {
    SECTION("mapa vacio empieza con praderas") {
        Mapa mapa(10, 10);
        REQUIRE(mapa.casilla(0, 0) == '.');
    }
}
