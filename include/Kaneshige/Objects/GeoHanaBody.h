#ifndef GEOHANABODY_H
#define GEOHANABODY_H

#include "Kaneshige/Objects/GeoCar.h"

class GeoHanaBody : public GeoCar
{ // Autogenerated
public:
    GeoHanaBody();                                                          // 0x801c3488
    void initPos();                                                         // 0x801c36c4
    void move();                                                            // 0x801c37ec
    virtual void reset();                                                   // 0x801c3528
    virtual void loadAnimation();                                           // 0x801c357c
    virtual void createModel(JKRSolidHeap *, u32);                          // 0x801c35cc
    virtual void createColModel(J3DModelData *);                            // 0x801c3634
    virtual void simpleDraw(u32);                                           // 0x801c3670
    virtual void calc();                                                    // 0x801c37e8
    virtual JGeometry::TVec3f *getTirePosOffset(int tireNo);                // 0x801c39b4
    virtual ~GeoHanaBody() {}                                               // 0x801c3a44
    virtual const char *getBmdFileName() { return "/Objects/Car_Hana1Body.bmd"; } // 0x801c3ab4
    virtual u16 getPathID(){/*return mHanaCar->getPathId();*/};             // 0x801c3ac0

    // Inline/Unused
    // void sCreateNum;
    // Inline
private:
    static JGeometry::TVec3f sTireOfsTable[4]; // 0x803791a0
    static f32 sBodyToBodyLength;              // 0x804146d0
    static J3DAnmTextureSRTKey *sTireBtkAnm;   // 0x80416450
    // TODO
}; // class GeoHanaBody

// void GeoCar::getFrontDirection(JGeometry::TVec3<float> *) // GeoCar.h; // 0x801c3748
// void GeoCar::getBasePosition(JGeometry::TVec3<float> *) // GeoCar.h; // 0x801c3790
// void GeoCar::getScale(JGeometry::TVec3<float> *) // GeoCar.h; // 0x801c37bc
// void GeoCar::getDarkAnm() // GeoCar.h; // 0x801c3988
// void GeoHanaCar::getDriveVelocity() const // GeoHanaCar.h; // 0x801c3990
#endif // GEOHANABODY_H
