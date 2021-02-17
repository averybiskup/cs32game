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
                if (getDirection() < 130)
                    setDirection(getDirection() + 15);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 50)
                    setDirection(getDirection() - 15);
                break;
            case KEY_PRESS_SPACE:
                break;
            case KEY_PRESS_UP:
                if (getSpeed() < 15) 
                {
                    setSpeed(1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getSpeed() > 1) 
                {
                    setSpeed(- 1);
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
    if (getX() > 128) {
        returnPlayer()->setDirection(100);
    }
    
    if (getX() < 128) {
        returnPlayer()->setDirection(80);
    }
}






