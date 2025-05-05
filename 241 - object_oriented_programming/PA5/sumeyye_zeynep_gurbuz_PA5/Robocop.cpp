#include "World.h"
#include "Robocop.h"

Robocop::Robocop() : Humanic() { }

Robocop::Robocop(std::string _name, int _strength, int _hitPoints, int _x, int _y)
    : Humanic(_name, _strength, _hitPoints, _x, _y) { }

int Robocop::getDamage() {
    return Humanic::getDamage(); // initial damage
}

bool Robocop::attack(Robot *rival) {
    int dmgAttack = this->getDamage(); // get amount of damage
    this->printMessage(dmgAttack, rival);
    rival->setHitPoints(rival->getHitPoints()-dmgAttack); // decrease the rival's hit points
    if (rival->getHitPoints() <= 0) // if the rival is dead, return true
        return true;
    return false; // if the rival is not dead, return false
}

std::string Robocop::getType() { return "Robocop"; }