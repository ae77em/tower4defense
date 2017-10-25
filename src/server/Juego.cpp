#include "Juego.h"
#include "ThreadedQueue.h"
#include "Message_Text.h"

#define T Juego
#define Queue ThreadedQueue<TextMessage>

void T::agregarTorre(unsigned x, unsigned y) {
    torres.emplace_back(x, y);
}

void T::subscribirCliente(Queue &q) {
    client_queues.push_back(q);
}
