#include "Actor.h"
#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void GhostRacer::doSomething() {
    int key;
    if (getWorld()->getKey(key)) {
        cout << key << endl; 
    }
}
