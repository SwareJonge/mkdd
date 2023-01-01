#ifndef APPMGR_H
#define APPMGR_H

#include "Osako/GameApp.h"

#include "types.h"

class AppMgr
{
public:
    enum KartAppEnum
    {
        KARTAPP_0 = 0
    };

    static void draw();
    static void calc();
    static bool setNextApp(KartAppEnum);
    static void insertErrorViewApp();
    static GameApp *createApp(KartAppEnum);
    static void restartApp();
    static bool isRestartable();

    static KartAppEnum msGameApp;
    static KartAppEnum msNextGameApp;
    static KartAppEnum msNextNextGameApp;
    static KartAppEnum msPrevGameApp;
    static GameApp * mspGameApp;
    static int msRequest;
    static u8 msDrawStallFrame;
    static u8 msCalcStallFrame;
    static u8 msChangeStallFrame;
};

#endif