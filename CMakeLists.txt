cmake_minimum_required(VERSION 3.0) 
project(tp4_towerdefense)      

file(GLOB CLIENT_SOURCES "src/client/*.cpp" "src/client/*.h" "src/common/*.cpp" "src/common/*.h")
file(GLOB SERVER_SOURCES "src/server/*.cpp/" "src/server/*.h" "src/common/*.cpp" "src/common/*.h")
 
add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS} )
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(client ${GTKMM_LIBRARIES})

find_package(Threads)
target_link_libraries (client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -pedantic-errors -O0 -ggdb -DDEBUG -fno-inline -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
