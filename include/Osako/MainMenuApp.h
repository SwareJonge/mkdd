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
    void right();
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
    s8 _1C;
    s8 _1D;
    s8 _1E;
    s8 _1F;
    s8 _20;
    s8 _21;
    s8 mGameFlagIdx;
    s8 mSecretKartId;
    s8 mCupIdx;
    s8 mLevelIdx;
    s8 _26;
};

#endif
