#ifndef RACEAPP_H
#define RACEAPP_H

#include "Osako/GameApp.h"
#include "types.h"

class RaceApp : GameApp
{
public:
    static RaceApp *create();
    static void call();

    RaceApp();
    virtual ~RaceApp();
    virtual void draw();
    virtual void calc();
};

#endif