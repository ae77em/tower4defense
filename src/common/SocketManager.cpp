#include "Socket.h"
#include "SocketManager.h"
#include <thread>
#include <iostream>
#include "Message_Text.h"
#include "ThreadedQueue.h"

#define TRON
#include "tron.h"

struct SocketManagerReader {
    Socket &socket;
    ThreadedQueue<TextMessage> &queue;

    SocketManagerReader(Socket &socket, ThreadedQueue<TextMessage> &queue)
            : socket(socket), queue(queue) {}

    void run() {
        TRACE("reader reached\n");
    }
};

struct SocketManagerWriter {
    Socket &socket;
    ThreadedQueue<TextMessage> &queue;

    SocketManagerWriter(Socket &socket, ThreadedQueue<TextMessage> &queue)
            : socket(socket), queue(queue) {}

    void run() {
        TRACE("writer reached\n");

        TextMessage message("hola");
        message.sendThrough(socket);
    }
};

SocketManager::SocketManager(Socket &&socket) : socket(std::move(socket)),
        reader(), writer(), reader_queue(), writer_queue() {
    TRACE("SocketManager constructor, socket fd: "
            + std::to_string(this->socket.get_socket()) + "\n");

    reader = std::thread(&SocketManagerReader::run,
            SocketManagerReader(this->socket, reader_queue));
    writer = std::thread(&SocketManagerWriter::run,
            SocketManagerWriter(this->socket, writer_queue));
}

SocketManager::~SocketManager() {
    reader.join();
    writer.join();
    socket.shutdown();
}


ThreadedQueue<TextMessage>& SocketManager::sendQueue() {
    return writer_queue;
}

ThreadedQueue<TextMessage>& SocketManager::receiveQueue() {
    return reader_queue;
}

