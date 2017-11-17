#ifndef TP4_TOWERDEFENSE_NOTIFICABLE_H
#define TP4_TOWERDEFENSE_NOTIFICABLE_H

#include <string>

class Notificable {
public:
    virtual void notify(std::string&) = 0;
};


#endif //TP4_TOWERDEFENSE_NOTIFICABLE_H
