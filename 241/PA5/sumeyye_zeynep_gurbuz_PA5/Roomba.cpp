#include "Roomba.h"

Roomba::Roomba() : Robot() { }

Roomba::Roomba(std::string _name, int _strength, int _hitPoints, int _x, int _y)
    : Robot(_name, _strength, _hitPoints, _x, _y) { }

int Roomba::getDamage() {
    return Robot::getDamage() * 2; // Roombas attack twice
}

bool Roomba::attack(Robot *rival) {
    int dmgAttack = this->getDamage(); // get amount of damage
    this->printMessage(dmgAttack, rival);
    rival->setHitPoints(rival->getHitPoints()-dmgAttack); // decrease the rival's hit points
    if (rival->getHitPoints() <= 0) // if the rival is dead, return true
        return true;
    return false; // if the rival is not dead, return false
}

std::string Roomba::getType() { return "Roomba"; }