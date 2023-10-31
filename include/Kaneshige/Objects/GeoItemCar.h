#ifndef GEOITEMCAR_H
#define GEOITEMCAR_H

#include <JSystem/JParticle/JPAEmitter.h>

#include "Kaneshige/Objects/GeoCar.h"

class GeoItemCar : public GeoCar
{
public:
    GeoItemCar(const CrsData::SObject &);                    // 0x801c2210
    virtual ~GeoItemCar(){};                                 // 0x801c2b84
    virtual void reset();                                    // 0x801c23c8
    virtual void loadAnimation();                            // 0x801c250c
    virtual char *getBmdFileName();                          // 0x801c255c
    virtual char *getShadowBmdFileName();                    // 0x801c2568
    virtual void createModel(JKRSolidHeap *, unsigned long); // 0x801c2588
    virtual void createColModel(J3DModelData *);             // 0x801c260c
    virtual void calc();                                     // 0x801c2654
    virtual void setRMtx();                                  // 0x801c275c
    virtual void update();                                   // 0x801c287c
    virtual JGeometry::TVec3f *getTirePosOffset(int);        // 0x801c28c0
    virtual void hitKart(int);                               // 0x801c2950
    virtual void hitItemBomb(){};                            // 0x801c2bf4
    virtual bool isHitVibrationOn() { return false; }        // 0x801c2bf8
    void initCallBackDeath();                                // 0x801c29cc
    void doCallBackDeath();                                  // 0x801c29ec
    void hitItemBoxProc(int, JPABaseEmitter **);             // 0x801c2a50
    // Inline
    f32 getColRadius() { return 150.0f; }                                   // 0x801c264c
    f32 getSwingHeight(f32 x, s16 v) { return sSwingAmp * JMASSin(v) + x; } // 0x801c2720
    // Inline/Unused
    // static u32 sCreateNum;

private:
    static JGeometry::TVec3f sTireOfsTable[4]; // 0x80379018
    static f32 sSwingAmp;                      // 0x804146b0
    static s16 sSwingVel;                      // 0x804146b4
    static J3DAnmTextureSRTKey *sBtkAnm;       // 0x80416448
    // TODO
}; // class GeoItemCar
// void GeographyObj::showShadow() // GeographyObj.h; // 0x801c24b8
// void ViewCtrlModel::setVisibleAll() // ViewCtrlModel.h; // 0x801c24e4
// void GeographyObj::getShadowModel() const // GeographyObj.h; // 0x801c24f4
// void JGeometry::TVec3<f32>::scale(f32, const JGeometry::TVec3<f32> &) // JGeometry.h; // 0x801c2850
// void GeographyObj::clrAllIsHitKartFlg() // GeographyObj.h; // 0x801c29c0
#endif // GEOITEMCAR_H