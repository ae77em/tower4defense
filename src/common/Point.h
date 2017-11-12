#ifndef TP4_TOWERDEFENSE_POINT_H
#define TP4_TOWERDEFENSE_POINT_H

#include <jsoncpp/json/json.h>

struct Point {
    Point(int x, int y);

    Json::Value serialize() const;
    static Point deserialize(const Json::Value &s);

    bool isPositive();

    bool isEqualsTo(Point p2){
        return x == p2.x && y == p2.y;
    }

    int x;
    int y;
};

struct DecimalPoint {
public:
    DecimalPoint(double x, double y){
        this->x = x;
        this->y = y;
    }

    ~DecimalPoint(){};

    bool isPositive(){
        return x >= 0 and y >= 0;
    }

    double x;
    double y;
};

#endif //TP4_TOWERDEFENSE_POINT_H
