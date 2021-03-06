#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <random>
#include <math.h>


class Actor;
class GhostRacer;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    int init();
    int move();
    void cleanUp();
    ~StudentWorld();
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
    int getBonus();
    bool collide(const Actor* a, const Actor* b) const;
    Actor* checkCollide(Actor* a);
    void setSoulsSaved(int s);
    int getSoulsSaved();
    void increaseSouls();
    int checkLaneBottom(int lane, int current_y);
    int checkLaneTop(int lane, int current_y);
    void subBonus();
    

private:
    GhostRacer* player;
    std::vector<Actor*> actors; 
    double lastBorderY;
    int level;
    int soulsSaved;
    int bonusPoints;
};

#endif // STUDENTWORLD_H_
