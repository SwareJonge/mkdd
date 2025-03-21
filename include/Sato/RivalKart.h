#ifndef RIVALKART_H
#define RIVALKART_H

#include <JSystem/JKernel/JKRDisposer.h>

class KartBody;
class KartSus;
class RivalBodyCtrl;
class RivalItemCtrl;
class RivalSpeedCtrl;

class RivalKartBase
{ // fabricated
    u32 _0;
};

class RivalKart : RivalKartBase, JKRDisposer
{ // Autogenerated
public:
    // Global
    RivalKart(KartBody *body); // 0x80270f9c
    virtual ~RivalKart();      // 1c

    void Init();                   // 0x802711e0
    static void StartInit();       // 0x80271228
    static void Reset();           // 0x80271250
    void ResetInit();              // 0x80271274
    static void setRankAry();      // 0x802712bc
    f32 getMaxSpeed() const;       // 0x802712e0
    f32 getOffsetMaxSpeed() const; // 0x802712ec
    void setMaxSpeed(const f32 &); // 0x802712f8
    void ContempLate();            // 0x80271308
    void ActiveCtrl();             // 0x80271398
    // Inline/Unused
    // void InitParam();
    // void getTargetPos(JGeometry::TVec3f *);
    // void getVtTargetPos(JGeometry::TVec3f *);
    // void getRoadWidth();
    // void DoSearchTarget();

    RivalBodyCtrl *getBodyCtrl() { return mBodyCtrl; }
    RivalSpeedCtrl *getSpeedCtrl() { return mSpeedCtrl; }

    KartBody *mBody;            // 20
    KartSus *mSus[4];           // 24
    int _34;                    // 34
    int mNo;                    // 38
    RivalBodyCtrl *mBodyCtrl;   // 3c
    RivalItemCtrl *mItemCtrl;   // 40
    RivalSpeedCtrl *mSpeedCtrl; // 44
}; // 0x48

#endif // RIVALKART_H
