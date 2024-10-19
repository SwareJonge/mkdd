#ifndef KARTDRAWER_H
#define KARTDRAWER_H

#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/DarkAnmMgr.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/DrawBuffer.h"
#include "Kawano/osage.h"
#include "Sato/J3DAnmObject.h"

#include "kartEnums.h"
#include "types.h"

class KartDrawer
{ // Autogenerated
public:
    // Global
    class DriverDB : public DrawBuffer
    {
    public:
        virtual void drawIn(u32 idx);   // 0x801b850c
        virtual void update();          // 0x801b8620
        // Inline
        void setLevelNo(int no) { mLevelNo = no;  }
        void setDriverNo(int no) { mDriverNo = no; }
        void setKartNo(int no) { mKartNo = no; }
        DriverDB() {
            mKartNo = 0;
            mDriverNo = 0;
            mLevelNo = 0;
        } 
        ~DriverDB() {}

    private:
        int mKartNo;   // 10
        int mDriverNo; // 14
        int mLevelNo;  // 18
    };

    enum EDrawStage
    {
        STAGE_0, // None?
        STAGE_1,
        STAGE_2,
        STAGE_GHOST
    };

    KartDrawer();                           // 0x801b8714
    void reset();                           // 0x801b8844
    void create(int, int, u16, EGhostKind); // 0x801b88e8
    void setAnimation();                    // 0x801b8ce8
    void resetAnimation();                  // 0x801b8e34
    void setTevAnm(int);                    // 0x801b8e68
    void attach(int);                       // 0x801b8f34
    void setAttribute(J3DAnmAttr);          // 0x801b90cc
    void resetFrame();                      // 0x801b91d8
    void setRate(float);                    // 0x801b9264
    void kartAnmFrameProc();                // 0x801b9340
    void frameProc();                       // 0x801b93e4
    void setShadowInfo(const CrsArea &);    // 0x801b9470
    bool isFlashHidding();                  // 0x801b94b4
    bool enableDrawing(u32, EDrawStage);    // 0x801b951c
    void drawKart(u32, EDrawStage);         // 0x801b96a4
    void drawDriver(u32, EDrawStage);       // 0x801b9760
    void update();                          // 0x801b9904
    void setLODLevel(u32, u16);             // 0x801b9b98
    void playTevAnm(int);                   // 0x801b9bd4
    void stopTevAnm();                      // 0x801b9bf4
    // Inline/Unused
    void setFrame(f32 rate);
    // void sForceLODLevel;
    // Inline
    bool isAvailableTevAnmShock() { return mShockAnm.getAnmBase() != nullptr; }
    bool isFlashing() const { return mFlashState != 0; }
    bool isHide(u32 viewNo) const { return (mHiddenDrivers & 1 << viewNo) != 0; }
    void hide() { mHiddenDrivers = 0xffff; }

    void setLight(u32 viewNo, RaceKartLight *kartLight)
    {
#line 107
        JUT_MAX_ASSERT(viewNo, 4);
        mKartLight[viewNo] = kartLight;
    }

private:
    static s16 sFlashInterval;          // 0x80414648
    static u8 sGhostA;                  // 0x8041464a
    static s16 sGhostFadeOutTime;       // 0x8041464c

    int mKartNo;                        // 0
    u32 mMaxViewNo;                     // 4
    bool mEnableLOD;                    // 8
    EDrawStage mStartStage;             // c
    EDrawStage mStage;                  // 10
    u8 mGhostAlpha;                     // 14
    s16 mGhostFadeTimer;                // 16
    u16 mHiddenDrivers;                 // 18
    u16 mMaxLevel;                      // 1a
    u16 *mpLOD;                         // 1c
    DriverDB *_20;                      // 20
    DriverDB *_24;                      // 24 
    OsageDrawBuffer *mOsageDB;          // 28
    u16 mFlashState;                    // 2c
    u16 mFlashTime;                     // 2e
    RaceKartLight *mKartLight[4];       // 30
    DarkAnmPlayer *_40[2];              // 40
    DarkAnmPlayer *_48;                 // 48
    s16 mShadowID;                      // 4c
    f32 mShadowRate;                    // 50
    int mAnmNo;                         // 54
    J3DAnmObjMaterial mDriverAnm[2];    // 58
    J3DAnmObjMaterial mAccessoryAnm[2]; // 98
    J3DAnmObjMaterial mBodyAnm;         // d8
    J3DAnmObjMaterial mArmAnm;          // f8
    J3DAnmObjMaterial mShockAnm;        // 118
    J3DAnmObjMaterial mWheelLAnm;       // 138
    J3DAnmObjMaterial mWheelRAnm;       // 158
};  // Size 0x178
#endif // KARTDRAWER_H
