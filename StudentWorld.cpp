#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
#include <random>
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

    //Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp)
int StudentWorld::init() 
{
    // Initializing player
    player = new GhostRacer(this, 128.0, 32.0);

    lastBorderY = 256;

    // Calculating where to place borders
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // Drawing the initial borders 
    double M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int i = 0; i <= M; i++) 
    {
        actors.push_back(new BorderLine(this, LEFT_EDGE + ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), false));
        actors.push_back(new BorderLine(this, RIGHT_EDGE - ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), false));
    }

    double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int i = 0; i <= N; i++) 
    {
        actors.push_back(new BorderLine(this, LEFT_EDGE, i * SPRITE_HEIGHT, true));
        actors.push_back(new BorderLine(this, RIGHT_EDGE, i * SPRITE_HEIGHT, true));
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() 
{
    int level = getLevel();
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    if (tryAdd(max(100 - (level * 10), 20)))
        actors.push_back(new ZombieCab(this, 100, 100));
    
    if (tryAdd(max(150 - (level * 10), 40)))
        actors.push_back(new OilSlick(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    if (tryAdd(max(200 - (level * 10), 30)))
        actors.push_back(new HumanPedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    if (tryAdd(max(100 - (level * 10), 20)))
        actors.push_back(new ZombiePedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    if (tryAdd(100 + (10 * level)))
        actors.push_back(new HolyWaterGoodie(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    // Killing player if health is 0
    if (player->getHP() <= 0) 
    {
        player->setDead();
        return GWSTATUS_PLAYER_DIED;
    }

    player->doSomething();

    // Calculating next border's y position
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - lastBorderY;

    // Rendering a new yellow line
    if (delta_y >= SPRITE_HEIGHT) 
    {
        actors.push_back(new BorderLine(this, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, true));
        actors.push_back(new BorderLine(this, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, true));
    }

    // Rendering a new white line 
    if (delta_y >= 4*SPRITE_HEIGHT) 
    {
        actors.push_back(new BorderLine(this, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, false));
        actors.push_back(new BorderLine(this, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, false));
        lastBorderY = VIEW_WIDTH;
    }

    // Keeping track of the last white line's y position
    lastBorderY -= (4 + player->getVerticalSpeed());

    // Iterating over actors
    for (auto actor = actors.begin(); actor != actors.end();) 
    {
        Actor* ptr = (*actor);

        // Checking that actor is alive
        if (ptr->isDead()) 
        {
            delete ptr;
            actor = actors.erase(actor);
        } else 
        {
            ptr->doSomething();


            double dx = abs(player->getX() - ptr->getX());
            double dy = abs(player->getY() - ptr->getY());
            double radius_sum = player->getRadius() + ptr->getRadius(); 

            actor++;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() 
{
    delete player;
    for (auto it = actors.begin(); it != actors.end();) 
    {
        delete *it;
        it = actors.erase(it);
    }
}
