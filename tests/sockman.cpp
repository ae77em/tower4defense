#include "SocketManager.h"
#include "Socket.h"

int main(int argc, char **argv) {
    Socket acceptor;
    acceptor.bind(8080);
    acceptor.listen();

    SocketManager server(Socket(acceptor.accept()));
}
