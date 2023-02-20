#ifndef RACEAPP_H
#define RACEAPP_H

#include "Osako/GameApp.h"
#include "Osako/PadRecorder.h"
#include "types.h"

class RaceApp : GameApp
{
public:
    static RaceApp *create();
    static void call();
    static RaceApp *ptr() { return mspRaceApp; };
    PadRecorder *getPadRecorder();

    RaceApp();
    virtual ~RaceApp();
    virtual void draw();
    virtual void calc();

    static RaceApp* mspRaceApp;
};

#endif