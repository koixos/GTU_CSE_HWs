#ifndef HUMANIC_H
#define HUMANIC_H

#include "Robot.h"

class Humanic : public Robot {
    public:
        Humanic();
        Humanic(std::string, int, int, int, int);

        int getDamage();
};

#endif // HUMANIC_H