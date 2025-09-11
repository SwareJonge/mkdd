#ifndef ANMPLAYER_H
#define ANMPLAYER_H

#include "Osako/ResMgr.h"
#include "Sato/AnmController.h"

class TAnmInfo
{
public:
    s8 get_12() { return _12; } // fabricated, but needed to get lbzx instead of lbz addi

    const char *mBckName;       // 0
    J3DAnmTransform *mTransAnm; // 4
    J3DMtxCalc *mCalcAnm;       // 8
    int mAnmCnt;                // C
    u8 mBlendFrameCnt;          // 10
    u8 _11;                     // 
    s8 _12;                     //

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
    bool isCurAnmReachEnd();                                                       // 0x802986c0
    bool checkFrameEnd();                                                          // 0x8029878c
    bool isFitBlendFrame();                                                        // 0x802988e4
    bool isGoArroundLoop();                                                        // 0x80298ab0

    u8 getCurAnmNumber() const { return mController->getNowTransNo(); }

    AnmController *mController; // 4
    TAnmInfo *mAnmInfo;         // 8
    u8 mMaxAnm;                 // c
    u8 _d;
    u16 _e;
    u8 _10;
    s8 _11;
    f32 _14;
}; // Size: 0x18
#endif // ANMPLAYER_H
