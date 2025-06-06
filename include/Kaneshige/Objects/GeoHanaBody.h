#ifndef GEOHANABODY_H
#define GEOHANABODY_H

#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Kaneshige//Objects/GeoHanaCar.h"

class GeoHanaBody : public GeoCar
{ // Autogenerated
public:
    GeoHanaBody();                                                                // 0x801c3488
    void initPos();                                                               // 0x801c36c4
    void move();                                                                  // 0x801c37ec
    virtual void reset();                                                         // 0x801c3528
    virtual void loadAnimation();                                                 // 0x801c357c
    virtual void createModel(JKRSolidHeap *, u32);                                // 0x801c35cc
    virtual void createColModel(J3DModelData *);                                  // 0x801c3634
    virtual void simpleDraw(u32);                                                 // 0x801c3670
    virtual void calc();                                                          // 0x801c37e8
    virtual const Vec &getTirePosOffset(int tireNo);                              // 0x801c39b4
    //virtual ~GeoHanaBody() {}                                                     // 0x801c3a44
    virtual const char *getBmdFileName() { return "/Objects/Car_Hana1Body.bmd"; } // 0x801c3ab4
    virtual u16 getPathID() { return mParentHana->getPathID(); };                 // 0x801c3ac0

    // Inline
    void setParent(GeoHanaCar *parentHana, GeoCar *parentCar) {
        mParentHana = parentHana;
        mParentCar = parentCar;
        setCarID(mParentHana->getCarID());
    }
private:
    // Unused
    static u16 sCreateNum;

    static const Vec sTireOfsTable[4];       // 0x803791a0
    static f32 sBodyToBodyLength;            // 0x804146d0
    static J3DAnmTextureSRTKey *sTireBtkAnm; // 0x80416450

    GeoHanaBodyShadow *mShadow;
    GeoHanaCar *mParentHana;
    GeoCar *mParentCar;
    J3DAnmObjMaterial mTireMat;
    CrsArea *mArea;
}; // class GeoHanaBody

#endif // GEOHANABODY_H
