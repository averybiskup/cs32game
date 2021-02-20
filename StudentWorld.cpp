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


StudentWorld::~StudentWorld() 
{
    cleanUp();
}

int StudentWorld::init() 
{
    // Initializing player
    player = new GhostRacer(IID_GHOST_RACER, 128.0, 32.0, 90, 4.0, 0, this);

    lastBorderY = 256;

    // Calculating where to place borders
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;


    // Drawing the initial borders 
    double M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int i = 0; i <= M; i++) {
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), 0, 2, 1, this, player));
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), 0, 2, 1, this, player));
    }

    double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int i = 0; i <= N; i++) {
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT, 0, 2, 1, this, player));
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT, 0, 2, 1, this, player));
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() 
{

    // Killing player if health is 0
    if (player->getHp() <= 0) 
    {
        player->kill();
        return GWSTATUS_PLAYER_DIED;
    }

    player->doSomething();

    // Calculating next border's y position
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - lastBorderY;

    // Rendering a new yellow line
    if (delta_y >= SPRITE_HEIGHT) 
    {
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, 0, 2, 1, this, player));
        actors.push_back(new YellowBorder(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, 0, 2, 1, this, player));
    }

    // Rendering a new white line 
    if (delta_y >= 4*SPRITE_HEIGHT) 
    {
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, 0, 2, 1, this, player));
        actors.push_back(new WhiteBorder(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, 0, 2, 1, this, player));
        lastBorderY = 256;
    }

    // Keeping track of the last white line's y position
    lastBorderY -= (4 + player->getSpeed());

    std::vector<int> deadActors;
    // Iterating over actors
    int i = 0;
    for (auto actor: actors) {


        if (actor->checkAlive() == false) 
            deadActors.push_back(i);
        else 
            actor->doSomething();

        i++;

        // Collision calcuation
        double dx = abs(player->getX() - actor->getX());
        double dy = abs(player->getY() - actor->getY());
        double radius_sum = player->getRadius() + actor->getRadius(); 

        // Checking for collisions between player and any actor
        // Assuming that actor is collideable
        if (actor->collideable()) {
            if (dx < radius_sum*0.25 && dy < radius_sum*0.6) {
                actor->hit();
            }
        }

    }


    
    // Deleting the dead actors from vector and memory
    for (auto i: deadActors) 
    {
        delete actors[i];
        actors.erase(actors.begin() + i);
    }

    deadActors.clear();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() 
{
    delete player;
    for (auto it = actors.begin(); it != actors.end();) {
        delete *it;
        it = actors.erase(it);
    }
}
