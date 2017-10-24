#include "Socket.h"
#include "SocketManager.h"
#include <thread>
#include <iostream>
#include "Message_Text.h"

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

        TextMessage message("hola");
        message.sendThrough(socket);
    }
};

SocketManager::SocketManager(Socket &&socket) : socket(std::move(socket)),
        reader(), writer() {
    TRACE("SocketManager constructor, socket fd: "
            + std::to_string(this->socket.get_socket()) + "\n");

    reader = std::thread(&SocketManagerReader::run,
            SocketManagerReader(this->socket));
    writer = std::thread(&SocketManagerWriter::run,
            SocketManagerWriter(this->socket));
}

SocketManager::~SocketManager() {
    reader.join();
    writer.join();
    socket.shutdown();
}
