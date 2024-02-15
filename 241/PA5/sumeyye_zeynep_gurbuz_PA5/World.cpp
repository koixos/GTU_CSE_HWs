#include "World.h"

World::World() {
    for (int i = 0; i < WORLDSIZE; i++)
        for (int j = 0; j< WORLDSIZE; j++)
            grid[i][j] = NULL;
}

World::~World() {
    for (int i = 0; i < WORLDSIZE; i++)
        for (int j = 0;j < WORLDSIZE; j++)
            if(grid[i][j] != NULL)
                delete (grid[i][j]);
}

void World::display() {
    std::cout << std::endl << std::endl;
    for (int j = 0; j < WORLDSIZE; j++) {
        for (int i = 0; i < WORLDSIZE; i++) {
            if (grid[i][j] == NULL)
                std::cout << ".";
            else if (grid[i][j]->getType().compare("Bulldozer") == 0)
                std::cout << "1";
            else if (grid[i][j]->getType().compare("Roomba") == 0)
                std::cout << "2";
            else if (grid[i][j]->getType().compare("Optimusprime") == 0)
                std::cout << "3";
            else if (grid[i][j]->getType().compare("Robocop") == 0)
                std::cout << "4";
        }
        std::cout << std::endl;
    }
}

void World::fight(Robot *attacker, Robot *rival) {
    while (true) {
        if (attacker->attack(rival)) { // if the rival is dead
            std::cout << rival->getName() << " is DEAD!" << std::endl;
            attacker->setMoved(); // flag the attacker as "moved"
            this->setAt(rival->getX(), rival->getY(), attacker); // set the attacker's new position
            this->setAt(attacker->getX(), attacker->getY(), NULL); // reset the old position
            return;
        }
        if (rival->attack(attacker)) { // if the attacker is dead
            std::cout << attacker->getName() << " is DEAD!" << std::endl;
            this->setAt(attacker->getX(), attacker->getY(), NULL); // delete the attacker
            return;
        }
    } 
}

bool World::move(Robot *robot) {
    int x = robot->getX(),
        y = robot->getY();
    if (x > 0 && this->getAt(x-1, y) != NULL) {
        // move left and fight
        this->fight(robot, this->getAt(x-1,y));
        return true; // meaning a death is occurred
    }
    if (x < WORLDSIZE-1 && this->getAt(x+1, y) != NULL) {
        // move right and fight
        this->fight(robot, this->getAt(x+1,y));
        return true; // meaning a death is occurred
    } 
    if (y > 0 && this->getAt(x, y-1) != NULL) {
        // move up and fight
        this->fight(robot, this->getAt(x,y-1));
        return true; // meaning a death is occurred
    } 
    if (y < WORLDSIZE-1 && this->getAt(x, y+1) != NULL) {
        // move down and fight
        this->fight(robot, this->getAt(x,y+1));
        return true; // meaning a death is occurred
    }
    // all directions empty
    if (x < WORLDSIZE-1) {
        this->setAt(x+1, y, robot); // move right
        this->setAt(x, y, NULL); // reset old position
        robot->setX(x+1); // set new coordinates
    } else if (y < WORLDSIZE-1) {
        this->setAt(x, y+1, robot); // move down
        this->setAt(x, y, NULL); // reset old position
        robot->setY(y+1); // set new coordinates
    } else if (x > 0) {
        this->setAt(x-1, y, robot); // move left
        this->setAt(x, y, NULL); // reset old position
        robot->setX(x-1); // set new coordinates
    } else if (y > 0) {
        this->setAt(x, y-1, robot); // move up
        this->setAt(x, y, NULL); // reset old position
        robot->setY(y-1); // set new coordinates
    }
    return false; // no robot is dead
}

Robot * World::search(int x, int y) {
    // this algorithm will travel the grid in a zigzag way
    // if x-coord is not in range
    if (x >= WORLDSIZE-1 && y < WORLDSIZE-1)
        for (int i = WORLDSIZE-1; i > 0; i--)
            for (int j = y+1; j < WORLDSIZE-1; j++)
                if (this->getAt(j, i) != NULL && !this->getAt(j, i)->getMoved())
                    return this->getAt(j, i);

    if (x >= WORLDSIZE-1 && y > 0)
        for (int i = WORLDSIZE-1; i > 0; i--)
            for (int j = y-1; j > 0; j--)
                if (this->getAt(j, i) != NULL && !this->getAt(j, i)->getMoved())
                    return this->getAt(j, i);
    
    // if y-coord is not in range
    if (y >= WORLDSIZE-1 && x < WORLDSIZE-1)
        for (int i = x+1; i < WORLDSIZE-1; i++)
            for (int j = WORLDSIZE-1; j > 0; j--)
                if (this->getAt(j, i) != NULL && !this->getAt(j, i)->getMoved())
                    return this->getAt(j, i);

    if (y >= WORLDSIZE-1 && x > 0)
        for (int i = x-1; i > 0; i--)
            for (int j = WORLDSIZE-1; j > 0; j--)
                if (this->getAt(j, i) != NULL && !this->getAt(j, i)->getMoved())
                    return this->getAt(j, i);
    
    // if every value is in ranges, continue to right
    for (int i = x; i < WORLDSIZE-1; i++)
        for (int j = y; j < WORLDSIZE-1; j++)
            if (this->getAt(j, i) != NULL && !this->getAt(j, i)->getMoved())
                return this->getAt(j, i);

    return NULL; // no robot found
}

Robot * World::getAt(int x, int y) {
    if (x >= 0 && x < WORLDSIZE && y >= 0 && y < WORLDSIZE)
        return grid[x][y];
    return NULL;
}

void World::setAt(int x, int y, Robot *robot) {
    if (x >= 0 && x < WORLDSIZE && y >= 0 && y < WORLDSIZE)
        grid[x][y] = robot;
}