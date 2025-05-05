#include "Optimusprime.h"

Optimusprime::Optimusprime() : Humanic() { }

Optimusprime::Optimusprime(std::string _name, int _strength, int _hitPoints, int _x, int _y)
    : Humanic(_name, _strength, _hitPoints, _x, _y) { };

int Optimusprime::getDamage() {
    int totalDamage = Humanic::getDamage(); // initial damage
    int chance = (rand() % 100) + 1;
    if (chance > 0 && chance <= 15) // by %15 chance
        totalDamage *= 2; // double attack damage
    return totalDamage;
}

bool Optimusprime::attack(Robot *rival) {
    int dmgAttack = this->getDamage(); // get amount of damage
    this->printMessage(dmgAttack, rival);
    rival->setHitPoints(rival->getHitPoints()-dmgAttack); // decrease the rival's hit points
    if (rival->getHitPoints() <= 0) // if the rival is dead, return true
        return true;
    return false; // if the rival is not dead, return false
}

std::string Optimusprime::getType() { return "Optimusprime"; }