#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, int dir, int size, int depth)
        : GraphObject(imageID, startX, startY, size, depth) {}; 
        
        virtual void doSomething() = 0;
};

class GhostRacer: public Actor {
    public:
        GhostRacer(int imageID, double startX, double startY, int dir, int size, int depth)
        : Actor(imageID, startX, startY, size, dir,  depth) {};

        void doSomething() {

        };
};


//1. A Ghost Racer object must have an image ID of IID_GHOST_RACER.
//2. A Ghost Racer object starts out alive.
//3. A Ghost Racer object has a direction (that it faces) of 90 degrees (upward).
//4. A Ghost Racer object has a starting position on the roadway at x=128, y=32.
//5. A Ghost Racer object has a size of 4.0.
//6. A Ghost Racer object has a graphical depth of 0.
//7. A Ghost Racer object starts out with a forward speed of 0.
//8. A Ghost Racer object starts with 10 units of holy water spray.
//9. A Ghost Racer object starts out with 100 hit points (health).
//10. A Ghost Racer object is a “collision avoidance-worthy actor”.

#endif // ACTOR_H_
