#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
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


    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    double M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int i = 0; i <= M; i++) {
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), 0, 2, 1, this, player));
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), 0, 2, 1, this, player));
    }
    lastBorderY = actors.back()->getY();


    double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int i = 0; i <= N; i++) {
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT, 0, 2, 1, this, player));
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT, 0, 2, 1, this, player));
    }



    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
 
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double prev_border_y;


    player->doSomething();
    std::vector<int> deadActors;

    int i = 0;
    for (auto actor: actors) {
        if (actor->checkAlive() == false) {
            deadActors.push_back(i);
        }
        i++;

        double dx = abs(player->getX() - actor->getX());
        double dy = abs(player->getY() - actor->getY());

        double radius_sum = player->getRadius() + actor->getRadius(); 

        if (actor->collideable()) {
            if (dx < radius_sum*0.25 && dy < radius_sum*0.6) {
                actor->hit();
            }
        }


        actor->doSomething();
    }


    

    double delta_y = new_border_y - lastBorderY;
    if (delta_y >= 4*SPRITE_HEIGHT) {
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, 0, 2, 1, this, player));
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, 0, 2, 1, this, player));
        lastBorderY = 256;
    }

    if (delta_y >= SPRITE_HEIGHT) {
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, 0, 2, 1, this, player));
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, 0, 2, 1, this, player));
    }

    lastBorderY += (-4 - player->getSpeed());

    
    // Deleting the dead actors from vector and memory
    for (auto i: deadActors) {
        delete actors[i];
        actors.erase(actors.begin() + i);
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    delete player;
    for (auto actor : actors) {
        delete actor;
    }
}
