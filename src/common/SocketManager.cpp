#include "Socket.h"
#include "SocketManager.h"
#include <thread>
#include "Message_Text.h"
#include "ThreadedQueue.h"

struct SocketManagerReader {
    Socket &socket;
    ThreadedQueue<TextMessage> &queue;

    SocketManagerReader(Socket &socket, ThreadedQueue<TextMessage> &queue)
            : socket(socket), queue(queue) {}

    void run() {
        while (true) {
            try {
                TextMessage message = receiveFrom(socket);
                queue.push(message);
            } catch (std::exception) {
                queue.close();
                break;
            }
        }
    }
};

struct SocketManagerWriter {
    Socket &socket;
    ThreadedQueue<TextMessage> &queue;

    SocketManagerWriter(Socket &socket, ThreadedQueue<TextMessage> &queue)
            : socket(socket), queue(queue) {}

    void run() {
        while (! queue.isAtEnd()) {
            TextMessage message = queue.pop();
            message.sendThrough(socket);
        }
    }
};

SocketManager::SocketManager(Socket &&socket) : socket(std::move(socket)),
        reader(), writer(), reader_queue(), writer_queue() {
    reader = std::thread(&SocketManagerReader::run,
            SocketManagerReader(this->socket, reader_queue));
    writer = std::thread(&SocketManagerWriter::run,
            SocketManagerWriter(this->socket, writer_queue));
}

SocketManager::~SocketManager() {
    writer_queue.close();
    writer.join();

    socket.shutdown();
    reader.join();
}


ThreadedQueue<TextMessage>& SocketManager::sendQueue() {
    return writer_queue;
}

ThreadedQueue<TextMessage>& SocketManager::receiveQueue() {
    return reader_queue;
}

