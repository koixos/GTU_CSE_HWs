#include "World.h"
#include <iostream>
#include <time.h>

int main() {
    srand(time(NULL));
    World world;
    int counter = 0;

    // creating Bulldozer instances
    for (int i = 0; i < INITIALROBOT; ) {
        int x = rand() % WORLDSIZE,
            y = rand() % WORLDSIZE;
         
        if (world.getAt(x, y) == NULL) {
            std::string name = "Bulldozer_";
            name = name.append(std::to_string(i));
            world.setAt(x, y, new Bulldozer(name, 50, 200, x, y));
            ++counter;
            ++i;
        }
    }

    // creating Roomba instances
    for (int i = 0; i < INITIALROBOT; ) {
        int x = rand() % WORLDSIZE,
            y = rand() % WORLDSIZE;
         
        if (world.getAt(x, y) == NULL) {
            std::string name = "Roomba_";
            name = name.append(std::to_string(i));
            world.setAt(x, y, new Roomba(name, 3, 10, x, y));
            ++counter;
            ++i;
        }
    }

    // creating Optimusprime instances
    for (int i = 0; i < INITIALROBOT; ) {
        int x = rand() % WORLDSIZE,
            y = rand() % WORLDSIZE;
         
        if (world.getAt(x, y) == NULL) {
            std::string name = "Optimusprime_";
            name = name.append(std::to_string(i));
            world.setAt(x, y, new Optimusprime(name, 100, 100, x, y));
            ++counter;
            ++i;
        }
    }

    // creating Robocop instances
    for (int i = 0; i < INITIALROBOT; ) {
        int x = rand() % WORLDSIZE,
            y = rand() % WORLDSIZE;
         
        if (world.getAt(x, y) == NULL) {
            std::string name = "Robocop_";
            name = name.append(std::to_string(i));
            world.setAt(x, y, new Robocop(name, 30, 40, x, y));
            ++counter;
            ++i;
        }
    }

    int x = 0, y = 0;    
    while (counter > 1) {
        Robot *attacker = world.search(x,y);
        world.display();
        if (world.move(attacker))
            --counter;
        x = attacker->getX();
        y = attacker->getY();
    }

    return 0;
}