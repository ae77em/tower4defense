#ifndef TP4_TOWERDEFENSE_COMMON_UTILS_H
#define TP4_TOWERDEFENSE_COMMON_UTILS_H

class CommonUtils {
public:
    static void printAsciiGameHeader();

    static int getNextMapDisplacement(int currentAxisPoint, int finalAxisPoint);

    static int getMovementDirection(int displacement, int mapDisplacement);
};

#endif //TP4_TOWERDEFENSE_COMMON_UTILS_H
