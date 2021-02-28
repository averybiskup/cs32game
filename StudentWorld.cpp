#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {
    player = nullptr;
    soulsSaved = 0;
}

bool StudentWorld::collide(const Actor* a, const Actor* b) const {
        double delta_x = abs(a->getX() - b->getX());
        double delta_y = abs(a->getY() - b->getY());

        double rad_sum = a->getRadius() + b->getRadius();

        return (delta_x < rad_sum*0.25 && delta_y < rad_sum*0.6);
}

Actor* StudentWorld::checkCollide(Actor* a) {
    for (auto actor = actors.begin(); actor != actors.end(); actor++) {
        if ((*actor)->getCollideable() && collide(a, *actor) && a != *actor) {
            return *actor;
        }
    }

    return nullptr;
}

void StudentWorld::setSoulsSaved(int s) {
    soulsSaved = s;
}

int StudentWorld::getSoulsSaved() {
    return soulsSaved;
}

void StudentWorld::increaseSouls() {
    soulsSaved++;
}
int StudentWorld::checkLaneTop(int lane, int current_y) {
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    int right;
    int left;
    if (lane == 1) {
        right = LEFT_EDGE + ROAD_WIDTH/3;
        left = LEFT_EDGE;
    } else if (lane == 2) {
        right = RIGHT_EDGE - ROAD_WIDTH/3;
        left = LEFT_EDGE + ROAD_WIDTH/3;
    } else if (lane == 3) {
        right = RIGHT_EDGE;
        left = RIGHT_EDGE - ROAD_WIDTH/3;
    }

    Actor* a = nullptr;

    

    for (auto actor = actors.begin(); actor != actors.end(); actor++) {
        if (((*actor)->isCollisionAvoidanceWorthy() 
             && (*actor)->getX() <= right
             && (*actor)->getX() >= left
             && ((*actor)->getY() < current_y)
             && (a == nullptr || a->getY() < (*actor)->getY())
            ))
        {
            a = (*actor); 
        }
    }


    if (a == nullptr)
        return -5000;
    else
        return a->getY();
}

int StudentWorld::checkLaneBottom(int lane, int current_y) {
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    int right;
    int left;
    if (lane == 1) {
        right = LEFT_EDGE + ROAD_WIDTH/3;
        left = LEFT_EDGE;
    } else if (lane == 2) {
        right = RIGHT_EDGE - ROAD_WIDTH/3;
        left = LEFT_EDGE + ROAD_WIDTH/3;
    } else if (lane == 3) {
        right = RIGHT_EDGE;
        left = RIGHT_EDGE - ROAD_WIDTH/3;
    }

    Actor* a = nullptr;

    if (player->getX() <= right && player->getX() >= left) {
        return player->getY();
    }

    for (auto actor = actors.begin(); actor != actors.end(); actor++) {
        if (((*actor)->isCollisionAvoidanceWorthy() 
             && (*actor)->getX() <= right
             && (*actor)->getX() >= left
             && ((*actor)->getY() > current_y)
             && (a == nullptr || a->getY() > (*actor)->getY())
            ))
        {
            a = (*actor); 
        }
    }
    //if (a != nullptr)
       // cout << lane << " - " << a->getY() << endl;

    if (a == nullptr) {
        //cout << lane << " - " << 256 << endl;
        return 5000;
    } else {
        return a->getY();
    }

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
    soulsSaved = 0;

    lastBorderY = 256;

    // Calculating where to place borders
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // rawing the initial borders 
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

    // Creating gametext
    ostringstream oss;
    oss << "Score: " << getScore();
    oss << setw(7) << "Lvl: " << getLevel();
    oss << setw(14) << "Souls2Save: " << (2*getLevel() + 5) - getSoulsSaved();
    oss << setw(9) << "Lives: " << getLives();
    oss << setw(10) << "Health: " << player->getHP();
    oss << setw(10) << "Sprays: " << player->getSprays();
    oss << setw(9) << "Bonus: " << player->getBonus();
    string s = oss.str();
    setGameStatText(s);

    int level = getLevel();
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    if (tryAdd(max(100 - (level * 10), 20))) {
        if (checkLaneBottom(1, SPRITE_HEIGHT) > VIEW_HEIGHT/3) {
            addActor(new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH/3, SPRITE_HEIGHT / 2));
        } else if (checkLaneTop(1, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3) {
            addActor(new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH/3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));
        } else if (checkLaneBottom(2, SPRITE_HEIGHT) > VIEW_HEIGHT/3) {
            addActor(new ZombieCab(this, ROAD_CENTER, SPRITE_HEIGHT / 2));
        } else if (checkLaneTop(2, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3) {
            addActor(new ZombieCab(this, ROAD_CENTER, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));
        } else if (checkLaneBottom(3, SPRITE_HEIGHT) > VIEW_HEIGHT/3) {
            addActor(new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH/3, SPRITE_HEIGHT / 2));
        } else if (checkLaneTop(3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3) {
            addActor(new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH/3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));
        }
    }



    if (tryAdd(max(150 - (level * 10), 40)))
        addActor(new OilSlick(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    if (tryAdd(max(200 - (level * 10), 30)))
        addActor(new HumanPedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    if (tryAdd(max(100 - (level * 10), 20)))
        addActor(new ZombiePedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    if (tryAdd(100 + (10 * level)))
        addActor(new HolyWaterGoodie(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    if (tryAdd(100))
        addActor(new SoulGoodie(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    // Killing player if health is 0
    if (player->getHP() <= 0) 
    {
        player->setDead();
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (getSoulsSaved() == ((getLevel()*2) + 5)) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    player->doSomething();

    // Calculating next border's y position
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - lastBorderY;

    // Rendering a new yellow line
    if (delta_y >= SPRITE_HEIGHT) 
    {
        addActor(new BorderLine(this, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, true));
        addActor(new BorderLine(this, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, true));
    }

    // Rendering a new white line 
    if (delta_y >= 4*SPRITE_HEIGHT) 
    {
        addActor(new BorderLine(this, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, false));
        addActor(new BorderLine(this, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, false));
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
