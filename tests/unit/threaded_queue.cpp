#include "catch.h"
#include "ThreadedQueue.h"
#include <thread>
#include <string>

TEST_CASE( "Push and pop from queue from a single thread", "[queue-linear]" ) {
    ThreadedQueue<std::string> q;

    SECTION ("pushing two items and popping them in order") {
        q.push("hello");
        q.push("world");

        REQUIRE(q.pop() == "hello");
        REQUIRE(q.pop() == "world");
    }
}

struct pushfiver {
    ThreadedQueue<std::string> &q;
    pushfiver(ThreadedQueue<std::string> &q) : q(q) {}
    void operator()() { for (int i = 0; i < 5; ++i) q.push(std::to_string(i)); }
};

TEST_CASE( "Push from one thread and pop from another", "[queue-one-to-one]" ) {
    ThreadedQueue<std::string> q;
    pushfiver p(q);

    SECTION ("pushing five times lets you pop five times") {
        std::thread t(std::move(p));

        REQUIRE(q.pop() == "0");
        REQUIRE(q.pop() == "1");
        REQUIRE(q.pop() == "2");
        REQUIRE(q.pop() == "3");
        REQUIRE(q.pop() == "4");
        t.join();
    }
}
