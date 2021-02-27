#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <random>


class Actor;
class GhostRacer;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    GhostRacer* getGhostRacer() {
        return player;
    };
    int randint(int min, int max) {
        return rand()%((max - min) + 1) + min;
    };
    bool tryAdd(int odds) {
        int ran = rand()%(odds + 1);
        return ran == 0;
    };
    void nextLevel() {
        level++;
    };
    void addActor(Actor* a) {
        actors.push_back(a);
    }
     

private:
    GhostRacer* player;
    std::vector<Actor*> actors; 
    double lastBorderY;
    int level;
};

#endif // STUDENTWORLD_H_
