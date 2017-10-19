/* A TextMessage flows through the network as a fixed-length prefix,
 * and a variable-length tail.
 *
 * The prefix is a ten-character, fixed-width, zero padded decimal
 * representation of the length of the tail.
 */

#include "../common/Message.h"
#include "../common/Socket.h"
#include <string>

#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

class TextMessage : public Message {
    std::string message;

public:
    TextMessage(std::string message);
    void sendThrough(Socket &sock);
    void receiveFrom(Socket &sock);
};

#endif

