#include "Juego.h"
#include "ThreadedQueue.h"
#include "Message_Text.h"

#define T Juego
#define Queue ThreadedQueue<TextMessage>

void T::agregarTorre(unsigned x, unsigned y) {
    torres.emplace_back(x, y);
    notificar(TextMessage("nueva torre en " + std::to_string(x) + ","
            + std::to_string(y)));
}

void T::subscribirCliente(Queue &q) {
    client_queues.push_back(q);
}

void T::notificar(const TextMessage &msg) {
    for (auto &wrapper : client_queues) wrapper.get().push(msg);
}
