#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include "Socket.h"
#include <string>

static const int MESSAGE_PREFIX_LENGTH = 11;

/* Un TextMessage viaja a traves de la red como un mensaje de longitud
 * variable
 * con prefijo de longitud preconcatenado, siendo este dato de logitud fija.
 *
 * El prefijo es una representación de texto de un número decimal, con
 * longitud fija de 10 caracteres, y con padding de ceros a la izquierda.
 */
class TextMessage {
    std::string message;

public:
    explicit TextMessage(std::string message);
    void sendTo(Socket &sock);
    TextMessage receiveFrom(Socket &sock);

    const std::string& getMessage();
};

#endif
