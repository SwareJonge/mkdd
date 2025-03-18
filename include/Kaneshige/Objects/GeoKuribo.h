#ifndef GEOKURIBO_H
#define GEOKURIBO_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

class GeoKuribo;

class GeoKuriboSupervisor : public GeoObjSupervisor, JKRDisposer {
public:
    GeoKuriboSupervisor();
    virtual ~GeoKuriboSupervisor(); // 0x801cb154
    void reset();
    void calc();
    void entry(GeoKuribo *);
    void thunderDownAll();

private:
    JSUList<GeoKuribo> mList;
};

class GeoKuribo : public GeographyObj, public StateObserver {
public:
    GeoKuribo(const CrsData::SObject &); // 0x801cb370
    void reset(); // 0x801cb4d0
    void resetAnimation(); // 0x801cb7b0
    void resetWalkDirection(short); // 0x801cb83c
    void loadAnimation(); // 0x801cba84
    void createModel(JKRSolidHeap *, unsigned long); // 0x801cbb6c
    void createColModel(J3DModelData *); // 0x801cbc1c
    void doKartColCallBack(int); // 0x801cbc44
    void changeStateCrush(int); // 0x801cbc88
    void thunderDown(); // 0x801cbd38
    void calc(); // 0x801cbd5c
    void checkItemHitting(); // 0x801cbdf8
    void occurKinoko(int); // 0x801cbeb4
    void initFuncWait(); // 0x801cc128
    void doFuncWait(); // 0x801cc12c
    void initFuncWalk(); // 0x801cc130
    void doFuncWalk(); // 0x801cc144
    void initFuncCrush(); // 0x801cc4b4
    void doFuncCrush(); // 0x801cc564
    void initFuncRecover(); // 0x801cc604
    void doFuncRecover(); // 0x801cc658

    // PTMF
    void InitExec(); // 0x801cbfc0
    void MoveExec(); // 0x801cc0d4

    // Inline
    bool tstKuriboFlagDecPoint() const { return mPointFlags & 1; } // 0x801cba78
    void setKuriboFlagDecPoint() { mPointFlags |= 1; }  // 0x801cc44c
    void clrKuriboFlagDecPoint() { mPointFlags &= ~1; } // 0x801cc45c
    //virtual ~GeoKuribo(); // 0x801cc794
    virtual const char *getBmdFileName() { return "/Objects/Kuribo1.bmd"; } // 0x801cc810
    virtual const char *getShadowBmdFileName() { return "/Objects/Kuribo1Shadow.bmd"; } // 0x801cc81c
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; } // 0x801cc828

private:
    friend class GeoKuriboSupervisor;

    static const u8 cNazoTable[8];

    static StateFuncSet<GeoKuribo> sTable[4]; // 0x803976e8
    static f32 sWalkAccl;                     // 0x80414708
    static f32 sWalkMaxVel;                   // 0x8041470c
    static f32 sWalkTurnRate;                 // 0x80414710
    static f32 sPointR;                       // 0x80414714
    static f32 sOccurVel0;                    // 0x80414718
    static f32 sShadowScale;                  // 0x8041471c
    static f32 sWalkAnmFrame;                 // 0x80414720
    static s16 sCrushAnmFrame;                // 0x80414724
    static s16 sBlendAnmFrame;                // 0x80414726
    static GeoKuriboSupervisor *sSupervisor;  // 0x804164a0
    static s16 sSupervisorCreateNum;          // 0x804164a4
    static J3DAnmTransform *sAnmTransWalkL;    // 0x804164a8
    static J3DAnmTransform *sAnmTransWalkR;   // 0x804164ac
    static J3DMtxCalc *sMtxCalcWalkL;         // 0x804164b0
    static J3DMtxCalc *sMtxCalcWalkR;         // 0x804164b4

    // Unused
    static s16 sCreateNum;

    JSULink<GeoKuribo> mLink;
    u8 mNazo;
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    f32 mYScale;
    u16 mPointFlags;
    s16 mPointID;
    CrsGround mGround;
    f32 mWalkAccl;
    f32 mMaxWalkAccl;
    f32 mWalkDir;
    f32 mWalkVel;
    f32 mMaxWalkVel;
    f32 mRate;
    s16 mCrushTimer;
    s16 mMaxCrushTime;
    s16 mRecoverState;
    f32 _218;
    f32 _21c;
};

#endif // GEOKURIBO_H
