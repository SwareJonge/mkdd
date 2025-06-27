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
    int _c;           //
    int _10;          //
    int _14;          //
    int _18;          //
    s8 mCursorPos;    // 1c
    s8 mDebugMode;    // 1d
    s8 mPlayerCount;  // 1e
    s8 mTestMode;     // 1f
    s8 mGameMode;     // 20
    s8 mBattleMode;   // 21
    s8 mGameFlagIdx;  // 22
    s8 mSecretKartId; // 23
    s8 mCupIdx;       // 24
    s8 mLevelIdx;     // 25
    s8 mRankIdx;      // 26
};

#endif
