#ifndef BOOLDOZER_H
#define BOOLDOZER_H

#include "Robot.h"

class Bulldozer : public Robot {
    public:
        Bulldozer();
        Bulldozer(std::string, int, int, int, int);

        int getDamage();
        bool attack(Robot *) override; // returns true if the attack is successfull
        std::string getType() override;
};

#endif // BOOLDOZER_H