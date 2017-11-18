cmake_minimum_required(VERSION 3.0)
project(tp4_towerdefense)

SET(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -pedantic-errors -O0 -ggdb -DDEBUG -fno-inline -std=c++11")

file(GLOB CLIENT_SOURCES
        "src/client/*.h"
        "src/client/*.cpp"
        "src/common/*.h"
        "src/common/*.cpp"
        "src/common/**/*.h"
        "src/common/**/*.cpp"
        "src/sdl/*.h"
        "src/sdl/*.cpp"
        "src/sdl/*/*.h"
        "src/sdl/*/*.cpp")
file(GLOB SERVER_SOURCES
        "src/server/*.cpp"
        "src/server/*/*.cpp"
        "src/server/*/*/*.cpp"
        "src/common/*.cpp"
        "src/common/*/*.cpp")
file(GLOB EDITOR_SOURCES "src/editor/*.cpp" "src/common/*.cpp"
        "src/common/**/*.cpp" "src/sdl/*.cpp") 
file(GLOB TEST_SOURCES "tests/unit/*.cpp" "src/common/*.cpp"
        "src/common/**/*.cpp" "src/sdl/*.cpp")

add_executable(game.exe ${CLIENT_SOURCES})
add_executable(server.exe ${SERVER_SOURCES})
add_executable(editor.exe ${EDITOR_SOURCES})
add_executable(unit.exe ${TEST_SOURCES})

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS} )
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(game.exe ${GTKMM_LIBRARIES})

pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
target_link_libraries(game.exe ${JSONCPP_LIBRARIES})
target_link_libraries(server.exe ${JSONCPP_LIBRARIES})
target_link_libraries(editor.exe ${JSONCPP_LIBRARIES})
target_link_libraries(unit.exe ${JSONCPP_LIBRARIES})

find_package(Threads)
target_link_libraries(game.exe ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(server.exe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(editor.exe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(unit.exe ${CMAKE_THREAD_LIBS_INIT})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(game.exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARIES})
TARGET_LINK_LIBRARIES(editor.exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARIES})
TARGET_LINK_LIBRARIES(unit.exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARIES})

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror -pedantic -pedantic-errors -O0 -ggdb -DDEBUG -fno-inline -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

