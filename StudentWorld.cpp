#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath) { return new StudentWorld(assetPath); }

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {
    player = nullptr;
    soulsSaved = 0;
}

// Check where two actors collide
bool StudentWorld::collide(const Actor* a, const Actor* b) const {
        double delta_x = abs(a->getX() - b->getX());
        double delta_y = abs(a->getY() - b->getY());

        double rad_sum = a->getRadius() + b->getRadius();

        return (delta_x < rad_sum*0.25 && delta_y < rad_sum*0.6);
}

// Check if any actors collide
Actor* StudentWorld::checkCollide(Actor* a) {
    for (auto actor = actors.begin(); actor != actors.end(); actor++) {
        if ((*actor)->getCollideable() && collide(a, *actor) && a != *actor) {
            return *actor;
        }
    }

    return nullptr;
}

// Set the number of souls saved
void StudentWorld::setSoulsSaved(int s) { soulsSaved = s; }

// Get the number of souls saved
int StudentWorld::getSoulsSaved() { return soulsSaved; }

// Increase souls saved by one
void StudentWorld::increaseSouls() { soulsSaved++; }

int StudentWorld::checkLaneTop(int lane, int current_y) 
{

    // Calculating edges
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // Calculating where to check for actors
    int right;
    int left;
    if (lane == 1) 
    {
        right = LEFT_EDGE + ROAD_WIDTH/3;
        left = LEFT_EDGE;
    } 
    else if (lane == 2) 
    {
        right = RIGHT_EDGE - ROAD_WIDTH/3;
        left = LEFT_EDGE + ROAD_WIDTH/3;
    } 
    else if (lane == 3) 
    {
        right = RIGHT_EDGE;
        left = RIGHT_EDGE - ROAD_WIDTH/3;
    }

    Actor* a = nullptr;
    
    // Iterating over actors to find the one closest to the top
    for (auto actor = actors.begin(); actor != actors.end(); actor++) 
    {
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

    // If there is no actor return nullptr, or the closest actor a
    if (a == nullptr)
        return -5000;
    else
        return a->getY();
}

int StudentWorld::checkLaneBottom(int lane, int current_y) 
{
    // Calculating edges
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // Calculating where to check for actors
    int right;
    int left;
    if (lane == 1) 
    {
        right = LEFT_EDGE + ROAD_WIDTH/3;
        left = LEFT_EDGE;
    } else if (lane == 2) 
    {
        right = RIGHT_EDGE - ROAD_WIDTH/3;
        left = LEFT_EDGE + ROAD_WIDTH/3;
    } else if (lane == 3) {
        right = RIGHT_EDGE;
        left = RIGHT_EDGE - ROAD_WIDTH/3;
    }

    Actor* a = nullptr;

    // If the player is in the lane, don't spawn here
    if (player->getX() <= right && player->getX() >= left)
        return player->getY();

    // Iterating over actors to find the closest one to the bottom
    for (auto actor = actors.begin(); actor != actors.end(); actor++) 
    {
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

    // If there is no actor return nullptr, or the closest actor a
    if (a == nullptr)
        return 5000;
    else 
        return a->getY();
}
    

StudentWorld::~StudentWorld() { cleanUp(); }

    //Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp)
int StudentWorld::init() 
{
    // Initializing player
    player = new GhostRacer(this, 128.0, 32.0);

    // Initializing level details
    soulsSaved = 0;
    bonusPoints = 5000;
    lastBorderY = 256;

    // Calculating where to place borders
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // Drawing the initial white borders 
    double M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int i = 0; i <= M; i++) 
    {
        actors.push_back(new BorderLine(this, LEFT_EDGE + ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), false));
        actors.push_back(new BorderLine(this, RIGHT_EDGE - ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), false));
    }
    
    // Drawing the initial yellow borders 
    double N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int i = 0; i <= N; i++) 
    {
        actors.push_back(new BorderLine(this, LEFT_EDGE, i * SPRITE_HEIGHT, true));
        actors.push_back(new BorderLine(this, RIGHT_EDGE, i * SPRITE_HEIGHT, true));
    }

    return GWSTATUS_CONTINUE_GAME;
}

// Returns the current bonus
int StudentWorld::getBonus() { return bonusPoints; }

// decrements bonus
void StudentWorld::subBonus() { bonusPoints--; }

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
    oss << setw(9) << "Bonus: " << getBonus();
    string s = oss.str();
    setGameStatText(s);

    // Getting current level
    int level = getLevel();

    // Subtraching bonus
    subBonus();

    // Math for getting where edges are
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    // Trying to add zombie cabs
    if (tryAdd(max(100 - (level * 10), 20))) {
        if (checkLaneBottom(1, SPRITE_HEIGHT) > VIEW_HEIGHT/3) 
            addActor(new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH/3, SPRITE_HEIGHT / 2));
        else if (checkLaneTop(1, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3)
            addActor(new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH/3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));

        else if (checkLaneBottom(2, SPRITE_HEIGHT) > VIEW_HEIGHT/3)
            addActor(new ZombieCab(this, ROAD_CENTER, SPRITE_HEIGHT / 2));
        else if (checkLaneTop(2, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3)
           addActor(new ZombieCab(this, ROAD_CENTER, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));

        else if (checkLaneBottom(3, SPRITE_HEIGHT) > VIEW_HEIGHT/3)
            addActor(new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH/3, SPRITE_HEIGHT / 2));
        else if (checkLaneTop(3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)) < (VIEW_HEIGHT*2)/3)
            addActor(new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH/3, VIEW_HEIGHT - (SPRITE_HEIGHT/2)));
    }

    // Trying to add an OilSlick
    if (tryAdd(max(150 - (level * 10), 40)))
        addActor(new OilSlick(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    // Trying to add a human ped
    if (tryAdd(max(200 - (level * 10), 30)))
        addActor(new HumanPedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    // Trying to add a zombie ped
    if (tryAdd(max(100 - (level * 10), 20)))
        addActor(new ZombiePedestrian(this, rand()%(VIEW_WIDTH + 1), VIEW_HEIGHT));

    // Trying to add a holy water goodie
    if (tryAdd(100 + (10 * level)))
        addActor(new HolyWaterGoodie(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    // Trying to add a soul goodie
    if (tryAdd(100))
        addActor(new SoulGoodie(this, randint(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    // Killing player if health is 0
    if (player->getHP() <= 0) 
    {
        player->setDead();
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    // Checking if level is done
    if (getSoulsSaved() == ((getLevel()*2) + 5)) {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(getBonus());
        return GWSTATUS_FINISHED_LEVEL;
    }

    // Telling player to do something
    player->doSomething();

    // Calculating next border's y position
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - lastBorderY;

    // Rendering new yellow lines
    if (delta_y >= SPRITE_HEIGHT) 
    {
        addActor(new BorderLine(this, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, true));
        addActor(new BorderLine(this, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, true));
    }

    // Rendering new white lines
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
            // Telling each actor to do something
            ptr->doSomething();
            actor++;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() 
{
    // Deleting player
    delete player;

    // Deleting each actor in vector
    for (auto it = actors.begin(); it != actors.end();) 
    {
        delete *it;
        it = actors.erase(it);
    }
}
