#ifndef GEOITEMCAR_H
#define GEOITEMCAR_H

#include <JSystem/JParticle/JPAEmitter.h>

#include "JSystem/JGeometry/Vec.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/J3DAnmObject.h"
#include "dolphin/mtx.h"
#include "types.h"

class GeoItemCar : public GeoCar
{
public:
    GeoItemCar(const CrsData::SObject &);                    // 0x801c2210
    //virtual ~GeoItemCar(){}                                // 0x801c2b84
    virtual void reset();                                    // 0x801c23c8
    virtual void loadAnimation();                            // 0x801c250c
    virtual const char *getBmdFileName();                    // 0x801c255c
    virtual const char *getShadowBmdFileName();              // 0x801c2568
    virtual void createModel(JKRSolidHeap *, u32);           // 0x801c2588
    virtual void createColModel(J3DModelData *);             // 0x801c260c
    virtual void calc();                                     // 0x801c2654
    virtual void setRMtx();                                  // 0x801c275c
    virtual void update();                                   // 0x801c287c
    virtual const Vec &getTirePosOffset(int);                // 0x801c28c0
    virtual void hitKart(int);                               // 0x801c2950
    virtual void hitItemBomb(){};                            // 0x801c2bf4
    virtual bool isHitVibrationOn() { return false; }        // 0x801c2bf8
    void initCallBackDeath();                                // 0x801c29cc
    void doCallBackDeath();                                  // 0x801c29ec
    static bool hitItemBoxProc(int, JPABaseEmitter **);      // 0x801c2a50
    // Inline
    static f32 getColRadius() { return 150.0f; }                                   // 0x801c264c
    static f32 getSwingHeight(f32 x, s16 v) { return JMASSin(v) * sSwingAmp + x; } // 0x801c2720
    static f32 getSwingHeight(s16 v) { // fabricated
        f32 s = JMASSin(v); 
        return s * sSwingAmp; 
    }
    static s16 getSwingVel() { return sSwingVel; }
    // Inline/Unused
    // static u32 sCreateNum;

private:
    static const Vec sTireOfsTable[4];         // 0x80379018
    static f32 sSwingAmp;                      // 0x804146b0
    static s16 sSwingVel;                      // 0x804146b4
    static J3DAnmTextureSRTKey *sBtkAnm;       // 0x80416448
    
    s16 _260;
    s16 _262;
    s16 _264;
    s16 _266;
    f32 _268;
    f32 _26c;
    JGeometry::TVec3f _270; //
    JGeometry::TVec3f _27c; //
    JGeometry::TVec3f _288; //
    J3DAnmObjMaterial mMat; // 294
    JPABaseEmitter *mpEmitter[2]; // 2b4
    int mKartNo;

}; // class GeoItemCar

#endif // GEOITEMCAR_H
