//
// Created by esteban on 21/10/17.
//

#ifndef TP4_TOWERDEFENSE_POINT_H
#define TP4_TOWERDEFENSE_POINT_H


class Point {
public:
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }

    ~Point(){};

    bool isPositive(){
        return x >= 0 and y >= 0;
    }

    int x;
    int y;
};


#endif //TP4_TOWERDEFENSE_POINT_H
