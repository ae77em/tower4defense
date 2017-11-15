#include <iostream>
#include "Utils.h"
#include "../sdl/Constants.h"

void Utils::printAsciiGameHeader() {
    std::cout <<
"       _                           ___     _       __                    \n"
"      | |                         /   |   | |     / _|                   \n"
"      | |_ _____      _____ _ __ / /| | __| | ___| |_ ___ _ __  ___  ___ \n"
"      | __/ _ \\ \\ /\\ / / _ \\ '__/ /_| |/ _` "
        "|/ _ \\  _/ _ \\ '_ \\/ __|/ _ \\\n"
"      | || (_) \\ V  V /  __/ |  \\___  | "
        "(_| |  __/ ||  __/ | | \\__ \\  __/\n"
"       \\__\\___/ \\_/\\_/ \\___|_|      "
        "|_/\\__,_|\\___|_| \\___|_| |_|___/\\___|"
        << std::endl
        << std::endl;
}

int Utils::getNextMapDisplacement(int currentAxisPoint, int finalAxisPoint) {
    int toReturn;

    if (currentAxisPoint != finalAxisPoint) {
        if (currentAxisPoint < finalAxisPoint) {
            toReturn = 1;
        } else {
            toReturn = -1;
        }
    } else {
        toReturn = 0;
    }

    return toReturn;
}

int Utils::getMovementDirection(int xDirection, int yDirection) {
    int toReturn;
    if (xDirection == 1) {
        toReturn = X_POSITIVE;
    } else if (xDirection == -1) {
        toReturn = X_NEGATIVE;
    } else if (yDirection == 1) {
        toReturn = Y_POSITIVE;
    } else if (yDirection == -1) {
        toReturn = Y_NEGATIVE;
    } else {
        toReturn = -1;
    }
    return toReturn;
}
