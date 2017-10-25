#include <vector>
#include <utility>
#include <functional>
#include "ThreadedQueue.h"
#include "Message_Text.h"

#ifndef JUEGO_H
#define JUEGO_H

#define T Juego
#define Queue ThreadedQueue<TextMessage>

/* -----> x
 * |
 * v
 * y
 */
class T {
    std::vector<std::pair<unsigned, unsigned>> torres;
    std::vector< std::reference_wrapper<Queue> > client_queues;

public:
    void agregarTorre(unsigned x, unsigned y);
    void subscribirCliente(Queue &q);

private:
    void notificar(const TextMessage &msg);
};

#undef T
#undef Queue

#endif
