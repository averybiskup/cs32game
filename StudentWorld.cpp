#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {
    player = nullptr;
}


StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init() {
    player = new GhostRacer(IID_GHOST_RACER, 128, 32, 90, 4, 0, this);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    player->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp() {
    delete player;
    for (auto actor : liveActors) {
        delete actor;
    }
}
