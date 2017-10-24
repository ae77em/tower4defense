#include <thread>
#include "Socket.h"
#include "ThreadedQueue.h"
#include "Message_Text.h"

#ifndef SOCKETMAN_H
#define SOCKETMAN_H

/* No tiene sentido copiar un thread. Consecuentemente se eliminan
 * las operaciones de copia. */

class SocketManager {
    Socket socket;
    std::thread reader, writer;
    ThreadedQueue<TextMessage> reader_queue, writer_queue;

    public:
    explicit SocketManager(Socket &&socket);
    ~SocketManager();

    SocketManager(const SocketManager &other) = delete;
    SocketManager& operator=(const SocketManager &other) = delete;

    ThreadedQueue<TextMessage>& sendQueue();
    ThreadedQueue<TextMessage>& receiveQueue();
};

#endif
