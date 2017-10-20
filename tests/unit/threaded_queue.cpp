#include "catch.h"
#include "ThreadedQueue.h"

TEST_CASE( "Push and pop from queue from a single thread", "[queue-linear]" ) {
    ThreadedQueue q;

    SECTION ("pushing two items and popping them in order") {
        q.push("hello");
        q.push("world");

        REQUIRE(q.pop() == "hello");
        REQUIRE(q.pop() == "world");
    }
}
