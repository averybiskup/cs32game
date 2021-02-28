#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <math.h>
using namespace std;

//=Actor===========================================

// Calculate if the object is off the screen, and kills it if it is
void Actor::offScreen() {
    if (getY() < 0 || getX() < 0 || 
        getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) 
    {
        setDead();
    }
}

StudentWorld* Actor::world() const { return sWorld; }

// Simple methods for Actor
void   Actor::setCollideable() { collideable = true; }
bool   Actor::getCollideable() { return collideable; }
int    Actor::randint(int min, int max) { return rand()%((max - min) + 1) + min; }
void   Actor::setVerticalSpeed(double speed) { verticalSpeed = speed; }
double Actor::getVerticalSpeed() const { return verticalSpeed; }
void   Actor::setHorizontalSpeed(double speed) { horizontalSpeed = speed; }
double Actor::getHorizontalSpeed() const { return horizontalSpeed; }
void   Actor::setDead() { dead = true; }
bool   Actor::isDead() const { return dead; }


//=ZombieCab===========================================

ZombieCab::ZombieCab(StudentWorld* sw, double x, double y)
    :Agent(sw, IID_ZOMBIE_CAB, x, y, 4, 90, 3)
{
    setCollideable();   
    int vs;
    if (y > VIEW_HEIGHT/2)
        vs = world()->getGhostRacer()->getVerticalSpeed() - randint(2, 4);
    else
        vs = randint(2, 4) + world()->getGhostRacer()->getVerticalSpeed();
    setVerticalSpeed(vs);
    damaged = false;
};

int ZombieCab::getDamaged() { return damaged; }

void ZombieCab::setDamaged() { damaged = true; }

void ZombieCab::doSomething() 
{
    // Check if dead
    if (isDead())
       return;

    if (!getDamaged() && world()->collide(this, world()->getGhostRacer())) {
        setDamaged();
        world()->playSound(SOUND_VEHICLE_CRASH);
        world()->getGhostRacer()->setHP(world()->getGhostRacer()->getHP() - 20); 
        if (getX() <= world()->getGhostRacer()->getX()) {
            setHorizontalSpeed(-5);
            setDirection(randint(0, 19) + 120);
        } else if (getX() > world()->getGhostRacer()->getX()) {
            setHorizontalSpeed(5);
            setDirection(randint(0, 19) + 60);
        }
    }

    int vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    int horiz_speed = getHorizontalSpeed();

    int new_x = getX() + horiz_speed;
    int new_y = getY() + vert_speed;

    moveTo(new_x, new_y); 

    // Check that object is still on screen
    offScreen();

    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

    int lane = 1;

    if (getX() <= LEFT_EDGE + ROAD_WIDTH/3 && getX() >= LEFT_EDGE)
        lane = 1;
    else if (getX() >= LEFT_EDGE + ROAD_WIDTH/3 && getX() <= RIGHT_EDGE - ROAD_WIDTH/3)
        lane = 2;
    else if  (getX() <= RIGHT_EDGE && getX() >= RIGHT_EDGE - ROAD_WIDTH/3)
        lane = 3;

    if ((world()->checkLaneBottom(lane, getY()) - getY()) < 96 && getVerticalSpeed() > world()->getGhostRacer()->getVerticalSpeed()) 
    {
        setVerticalSpeed(getVerticalSpeed() - 0.5);
        return;
    }

    if (getVerticalSpeed() <= world()->getGhostRacer()->getVerticalSpeed() && (getY() - world()->checkLaneTop(lane, getY())) < 96) 
    {
        setVerticalSpeed(getVerticalSpeed() + 0.5);
        return;
    }

    setMovementPlan(getMovementPlan() - 1);

    if (getMovementPlan() > 0)
        return;
    else  
    {
        setMovementPlan(randint(4, 32));
        setVerticalSpeed(randint(-2, 2) + getVerticalSpeed());
    }

}
// Return true and do logic because this class can be sprayed
bool ZombieCab::beSprayedIfAppropriate() 
{
    setHP(getHP() - 1);
    if (getHP() <= 0) 
    {
        setDead();
        if (randint(1, 5) == 3) 
            world()->addActor(new OilSlick(world(), getX(), getY()));

        world()->increaseScore(200);
        world()->playSound(SOUND_VEHICLE_DIE);
        return true;
    } else 
    {
        world()->playSound(SOUND_VEHICLE_HURT);
    }

    return true;
}

//=Pedestrian===========================================

Pedestrian::Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size)
        : Agent(sw, imageID, x, y, size, 0, 2)
    {
        setCollideable();   
        setVerticalSpeed(-4);
    };

void Pedestrian::moveAndPossiblyPickPlan() 
{
    // Check if dead
    if (isDead())
        return;

    int vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    int horiz_speed = getHorizontalSpeed();
    int new_y = getY() + vert_speed;
    int new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);

    // Check if object is off the screen
    offScreen();

    setMovementPlan(getMovementPlan() - 1);

    if (getMovementPlan() > 0)
        return;
    else 
    {
        int r = 0;
        while (r == 0)
            r = randint(-3, 3);

        setHorizontalSpeed(r);

        setMovementPlan(randint(4, 32));

        if (getHorizontalSpeed() < 0)
            setDirection(180);
        else
            setDirection(0);
    }
}




//=HumanPedestrian===========================================

HumanPedestrian::HumanPedestrian(StudentWorld* sw, double x, double y)
    : Pedestrian(sw, IID_HUMAN_PED, x, y, 2) {};

// Return true and do logic because this class can be sprayed
bool HumanPedestrian::beSprayedIfAppropriate() 
{

    setHorizontalSpeed(getHorizontalSpeed() * -1); 

    setDirection(getDirection() + 180);
    world()->playSound(SOUND_PED_HURT);

    return true;
}

void HumanPedestrian::doSomething() 
{
    // Check if dead
    if (isDead()) 
        return;

    if (world()->collide(world()->getGhostRacer(), this)) 
    {
        setDead();
        world()->getGhostRacer()->setHP(0);
        return;
    }

    moveAndPossiblyPickPlan();    
}

//=ZombiePedestrian===========================================

ZombiePedestrian::ZombiePedestrian(StudentWorld* sw, double x, double y)
    : Pedestrian(sw, IID_ZOMBIE_PED, x, y, 3) {};

// Return true and do logic because this class can be sprayed
bool ZombiePedestrian::beSprayedIfAppropriate() 
{
    setHP(getHP() - 1);
    
    if (getHP() == 0) 
    {
        setDead();

        if (!world()->collide(this, world()->getGhostRacer()))
            if (randint(1, 5) == 3)
                world()->addActor(new HealingGoodie(world(), getX(), getY()));
        
        world()->increaseScore(150);
        world()->playSound(SOUND_PED_DIE);
    } else 
    {
        world()->playSound(SOUND_PED_HURT);
    }


    return true;
}

void ZombiePedestrian::setTicksGrunt(int t) { ticksGrunt = t; }

int ZombiePedestrian::getTicksGrunt() { return ticksGrunt; }

void ZombiePedestrian::doSomething() 
{
    // Check if dead
    if (isDead())
        return;

    if (world()->collide(this, world()->getGhostRacer())) 
    {
        world()->playSound(SOUND_PED_DIE);
        setDead();
        world()->getGhostRacer()->setHP(world()->getGhostRacer()->getHP() - 5);
        world()->increaseScore(150);
        return;
    }

    moveAndPossiblyPickPlan();    

    if (abs(world()->getGhostRacer()->getX() - getX()) <= 30
        && world()->getGhostRacer()->getY() < getY()) 
    {

        setTicksGrunt(getTicksGrunt() - 1);

        if (getTicksGrunt() <= 0) 
        {
            world()->playSound(SOUND_ZOMBIE_ATTACK);
            setTicksGrunt(20);
        }

        setDirection(270);

        if (world()->getGhostRacer()->getX() < getX()) 
            setHorizontalSpeed(-1);
        else if (world()->getGhostRacer()->getX() > getX()) 
            setHorizontalSpeed(1);
        else 
            setHorizontalSpeed(0);
    }

}

//=Agent===========================================
Agent::Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp)
        : Actor(sw, imageID, x, y, size, dir, 0) 
{
    health = hp;
    setCollideable();
    movementPlan = 0;
};

int Agent::getHP() const { return health; }

void Agent::setHP(int hp) { health = hp; }

bool Agent::isCollisionAvoidanceWorthy() const { return true; }


//=GhostRacer===========================================

// Constructor for GhostRacer
GhostRacer::GhostRacer(StudentWorld* sw, double x, double y)
    : Agent(sw, IID_GHOST_RACER, x, y, 4, 90, 100)
{
    spraysLeft = 10;
};

// Return false because this class cannot be sprayed
bool GhostRacer::beSprayedIfAppropriate() { return false; }

// Get number of sprays left
int GhostRacer::getSprays() { return spraysLeft; }

// Set number of sprays left
void GhostRacer::setSprays(int s) { spraysLeft = s; }

// Do this function every tick of the game
void GhostRacer::doSomething() 
{
    // Check if dead
    if (isDead())
        return;

    if (getHP() == 0) 
    {
        setDead();
        world()->playSound(SOUND_PLAYER_DIE);
    }

    if (getX() < (ROAD_CENTER - ROAD_WIDTH/2))
    {
        if (getDirection() > 90) 
        {
            world()->playSound(SOUND_VEHICLE_CRASH);
            moveTo((ROAD_CENTER - ROAD_WIDTH/2), getY());
            setDirection(82);
            setHP(getHP() -10);
        }
    }
    
    if (getX() > (ROAD_CENTER + ROAD_WIDTH/2)) 
    {
        if (getDirection() < 90) 
        {
            world()->playSound(SOUND_VEHICLE_CRASH);
            moveTo((ROAD_CENTER + ROAD_WIDTH/2), getY());
            setDirection(98);
            setHP(getHP() -10);
        }
    }
    
    int key;

    if (world()->getKey(key)) 
    {
        switch(key) 
        {
            case KEY_PRESS_LEFT:
                if (getDirection() < 114)
                    setDirection(getDirection() + 8);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 66)
                    setDirection(getDirection() - 8);
                break;
            case KEY_PRESS_SPACE:
                if (getSprays() != 0) {
                    world()->addActor(new Spray(world(), getX(), getY() + SPRITE_HEIGHT, getDirection())); 
                    setSprays(getSprays() - 1);
                    world()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_UP:
                if (getVerticalSpeed() < 5) 
                    setVerticalSpeed(getVerticalSpeed() + 1);
                break;
            case KEY_PRESS_DOWN:
                if (getVerticalSpeed() > -1) 
                    setVerticalSpeed(getVerticalSpeed() - 1);
                break;
        }
    }

    // Math for ghost racer movement
    double max_shift_per_tick = 4.0;
    double direction = (getDirection() * M_PI) / 180.0;
    double delta_x = cos(direction) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();

    moveTo(cur_x + delta_x, cur_y);

    

}

//=BorderLine===========================================

bool BorderLine::isCollisionAvoidanceWorthy() const { return false; }

// Return false because this class cannot be sprayed
bool BorderLine::beSprayedIfAppropriate() { return false; }

void BorderLine::doSomething() 
{
    // Check if dead
    if (isDead())
        return;

    // Do math for deciding where to move to
    double vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = 0;
    
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);

    // Check that object is still on screen
    offScreen();
};

//=GhostRacerActivatedObject===========================================
GhostRacerActivatedObject::GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir)
    : Actor(sw, imageID, x, y, size, dir, 2)
{
    setVerticalSpeed(-4);
    alreadyCollided = false; 
};

bool GhostRacerActivatedObject::isCollisionAvoidanceWorthy() const { return false; }

void GhostRacerActivatedObject::doMove() 
{
    double vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = 0;

    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);

    // Check that object is still on screen
    offScreen();
}

void GhostRacerActivatedObject::setCollided() { alreadyCollided = true; }

bool GhostRacerActivatedObject::getCollided() { return alreadyCollided; }

void GhostRacerActivatedObject::checkCollide(GhostRacer* gr, int sound) 
{
    if (world()->collide(gr, this)) {
        doActivity(gr);
        setCollided();
        world()->playSound(sound);
    }
}

//=OilSlick===========================================

OilSlick::OilSlick(StudentWorld* sw, double x, double y) 
    : GhostRacerActivatedObject(sw, IID_OIL_SLICK, x, y, randint(2, 5), 0) {};

void OilSlick::doSomething() 
{
    // Check if dead
    if (isDead())
        return;
    doMove();
    checkCollide(world()->getGhostRacer(), SOUND_OIL_SLICK);
};

void OilSlick::doActivity(GhostRacer* gr) 
{
    int or10 = 0;
    while (or10 == 0)
        or10 = randint(-1, 1);

    gr->setDirection(gr->getDirection() + (randint(5, 20) * or10));
    if (gr->getDirection() < 60)
        gr->setDirection(60);
    else if (gr->getDirection() > 120)
        gr->setDirection(120);
};

// Return false because this class cannot be sprayed
bool OilSlick::beSprayedIfAppropriate() { return false; }

//=HealingGoodie===========================================
HealingGoodie::HealingGoodie(StudentWorld* sw, double x, double y)
    : GhostRacerActivatedObject(sw, IID_HEAL_GOODIE, x, y, 1, 0)
{
    setCollideable();
};
void HealingGoodie::doSomething() 
{
    // Check if dead
    if (isDead())
        return;
    doMove();
    checkCollide(world()->getGhostRacer(), SOUND_GOT_GOODIE);
};

void HealingGoodie::doActivity(GhostRacer* gr) 
{
    setDead();
    if (gr->getHP() >= 90)
        gr->setHP(100);
    else if (gr->getHP() < 90)
        gr->setHP(gr->getHP() + 10);

    world()->increaseScore(250);

};

// Return true and do logic because this class can be sprayed
bool HealingGoodie::beSprayedIfAppropriate() 
{
    setDead();
    return true;
}

//=HolyWaterGoodie===========================================

HolyWaterGoodie::HolyWaterGoodie(StudentWorld* sw, double x, double y)
    : GhostRacerActivatedObject(sw, IID_HOLY_WATER_GOODIE, x, y, 2, 90)
{
    setCollideable();
};

void HolyWaterGoodie::doSomething() 
{
    // Check if dead
    if (isDead())
        return;
    doMove();
    checkCollide(world()->getGhostRacer(), SOUND_GOT_GOODIE);
};

void HolyWaterGoodie::doActivity(GhostRacer* gr) 
{
    world()->increaseScore(50);
    gr->setSprays(gr->getSprays() + 10); 
    setDead();
};

// Return true and do logic because this class can be sprayed
bool HolyWaterGoodie::beSprayedIfAppropriate() 
{
    setDead();
    return true;
}

//=SoulGoodie===========================================

SoulGoodie::SoulGoodie(StudentWorld* sw, double x, double y)
    : GhostRacerActivatedObject(sw, IID_SOUL_GOODIE, x, y, 4, 0) {};

void SoulGoodie::doSomething() 
{
    // Check if dead
    if (isDead())
        return;

    doMove();
    checkCollide(world()->getGhostRacer(), SOUND_GOT_SOUL);
    setDirection(getDirection() + 10);
};

void SoulGoodie::doActivity(GhostRacer* gr) 
{
    world()->increaseSouls();
    setDead();
    world()->increaseScore(100);
};

// Return false because this class cannot be sprayed
bool SoulGoodie::beSprayedIfAppropriate() { return false; }

//=Spray===========================================

Spray::Spray(StudentWorld* sw, double x, double y, int dir)
    : Actor(sw, IID_HOLY_WATER_PROJECTILE, x, y, 1, dir, 1)
{
    maxTravelDistance = 160;
    travelled = 0;
};
bool Spray::isCollisionAvoidanceWorthy() const { return false; }

void Spray::doSomething() 
{
    // Check if dead
    if (isDead())
        return;    
       
    Actor* target; 
    target = world()->checkCollide(this);

    if (target != nullptr)
        if (target->beSprayedIfAppropriate())
            setDead();
   
    moveForward(SPRITE_HEIGHT);

    // Check that object is still on screen
    offScreen();

    if (travelled >= maxTravelDistance)
        setDead();

    increaseTravel();
};

void Spray::increaseTravel() { travelled += SPRITE_HEIGHT; }



