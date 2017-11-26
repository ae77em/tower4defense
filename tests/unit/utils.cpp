#include "catch.h"
#include "../../src/sdl/SdlUtils.h"
#include "../../src/common/Point.h"
#include <vector>

TEST_CASE("findClosest", "[utils-closest]") {
    SECTION("l vacio throws") {
        Point p = {0, 0};
        std::vector<Point> l;

        REQUIRE_THROWS(SdlUtils::findClosest(p, l));
    }

    SECTION("prueba 1") {
        Point p = {0, 0};
        std::vector<Point> l = {{1,3}, {0,0}, {-1,2}};

        auto x = SdlUtils::findClosest(p, l);
        REQUIRE(x.x == 0);
        REQUIRE(x.y == 0);
    }

    SECTION("prueba 2") {
        Point p = {1, 2};
        std::vector<Point> l = {{1,3}, {0,0}, {-1,2}};

        auto x = SdlUtils::findClosest(p, l);
        REQUIRE(x.x == 1);
        REQUIRE(x.y == 3);
    }
}
