#ifndef ROBOCOP_H
#define ROBOCOP_H

#include "Humanic.h"

class Robocop : public Humanic {
    public:
        Robocop();
        Robocop(std::string, int, int, int, int);

        int getDamage();
        bool attack(Robot *) override; // returns true if the attack is successfull
        std::string getType() override;
};

#endif // ROBOCOP_H