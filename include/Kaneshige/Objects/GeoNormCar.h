#ifndef GEONORMCAR_H
#define GEONORMCAR_H

#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Kaneshige/Objects/GeoCar.h"

class GeoNormCar : public GeoCar
{
public:
    GeoNormCar(const CrsData::SObject &);                                 // 0x801beb00
    virtual void reset();                                                 // 0x801bed30
    virtual void createModel(JKRSolidHeap *, u32);                        // 0x801beda0
    virtual void createColModel(J3DModelData *);                          // 0x801bee68
    virtual void loadAnimation();                                         // 0x801bef20
    virtual void simpleDraw(u32);                                         // 0x801bef94
    virtual void calc();                                                  // 0x801bf00c
    virtual void update();                                                // 0x801bf0b4
    virtual const Vec &getTirePosOffset(int tireNo);                      // 0x801bf11c
    //virtual ~GeoNormCar() {}                                              // 0x801bf1ac
    virtual const char *getBmdFileName() { return "/Objects/Car_Public1.bmd"; } // 0x801bf228
    virtual u32 getSoundCarType() const { return 0; }                     // 0x801bf234
    // Inline/Unused
    // void resetAnimation();
    // static u32 sCreateNum;

    // Inline
    bool isTireBtkEnable() const { return sTireBtkAnm != nullptr; }; // 0x801bee30
private:
    static const Vec sTireOfsTable[4]; // 0x80378c50
    static J3DAnmTexPattern *sBodyBtpAnm;      // 0x80416418
    static J3DAnmTextureSRTKey *sTireBtkAnm;   // 0x8041641c
    
    GeoCarShadow *mShadow;
    f32 mFrame;
    J3DAnmObjMaterial mBodyMat;
    J3DAnmObjMaterial mTireMat;
    f32 _2a8;
};     // class GeoNormCar
#endif // GEONORMCAR_H
