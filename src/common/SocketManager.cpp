#include "Socket.h"
#include "SocketManager.h"
#include <thread>
#include <iostream>

#define TRON
#include "tron.h"

struct SocketManagerReader {
    Socket &socket;
    SocketManagerReader(Socket &socket) : socket(socket) {}
    void run() {
        TRACE("reader reached\n");
    }
};

struct SocketManagerWriter {
    Socket &socket;
    SocketManagerWriter(Socket &socket) : socket(socket) {}
    void run() {
         TRACE("writer reached\n");
    }
};

SocketManager::SocketManager(Socket &&socket) : socket(std::move(socket)),
        reader(), writer() {
    reader = std::thread(&SocketManagerReader::run,
            SocketManagerReader(socket));
    writer = std::thread(&SocketManagerWriter::run,
            SocketManagerWriter(socket));
}

SocketManager::~SocketManager() {
    reader.join();
    writer.join();
    socket.shutdown();
}
