#ifndef KARTDRAWER_H
#define KARTDRAWER_H

#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/DrawBuffer.h"
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
        virtual void viewCalc(u32 idx); // 0x80198c64
        // Inline
        void setLevelNo(int);  // 0x801b8c08
        void setDriverNo(int); // 0x801b8c10
        void setKartNo(int);   // 0x801b8c18
        ~DriverDB() {
            _10 = 0;
            _14 = 0;
            _18 = 0;
        }
        DriverDB();            // 0x801b8c9c

    private:
        int _10;
        int _14;
        int _18;
    };

    enum EDrawStage
    {
        // TODO
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
    void isFlashHidding();                  // 0x801b94b4
    void enableDrawing(u32, EDrawStage);    // 0x801b951c
    void drawKart(u32, EDrawStage);         // 0x801b96a4
    void drawDriver(u32, EDrawStage);       // 0x801b9760
    void update();                          // 0x801b9904
    void setLODLevel(u32, u16);             // 0x801b9b98
    void playTevAnm(int);                   // 0x801b9bd4
    void stopTevAnm();                      // 0x801b9bf4
    static s16 sFlashInterval;              // 0x80414648
    static u8 sGhostA;                      // 0x8041464a
    static s16 sGhostFadeOutTime;           // 0x8041464c
    // Inline/Unused
    // void setFrame(float);
    // void J3DAnmObjBase::setFrame(float);
    // void sForceLODLevel;
    // Inline
    void isAvailableTevAnmShock(); // 0x801b9194
    void isFlashing() const;       // 0x801b950c
    void isHide(u32) const;        // 0x801b95c8
    void hide();                   // 0x801b9b88

    void setLight(u32 viewNo, RaceKartLight *kartLight)
    {
#line 107
        JUT_MAX_ASSERT(viewNo, 4);
        mKartLight[viewNo] = kartLight;
    }

    u8 _0[0x30];
    RaceKartLight *mKartLight[4];
    u8 _40[0x178 - 0x40];

    // Size 0x178
}; // class KartDrawer
// Outside class members
// void DrawBuffer::create(u32);
// 0x// void KartCtrl::SetCoDriverCurrentViewNo(int, u32) // KartCtrl.h; // 0x801b8598
// void KartCtrl::SetDriverCurrentViewNo(int, u32) // KartCtrl.h; // 0x801b85dc
// void KartCtrl::DrwaCoDriver(int, u16) // KartCtrl.h; // 0x801b868c
// void KartCtrl::DrwaDriver(int, u16) // KartCtrl.h; // 0x801b86d0
// void OsageDrawBuffer::setTargetKart(short) // OsageDrawBuffer.h; // 0x801b8bc4
// void OsageDrawBuffer::OsageDrawBuffer(u32) // OsageDrawBuffer.h; // 0x801b8bcc
// void KartLoader::isOsageExist(int) // KartLoader.h; // 0x801b8e0c
// void J3DAnmObjMaterial::getAnmBase() // J3DAnmObjMaterial.h; // 0x801b91c0
// void J3DFrameCtrl::setAttribute(u8) // J3DFrameCtrl.h; // 0x801b91c8
// void J3DAnmObjBase::getFrameCtrl() // J3DAnmObjBase.h; // 0x801b91d0
// void J3DAnmObjBase::setRate(const float &) // J3DAnmObjBase.h; // 0x801b9310
// void J3DFrameCtrl::setRate(float) // J3DFrameCtrl.h; // 0x801b9338
// void J3DFrameCtrl::setFrame(float) // J3DFrameCtrl.h; // 0x801b93dc
// void KartInfo::isAvailableKart() const // KartInfo.h; // 0x801b95e4
// void KartInfo::isAvailableDriver(int) const // KartInfo.h; // 0x801b9608
// void KartLoader::getWheelNumber() // KartLoader.h; // 0x801b9b80
#endif // KARTDRAWER_H