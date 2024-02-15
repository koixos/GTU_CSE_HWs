#ifndef ROOMBA_H
#define ROOMBA_H

#include "Robot.h"

class Roomba : public Robot {
    public:
        Roomba();
        Roomba(std::string, int, int, int, int);

        int getDamage();
        bool attack(Robot *) override; // returns true if the attack is successfull
        std::string getType() override;
};

#endif // ROOMBA_H