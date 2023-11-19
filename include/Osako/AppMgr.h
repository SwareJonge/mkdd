#ifndef APPMGR_H
#define APPMGR_H

#include "Osako/GameApp.h"

#include "types.h"

class AppMgr
{
public:
    enum KartAppEnum
    {
        mcKartApp0,
        mcKartApp1,
        mcKartApp2,
        mcErrorView,
        mcLogo,
        mcMainMenu,
        mcPlayerSelect,
        mcMapSelect,
        mcRace,
        mcAward,
        mcSequence,
        mcNetGate,
        mcMovie,
        mcKartApp13,
        mcScene,
        mcTest,
        mcTestView
    };

    static void draw();
    static void calc();
    static bool setNextApp(KartAppEnum);
    static void insertErrorViewApp();
    static GameApp *createApp(KartAppEnum);
    static void restartApp();
    static bool isRestartable();

    static KartAppEnum getPrevApp() { return msPrevGameApp; }
    static void deleteCurrentApp() { msRequest |= 1; }

private:
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