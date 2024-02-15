#include "Robot.h"
#include "Bulldozer.h"
#include "Roomba.h"
#include "Optimusprime.h"
#include "Robocop.h"

const int WORLDSIZE = 10;
const int INITIALROBOT = 5;

class World {
    protected:
        Robot *grid[WORLDSIZE][WORLDSIZE]; 
    public:
        World();
        ~World();

        void display(); // displays the game grid
        void fight(Robot *, Robot *);
        bool move(Robot *);
        Robot * search(int, int); // searchs for an available robot 

        Robot * getAt(int, int);
        void setAt(int, int, Robot *);
};