cmake_minimum_required(VERSION 3.0)
project(tp4_towerdefense)

include_directories("include")

file(GLOB CLIENT_SOURCES "src/client/*.cpp" "src/common/*.cpp")
file(GLOB SERVER_SOURCES "src/server/*.cpp/" "src/common/*.cpp")
file(GLOB TEST_SOURCES "tests/unit/*.cpp" "src/common/*.cpp")

add_executable(client.exe ${CLIENT_SOURCES} src/client/Game.cpp src/client/Game.h)
add_executable(server.exe ${SERVER_SOURCES})
add_executable(unit.exe ${TEST_SOURCES})

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS} )
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(client.exe ${GTKMM_LIBRARIES})

find_package(Threads)
target_link_libraries(client.exe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server.exe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(unit.exe ${CMAKE_THREAD_LIBS_INIT})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(client.exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -pedantic-errors -O0 -ggdb"
        "-DDEBUG -fno-inline -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
