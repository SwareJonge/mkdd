#ifndef MAINMENUAPP_H
#define MAINMENUAPP_H

#include "types.h"
#include "Osako/GameApp.h"

class MainMenuApp : public GameApp
{
public:
    static MainMenuApp *create();
    static void call();

    MainMenuApp();
    virtual ~MainMenuApp(); // override

    void up();
    void down();
    void left();
    void decide();
    void cancel();       // doesn't exist, inline auto?
    virtual void draw(); // override
    virtual void calc(); // override

    static MainMenuApp *ptr() { return mspMainMenuApp; }

private:
    static MainMenuApp *mspMainMenuApp;
    int _C;
    int _10;
    int _14;
    int _18;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    u8 _1F;
    u8 _20;
    u8 _21;
    u8 mGameFlagIdx;
    u8 _23;
    u8 _mCupIdx;
    u8 _mLevelIdx;
    u8 _26;
};

#endif