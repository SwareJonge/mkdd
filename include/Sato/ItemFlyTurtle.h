#ifndef ITEMFLYTURTLE_H
#define ITEMFLYTURTLE_H

#include "Kaneshige/CenterLine.h"
#include "Kaneshige/KartChecker.h"
#include "Sato/AnmController.h"
#include "Sato/ItemObj.h"
#include "Sato/ObjUtility.h"

// TODO: virtual and static functions

class ItemFlyTurtle : public ItemObj
{
public:
    ItemFlyTurtle(u32);                      // 0x80279054
    static void loadAnmData(J3DModelData *); // 0x8027928c
    void setInitReleaseParam();              // 0x80279944
    void moveWait();                         // 0x80279b10
    void moveFlying();                       // 0x80279bec
    void moveSearchBack();                   // 0x80279d38
    void moveSearch();                       // 0x80279f4c
    void moveAttack();                       // 0x8027a1a4
    void moveCommon();                       // 0x8027a330
    void find1StKartNo();                    // 0x8027a4d0
    void chkSearchTargetKartRadius();        // 0x8027a808
    void chkIsBackTargetKartOffsetPos();     // 0x8027a8ec
    void calcNowHeight();                    // 0x8027affc

    // Inline/Unused
    static const char *getBckName(int);
    // VTable
    virtual ~ItemFlyTurtle() {}                                   // 8
    virtual void reset();                                         // 10
    virtual void update();                                        // 14
    virtual void setCurrentViewNo(u32);                           // 1C
    virtual const char *getBmdFileName();                         // 28
    virtual int getLockOnKartNo() const { return mLockOnKartNo; } // 34
    virtual void createModel(JKRSolidHeap *, u32, u32);           // 40
    virtual void createColModel(J3DModelData *);                  // 44
    virtual void doUniv();                                        // 50
    virtual void initEquip();                                     // 54
    virtual void initRelease();                                   // 58
    virtual void initSpRelease() { initRelease(); };              // 5C
    virtual void initDisappear();                                 // 60
    virtual void initFall();                                      // 64
    virtual void initForceDisappear();                            // 70
    virtual void doEquip();                                       // 78
    virtual void doRelease();                                     // 7C
    virtual void doSpRelease() { doRelease(); };                  // 80
    virtual void doDisappear();                                   // 84
    virtual void moveColWall(CrsGround &);                        // 98
    virtual void moveNormal(CrsGround &);                         // 9C
    virtual void moveRescue(CrsGround &);                         // A0
    virtual void moveCannon(CrsGround &);                         // A4
    virtual void deleteEffect();                                  // A8

    static Vec sHandOffsetPos;           // 0x8041383c
    static J3DAnmTransform *mAnmTrans[2]; // 0x80414f64
    static J3DMtxCalc *mAnmCalc[2];      // 0x80414f6c

    AnmController mController;                      // 2b0
    CLPointControllerFlyTurtle *mCLPointController; // 2c4
    int mMoveState;                                 // 2c8
    int mTargetKartNo;                              // 2cc
    int mLockOnKartNo;                              // 2d0
    u8 _2d4[0x32c - 0x2d4];                         //
    BombEffectObj *mBombEffect;                     // 32c
    ObjColBase *mNewBounds;                         // 330
    u8 _334[0x350 - 0x334];                         //
    LapChecker *mLapChecker;                        // 350
}; // class ItemFlyTurtle

class CLPointControllerFlyTurtle : public CLPointController
{
public:
    CLPointControllerFlyTurtle();
    virtual int findNextPointNo(CLPoint *);
    void recursiveTargetCalcBack(CLPoint *, int, int *, u8 *, bool *);
};

#endif // ITEMFLYTURTLE_H
