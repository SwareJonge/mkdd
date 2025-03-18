#ifndef GEOCANNON_H
#define GEOCANNON_H

#include "JSystem/J3D/J3DAnmTransform.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JSupport/JSUList.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

struct J3DAnmObjTrans; // TODO
class GeoCannon;

class GeoCannonSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoCannonSupervisor();

    void entryCannon(GeoCannon *);
    GeoCannon *searchCannon(s32);

    virtual ~GeoCannonSupervisor();   // 0x801bf428
    virtual void reset();             // overide
    virtual void calc();              // overide
private:
    JSUList<GeoCannon> mList;
};

class GeoCannon : public GeographyObj, StateObserver
{
public:
    GeoCannon(const CrsData::SObject &);   // 0x801bf6a8
    void resetAnimation();         // 0x801bfa68
    virtual void reset();                  // 0x801bf850    
    virtual void loadAnimation();          // 0x801bfb9c
    virtual void createModel(JKRSolidHeap *, u32); // 0x801bfc0c
    virtual void calc();                           // 0x801c009c
    virtual void update();                         // 0x801c00e8

    void shot();                           // 0x801c04c4
    void initFuncWait();                   // 0x801c0574
    void doFuncWait();                     // 0x801c0578

    // Inline
    bool isSameID(s32 id) const { return mObjData->mParam1 == id; } // 0x801bf644
    // Inline/Unused
    // static u32 sCreateNum;


    //virtual ~GeoCannon() {}                                           // 0x801c05d4

    virtual GeoCannonSupervisor *getSupervisor() { return sSupervisor; }
    virtual const char *getBmdFileName() { return "/Objects/Cannon1.bmd"; } // 0x801c065c
    static GeoCannonSupervisor *getSupervisor() { return sSupervisor; }
    
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }   // 0x801c0668

    virtual void InitExec(); // 0x801c035c
    virtual void MoveExec();// 0x801c0470
    
private:
    friend class GeoCannonSupervisor;

    static StateFuncSet<GeoCannon> sTable[1]; // 0x80396640
    static char *sEmitterName[4];             // 0x8039665c
    static u16 sSupervisorCreateNum;          // 0x80416420
    static GeoCannonSupervisor *sSupervisor;  // 0x80416424
    static J3DAnmTevRegKey *sTevRegKey;       // 0x80416428
    static J3DAnmTransform *sBcaAnmTrans;      // 0x8041642c
    static J3DMtxCalc *sBcaMtxCalc;           // 0x80416430

    JSULink<GeoCannon> mLink;      // 158
    JGeometry::TVec3f mFrDir;      // 168
    JGeometry::TVec3f mUpDir;      // 174
    JGeometry::TVec3f mLfDir;      // 180
    int _18c;                      //
    JGeometry::TVec3f _190;        //
    J3DAnmObjMaterial mMat;        // 19C
    JPABaseEmitter *mpaEmitter[4]; // 1BC
};

#endif // GEOCANNON_H
