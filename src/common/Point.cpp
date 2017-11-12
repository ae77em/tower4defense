#include "Point.h"
#include <jsoncpp/json/json.h>

Point::Point(int x, int y) : x(x), y(y) {}

bool Point::isPositive() {
    return x >= 0 and y >= 0;
}

Json::Value Point::serialize() const {
    Json::Value root;
    root.append(x);
    root.append(y);
    return root;
}

Point Point::deserialize(const Json::Value &s) {
    return Point(s[0].asInt(), s[1].asInt());
}
