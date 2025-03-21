#ifndef GEOBUS_H
#define GEOBUS_H

#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/J3DAnmObject.h"
#include "dolphin/mtx.h"

class GeoBus : public GeoCar
{ // Autogenerated
public:
    GeoBus(const CrsData::SObject &);                 // 0x801bd234
    virtual void reset();                             // 0x801bd4a4
    virtual void createModel(JKRSolidHeap *, u32);    // 0x801bd530
    virtual void loadAnimation();                     // 0x801bd5d8
    virtual void createColModel(J3DModelData *);      // 0x801bd644
    virtual const Vec &getTirePosOffset(int);         // 0x801bd748
    virtual void simpleDraw(u32);                     // 0x801bd7d8
    virtual void calc();                              // 0x801bd840

    //virtual ~GeoBus() {}                                                // 0x801bd8c0, overide
    virtual const char *getBmdFileName() { return "/Objects/Car_Bus1.bmd"; }; // 0x801bd93c, overide
    virtual u32 getMotorType() const { return 3; };                     // 0x801bd948, overide
private:
    static const Vec sTireOfsTable[4];       // 0x80378a60
    static J3DAnmTexPattern *sBodyBtpAnm;    // 0x804163f8
    static J3DAnmTextureSRTKey *sTireBtkAnm; // 0x804163fc
    // Unused
    // static u32 sCreateNum;

    GeoCarShadow *mShadow;
    f32 mFrame;
    J3DAnmObjMaterial mBodyMat;
    J3DAnmObjMaterial mTireMat;
    f32 _2a8;
}; // class GeoBus

#endif // GEOBUS_H
