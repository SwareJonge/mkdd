#ifndef ANMPLAYER_H
#define ANMPLAYER_H

#include "Osako/ResMgr.h"
#include "Sato/AnmController.h"

class TAnmInfo
{
public:
    const char *mBckName;        // 0
    AnmControlTrans *mTransAnm; // 4
    J3DMtxCalc *mCalcAnm;       // 8
    int mAnmCnt;                // C
    bool mHasBlendColor;        // 10
}; // Size: 0x14

class TAnmPlayer
{
public:
    TAnmPlayer();                                                                  // 0x80297d2c
    virtual ~TAnmPlayer();                                                         // 0x80297d50
    static void resetAnimations(TAnmInfo *, u8);                                   // 0x80297d98
    static void loadAnimations(TAnmInfo *, u8, J3DModelData *, ResMgr::ArchiveId); // 0x80297dd0
    static void registAnimations(AnmController *, ExModel *, TAnmInfo *, u8);      // 0x80297f14
    void init(AnmController *, TAnmInfo *, u8);                                    // 0x80298090
    void reset();                                                                  // 0x802980bc
    void update();                                                                 // 0x80298248
    void firstEndCheck(u8);                                                        // 0x80298604
    void isCurAnmReachEnd();                                                       // 0x802986c0
    void checkFrameEnd();                                                          // 0x8029878c
    void isFitBlendFrame();                                                        // 0x802988e4
    void isGoArroundLoop();                                                        // 0x80298ab0

    u8 getCurAnmNumber() const { return mController->getNowTransNo(); }

    AnmController *mController;
    int _8;
    u8 _c;
    u8 _d;
    u16 _e;
    u8 _10;
}; // Size: 0x14
#endif // ANMPLAYER_H