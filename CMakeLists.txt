cmake_minimum_required(VERSION 3.0)
project(tp4_towerdefense)

file(GLOB CLIENT_SOURCES "src/client/*.cpp" "src/client/*.h" "src/common/*.cpp"
        "src/common/*.h")
file(GLOB SERVER_SOURCES "src/server/*.cpp/" "src/server/*.h" "src/common/*.cpp"
        "src/common/*.h")

add_executable(client.exe ${CLIENT_SOURCES})
add_executable(server.exe ${SERVER_SOURCES})

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS} )
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(client.exe ${GTKMM_LIBRARIES})

find_package(Threads)
target_link_libraries(client.exe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server.exe ${CMAKE_THREAD_LIBS_INIT})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(client.exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -pedantic-errors -O0 -ggdb"
        "-DDEBUG -fno-inline -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
