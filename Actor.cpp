#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <math.h>
using namespace std;

//=Actor===========================================

void Actor::offScreen() {
    if (getY() < 0 || getX() < 0 || 
        getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) 
    {
        setDead();
    }
}

StudentWorld* Actor::world() const { return sWorld; }

bool   Actor::isCollisionAvoidanceWorthy() const { return collideable; }
void   Actor::setCollideable() { collideable = true; }
int    Actor::randint(int min, int max) { return rand()%((max - min) + 1) + min; }
void   Actor::setVerticalSpeed(double speed) { verticalSpeed = speed; }
double Actor::getVerticalSpeed() const { return verticalSpeed; }
void   Actor::setHorizontalSpeed(double speed) { horizontalSpeed = speed; }
double Actor::getHorizontalSpeed() const { return horizontalSpeed; }
void   Actor::setDead() { dead = true; }
bool   Actor::isDead() const { return dead; }


//=ZombieCab===========================================

int ZombieCab::soundWhenDie() {
    return 0;
}

int ZombieCab::soundWhenHurt() {
    return 0;
}

void ZombieCab::doSomething() {
   setVerticalSpeed(world()->getGhostRacer()->getVerticalSpeed());
}

bool ZombieCab::beSprayedIfAppropriate() {
    return true;
}

//=Pedestrian===========================================

void Pedestrian::moveAndPossiblyPickPlan() {
    if (isDead())
        return;

    int vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    int horiz_speed = getHorizontalSpeed();
    int new_y = getY() + vert_speed;
    int new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    offScreen();

    setMovementPlan(getMovementPlan() - 1);
    if (getMovementPlan() > 0)
        return;
    else {

        int r = randint(-3, 3);
        cout << r << endl;
        if (r == 0)
            r++;

        setHorizontalSpeed(r);

        setMovementPlan(randint(4, 32));

        if (getHorizontalSpeed() < 0)
            setDirection(180);
        else
            setDirection(0);
    }
}




//=HumanPedestrian===========================================

bool HumanPedestrian::beSprayedIfAppropriate() {
    return false;
}

int HumanPedestrian::soundWhenHurt() {
    return 0;
}

int HumanPedestrian::soundWhenDie() {
    return 0;
}

void HumanPedestrian::doSomething() {
    moveAndPossiblyPickPlan();    
}

//=ZombiePedestrian===========================================

int ZombiePedestrian::soundWhenHurt() {
    return 0;
}

int ZombiePedestrian::soundWhenDie() {
    return 0;
}

bool ZombiePedestrian::beSprayedIfAppropriate() {
    return false;
}

void ZombiePedestrian::doSomething() {
    moveAndPossiblyPickPlan();    
}

//=Agent===========================================

int Agent::getHP() const { return health; }

void Agent::setHP(int hp) { health = hp; }

bool Agent::isCollisionAvoidanceWorthy() const {
    return true;
}


bool Agent::beSprayedIfAppropriate() {
    return false;
}

bool Agent::moveRelativeToGhostRacerVerticalSpeed(double dx) {
    return true;
}

bool Agent::takeDamageAndPossiblyDie(int hp) {
    return true;
}


//=GhostRacer===========================================

int GhostRacer::soundWhenDie() {
    return 0;
}

int GhostRacer::soundWhenHurt() {
    return 0;
}
void GhostRacer::increaseSprays(int amt) {};

void GhostRacer::doSomething() 
{
    int key;

    // Math for ghost racer movement
    double max_shift_per_tick = 4.0;
    double direction = (getDirection() * M_PI) / 180.0;
    double delta_x = cos(direction) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();

    if (world()->getKey(key)) 
    {
        switch(key) 
        {
            case KEY_PRESS_LEFT:
                if (getDirection() < 120)
                    setDirection(getDirection() + 10);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 60)
                    setDirection(getDirection() - 10);
                break;
            case KEY_PRESS_SPACE:
                break;
            case KEY_PRESS_UP:
                if (getVerticalSpeed() < 5) 
                {
                    setVerticalSpeed(getVerticalSpeed() + 1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getVerticalSpeed() > 0) 
                {
                    setVerticalSpeed(getVerticalSpeed() - 1);
                }
                break;
            case KEY_PRESS_ENTER:
                break;
            case KEY_PRESS_ESCAPE:
                break;
            case KEY_PRESS_TAB:
                break;
        }

    }
    moveTo(cur_x + delta_x, cur_y);

    if (getX() < (ROAD_CENTER - ROAD_WIDTH/2))
    {
        setDirection(75);
        setHP(getHP() -10);
    }
    
    if (getX() > (ROAD_CENTER + ROAD_WIDTH/2)) 
    {
        setDirection(105);
        setHP(getHP() -10);
    }

}

//=BorderLine===========================================

bool BorderLine::isCollisionAvoidanceWorthy() const {
    return false;
}

bool BorderLine::beSprayedIfAppropriate() {
    return false;
}

bool BorderLine::moveRelativeToGhostRacerVerticalSpeed(double dx) {
    return true;
}

void BorderLine::doSomething() 
{
    double vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = 0;
    
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);
    offScreen();

};

//=GhostRacerActivatedObject===========================================

void GhostRacerActivatedObject::doMove() {
    double vert_speed = getVerticalSpeed() - world()->getGhostRacer()->getVerticalSpeed();
    double horiz_speed = 0;

    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);
    offScreen();
}

//=OilSlick===========================================

void OilSlick::doSomething() {
    doMove();
};

void OilSlick::doActivity(GhostRacer* gr) {};

int OilSlick::getScoreIncrease() const {return 0; };

int OilSlick::getSound() const {return 0; };

bool OilSlick::selfDestructs() const {return false; };

bool OilSlick::isSprayable() const {return false; };

bool OilSlick::beSprayedIfAppropriate() {return false;}

//=HealingGoodie===========================================

void HealingGoodie::doSomething() {
    doMove();
};

void HealingGoodie::doActivity(GhostRacer* gr) {};

int HealingGoodie::getScoreIncrease() const {return 0; };

int HealingGoodie::getSound() const {return 0; };

bool HealingGoodie::selfDestructs() const {return false; };

bool HealingGoodie::isSprayable() const {return false; };

bool HealingGoodie::beSprayedIfAppropriate() {return false;}


//=HolyWaterGoodie===========================================

void HolyWaterGoodie::doSomething() {
    doMove();
};

void HolyWaterGoodie::doActivity(GhostRacer* gr) {};

int HolyWaterGoodie::getScoreIncrease() const {return 0; };

int HolyWaterGoodie::getSound() const {return 0; };

bool HolyWaterGoodie::selfDestructs() const {return false; };

bool HolyWaterGoodie::isSprayable() const {return false; };

bool HolyWaterGoodie::beSprayedIfAppropriate() {return false;}

//=SoulGoodie===========================================

void SoulGoodie::doSomething() {};

void SoulGoodie::doActivity(GhostRacer* gr) {};

int SoulGoodie::getScoreIncrease() const {return 0; };

int SoulGoodie::getSound() const {return 0; };

bool SoulGoodie::selfDestructs() const {return false; };

bool SoulGoodie::isSprayable() const {return false; };

bool SoulGoodie::beSprayedIfAppropriate() {return false;}

