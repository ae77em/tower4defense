#include "SocketManager.h"
#include "../src/common/Socket.h"
#include <cstdlib>

int main(int argc, char **argv) {
    uint16_t port = argv[1] ? atoi(argv[1]) : 8080;

    Socket acceptor;
    acceptor.bind(port);
    acceptor.listen();

    SocketManager server(Socket(acceptor.accept()));

    ThreadedQueue<TextMessage> &q = server.sendQueue();
    q.push(TextMessage("hola"));
    q.push(TextMessage("mundo!"));
}
