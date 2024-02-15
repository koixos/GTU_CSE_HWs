#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>

class Bulldozer;
class Roomba;
class Optimusprime;
class Robocop;

class Robot {
    private:
        std::string name = "null";
        int strength = -1;
        int hitPoints = -1;
        int x = -1;
        int y = -1;
        bool moved = false;
    public:
        Robot();
        Robot(std::string, int, int, int, int);

        void printMessage(int, Robot *);
        int getDamage(); // Returns amount of damage for one round
        virtual bool attack(Robot *) = 0; // Attacks to the given robot
        virtual std::string getType() = 0;

        std::string getName();
        int getStrength();
        int getHitPoints();
        int getX();
        int getY();
        bool getMoved();
        
        void setName(std::string);
        void setStrength(int);
        void setHitPoints(int);
        void setX(int);
        void setY(int);
        void setMoved();
};

#endif // ROBOT_H