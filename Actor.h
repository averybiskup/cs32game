#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"
#include <random>

class StudentWorld;

//int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0)
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int depth)
        : GraphObject(imageID, x, y, dir, size, depth)
    {
        sWorld = sw;
        dead = false; 
        verticalSpeed = 0;
        collideable = false;
        horizontalSpeed = 0;
    };
      // Action to perform for each tick.
    virtual void doSomething() = 0;

      // Is this actor dead?
    bool isDead() const;
    
      // Mark this actor as dead.
    void setDead();

      // Get this actor's world
    StudentWorld* world() const;

      // Get this actor's vertical speed.
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;

      // Set this actor's vertical speed.
    void setVerticalSpeed(double speed);
    void setHorizontalSpeed(double speed);


      // If this actor is affected by holy water projectiles, then inflict that
      // affect on it and return true; otherwise, return false.
    virtual bool beSprayedIfAppropriate() = 0;

      // Does this object affect zombie cab placement and speed?
    virtual bool isCollisionAvoidanceWorthy() const = 0;
    void setCollideable();
    bool getCollideable();
    void offScreen();
    int randint(int min, int max);
    

      // Adjust the x coordinate by dx to move to a position with a y coordinate
      // determined by this actor's vertical speed relative to GhostRacser's
      // vertical speed.  Return true if the new position is within the view;
      // otherwise, return false, with the actor dead.
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx) = 0;

private:
    bool dead;
    StudentWorld* sWorld;
    double verticalSpeed;
    double horizontalSpeed;
    bool collideable;

};

class BorderLine : public Actor
{
public:
    //Actor(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int depth)
    BorderLine(StudentWorld* sw, double x, double y, bool isYellow)
        : Actor(sw, (isYellow) ? IID_YELLOW_BORDER_LINE : IID_WHITE_BORDER_LINE,
                      x, y, 2, 0, 2) 
        {
            setVerticalSpeed(-4); 
        };
    void doSomething();
    bool isCollisionAvoidanceWorthy() const;
    bool beSprayedIfAppropriate();
    bool moveRelativeToGhostRacerVerticalSpeed(double dx);
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp)
        : Actor(sw, imageID, x, y, size, dir, 0) 
    {
        health = hp;
        setCollideable();
        movementPlan = 0;
    };
    bool isCollisionAvoidanceWorthy() const;
    virtual void doSomething() = 0;
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx);

      // Get hit points.
    int getHP() const;

      // Increase hit points by hp.
    void setHP(int hp);

      // Do what the spec says happens when hp units of damage is inflicted.
      // Return true if this agent dies as a result, otherwise false.
    virtual bool takeDamageAndPossiblyDie(int hp);

      // What sound should play when this agent is damaged but does not die?
    virtual int soundWhenHurt() = 0;

      // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() = 0;

    int getMovementPlan() { return movementPlan; };
    void setMovementPlan(int mp) { movementPlan = mp; };

private:
    int movementPlan;
    int health;
};

class GhostRacer : public Agent
{
public:
    GhostRacer(StudentWorld* sw, double x, double y)
        : Agent(sw, IID_GHOST_RACER, x, y, 4, 90, 100)
    {
        spraysLeft = 10;
        bonus = 0;
        soulsSaved = 0;
    };
    void doSomething();
    virtual int soundWhenDie();
    virtual int soundWhenHurt();
    virtual bool beSprayedIfAppropriate();

    int getSprays();
    void setSprays(int s);

    int getBonus();

    void setBonus(int b);

      // How many holy water projectiles does the object have?
    int getNumSprays() const;

      // Spin as a result of hitting an oil slick.
    void spin();

private:
    int spraysLeft;
    int soulsSaved;
    int bonus;
};

class ZombieCab : public Agent
{
public:
    ZombieCab(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual int soundWhenDie();
    virtual int soundWhenHurt();
    void setDamaged();
    int getDamaged();
private:
    bool damaged;
};

class Pedestrian : public Agent
{
public:
    Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size)
        : Agent(sw, imageID, x, y, size, 0, 2)
    {
        setCollideable();   
        setVerticalSpeed(-4);
    };

      // Move the pedestrian.  If the pedestrian doesn't go off screen and
      // should pick a new movement plan, pick a new plan.
    void moveAndPossiblyPickPlan();
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* sw, double x, double y)
        : Pedestrian(sw, IID_HUMAN_PED, x, y, 2)
    {
    };
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    int soundWhenHurt();
    int soundWhenDie();
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* sw, double x, double y)
        : Pedestrian(sw, IID_ZOMBIE_PED, x, y, 3)
    {
    };
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    int soundWhenHurt();
    int soundWhenDie();
};


class Spray : public Actor
{
public:
    Spray(StudentWorld* sw, double x, double y, int dir)
        : Actor(sw, IID_HOLY_WATER_PROJECTILE, x, y, 1, dir, 1)
    {
        maxTravelDistance = 160;
        travelled = 0;
    };
    virtual void doSomething();
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx) { return false; };
    virtual bool beSprayedIfAppropriate() { return false; };
    void increaseTravel();
    virtual bool isCollisionAvoidanceWorthy() const;

public:
    int maxTravelDistance;
    int travelled;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir)
        : Actor(sw, imageID, x, y, size, dir, 2)
    {
        setVerticalSpeed(-4);
        alreadyCollided = false; 
    };

    bool isCollisionAvoidanceWorthy() const;
      //Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;
    virtual void doMove();

      // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

      // Return whether the object dies after activation.
    virtual bool selfDestructs() const = 0;

      // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const = 0;
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx) { return false; };
    void setCollided();
    bool getCollided();
private:
    bool alreadyCollided;
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* sw, double x, double y) 
        : GhostRacerActivatedObject(sw, IID_OIL_SLICK, x, y, randint(2, 5), 0)
    {
    };
    virtual bool beSprayedIfAppropriate();
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* sw, double x, double y)
        : GhostRacerActivatedObject(sw, IID_HEAL_GOODIE, x, y, 1, 0)
    {
    };
    virtual bool beSprayedIfAppropriate();
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual int getSound() const;
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* sw, double x, double y)
        : GhostRacerActivatedObject(sw, IID_HOLY_WATER_GOODIE, x, y, 2, 90)
    {
    };
    virtual bool beSprayedIfAppropriate();
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual int getSound() const;
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* sw, double x, double y)
        : GhostRacerActivatedObject(sw, IID_SOUL_GOODIE, x, y, 4, 0)
    {
    };
    virtual bool beSprayedIfAppropriate();
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual int getSound() const;
};

#endif // ACTOR_INCLUDED
