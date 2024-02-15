#ifndef OPTIMUSPRIME_H
#define OPTIMUSPRIME_H

#include "Humanic.h"
#include <string>

class World;

class Optimusprime : public Humanic {
    public:
        Optimusprime();
        Optimusprime(std::string, int, int, int, int);

        int getDamage();
        bool attack(Robot *) override; // returns true if the attack is successfull
        std::string getType() override;
};

#endif // OPTIMUSPRIME_H