#include "Robot.h"
#include <math.h>

Robot::Robot() { }

Robot::Robot(std::string _name, int _strength, int _hitPoints, int _x, int _y) {
    this->name.swap(_name);
    this->strength = _strength;
    this->hitPoints = _hitPoints;
    this->x = _x;
    this->y = _y;
}

void Robot::printMessage(int damage, Robot *rival) {
    std::cout
        << this->getName() << "(" << this->getHitPoints() << ") hits "
        << rival->getName() << "(" << rival->getHitPoints() << ") with " << damage << std::endl
        << "New hitpoints of " << rival->getName() << " is " << rival->getHitPoints()-damage << std::endl; 
}

int Robot::getDamage() {
    return (rand() % strength) + 1; // All robots inflict damage at least in range of (0, strength].
}

std::string Robot::getName() { return name; }

int Robot::getStrength() { return strength; }

int Robot::getHitPoints() { return hitPoints; }

int Robot::getX() { return x; }

int Robot::getY() { return y; }

bool Robot::getMoved() { return moved; }

void Robot::setName(std::string _name) { this->name.swap(_name); }

void Robot::setStrength(int _strength) { this->strength = _strength; }

void Robot::setHitPoints(int _hitPoints) { this->hitPoints = _hitPoints; }

void Robot::setX(int _x) { this->x = _x; }

void Robot::setY(int _y) { this->y = _y; }

void Robot::setMoved() { this->moved = !this->moved; }