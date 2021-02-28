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
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp);
    bool isCollisionAvoidanceWorthy() const;

      // Get hit points.
    int getHP() const;

      // Increase hit points by hp.
    void setHP(int hp);

    int getMovementPlan() { return movementPlan; };
    void setMovementPlan(int mp) { movementPlan = mp; };

private:
    int movementPlan;
    int health;
};

class GhostRacer : public Agent
{
public:
    GhostRacer(StudentWorld* sw, double x, double y);
    void doSomething();
    bool beSprayedIfAppropriate();

    int getSprays();
    void setSprays(int s);

private:
    int spraysLeft;
};

class ZombieCab : public Agent
{
    public:
        ZombieCab(StudentWorld* sw, double x, double y);
        void doSomething();
        bool beSprayedIfAppropriate();
        void setDamaged();
        int getDamaged();
    private:
        bool damaged;
};

class Pedestrian : public Agent
{
    public:
        Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size);
        void moveAndPossiblyPickPlan();
};

class HumanPedestrian : public Pedestrian
{
    public:
        HumanPedestrian(StudentWorld* sw, double x, double y);
        void doSomething();
        bool beSprayedIfAppropriate();
};

class ZombiePedestrian : public Pedestrian
{
    public:
        ZombiePedestrian(StudentWorld* sw, double x, double y);
        void doSomething();
        bool beSprayedIfAppropriate();
        int getTicksGrunt();
        void setTicksGrunt(int t);
    private:
        int ticksGrunt;
};


class Spray : public Actor
{
public:
    Spray(StudentWorld* sw, double x, double y, int dir);
    void doSomething();
    bool beSprayedIfAppropriate() { return false; };
    void increaseTravel();
    bool isCollisionAvoidanceWorthy() const;

public:
    int maxTravelDistance;
    int travelled;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir);

    bool isCollisionAvoidanceWorthy() const;
      //Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;
    void doMove();
    void checkCollide(GhostRacer* gr, int sound);

      // Return whether the object is affected by a holy water projectile.
    void setCollided();
    bool getCollided();
private:
    bool alreadyCollided;
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* sw, double x, double y);
    bool beSprayedIfAppropriate();
    void doSomething();
    void doActivity(GhostRacer* gr);
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* sw, double x, double y);
    bool beSprayedIfAppropriate();
    void doSomething();
    void doActivity(GhostRacer* gr);
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* sw, double x, double y);
    bool beSprayedIfAppropriate();
    void doSomething();
    void doActivity(GhostRacer* gr);
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* sw, double x, double y);
    bool beSprayedIfAppropriate();
    void doSomething();
    void doActivity(GhostRacer* gr);
};

#endif // ACTOR_INCLUDED
