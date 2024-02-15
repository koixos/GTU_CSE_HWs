#include "Humanic.h"

Humanic::Humanic() : Robot() { }

Humanic::Humanic(std::string _name, int _strength, int _hitPoints, int _x, int _y)
    : Robot(_name, _strength, _hitPoints, _x, _y) { };

int Humanic::getDamage() {
    int totalDamage = Robot::getDamage(); // initial damage
    int chance = (rand() % 100) + 1;
    if (chance > 0 && chance <= 10) // by %10 chance
        totalDamage += 50; // nuke attack damage
    return totalDamage;
}

//std::string Humanic::getType() { return "Humanic"; }