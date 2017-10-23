#include "Socket.h"
#include "SocketManager.h"
#include <thread>

struct SocketManagerReader {
    Socket &socket;
    SocketManagerReader(Socket &socket) : socket(socket) {}
    void run() {
    }
};

struct SocketManagerWriter {
    Socket &socket;
    SocketManagerWriter(Socket &socket) : socket(socket) {}
    void run() {
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
    socket.shutdown();
}
