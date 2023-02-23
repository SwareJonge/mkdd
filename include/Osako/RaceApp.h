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
    PadRecorder *getPadRecorder() {
        return mPadRecorder;
    }

    RaceApp();
    virtual ~RaceApp();
    virtual void draw();
    virtual void calc();

    static RaceApp* mspRaceApp;
private:
    u8 _0C[0x28 - 0xc];
    PadRecorder * mPadRecorder;
};

#endif