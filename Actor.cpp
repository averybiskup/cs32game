#include "Actor.h"
#include <iostream>
#include <tgmath.h>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


#define PI 3.14159265

void GhostRacer::doSomething() 
{
    int key;

    // Math for ghost racer movement
    double max_shift_per_tick = 4.0;
    double direction = (getDirection() * PI) / 180.0;
    double delta_x = cos(direction) * max_shift_per_tick;
    double cur_x = getX();
    double cur_y = getY();

    if (getWorld()->getKey(key)) 
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
                if (getSpeed() < 5) 
                {
                    setSpeed(1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getSpeed() > 0) 
                {
                    setSpeed(-1);
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
}

double GhostRacer::getSpeed() 
{
    return speed;
}

void GhostRacer::setSpeed(int new_speed) 
{
    speed += new_speed;
}

void Healthy::setHp(int damage)
{
    hp = damage;
    cout << hp << endl;
}

double Healthy::getHp() {
    return hp;
}


void BorderLine::doSomething() 
{
    double vert_speed = vertSpeed - player->getSpeed();
    double horiz_speed = horzSpeed;
    
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);

    if (getY() < 0 || getX() < 0 || 
        getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) 
    {
        kill();
    }
};

void YellowBorder::hit()
{
    Actor* player = returnPlayer();
    if (getX() > 128) {
        player->setDirection(105);
        player->setHp(player->getHp() -10);
    }
    
    if (getX() < 128) {
        player->setDirection(75);
        player->setHp(player->getHp() -10);
    }

    
}






