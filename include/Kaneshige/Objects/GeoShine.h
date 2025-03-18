#ifndef GEOSHINE_H
#define GEOSHINE_H

#include "JSystem/JGeometry/Vec.h"
#include "Sato/GeographyObj.h"
#include "Sato/JPEffectMgr.h"

class GeoRabbitMark;

class GeoShine : public GeographyObj { // Autogenerated
public:
    GeoShine();                            // 0x801d50fc
    void reset();                          // 0x801d5190
    void calcShadowModel();                // 0x801d53d4
    const char *getShadowBmdFileName();    // 0x801d559c
    void loadAnimation();                  // 0x801d55bc
    void createColModel(J3DModelData *);   // 0x801d55c0
    void createModel(JKRSolidHeap *, u32); // 0x801d55e8
    void simpleDraw(u32);                  // 0x801d55ec
    void calc();                           // 0x801d5618
    void move();                           // 0x801d561c
    void startFastRot();                   // 0x801d5a84

    // Inline
    virtual ~GeoShine() {} // 0x801d5aac
    const char *getBmdFileName() { return "/Objects/MiniGame_shine.bmd"; } // 0x801d5b04

    void setMark(GeoRabbitMark *mark) { mMark = mark; }
    void setConsoleNo(u32 no) { mConsoleNo = no; } 
    void enableWinEffect() { mWinEffectOn = true; }

private:
    static const char *sNormalEmitterName[3]; // 0x803980b8
    static const char *sWinnerEmitterName[2]; // 0x804147a8
    static JGeometry::TVec3f sShadowScale;    // 0x803fc288
    static f32 sScalingNearZ;                 // 0x804147b0
    static f32 sScalingFarZ;                  // 0x804147b4
    static f32 sMaxScale;                     // 0x804147b8
    static f32 sRotAnmMaxVel;                 // 0x804147bc
    static f32 sRotAnmVelNormal;              // 0x804147c0
    static f32 sRotAnmFastV0;                 // 0x804147c4
    static f32 sRotAnmResist;                 // 0x804147c8

    GeoRabbitMark *mMark;
    u32 mConsoleNo;
    JGeometry::TVec3f _154;
    JGeometry::TVec3f _160;
    JGeometry::TVec3f _16c;
    JGeometry::TVec3f _178;
    JGeometry::TVec3f _184;
    JGeometry::TVec3f _190;
    f32 mRotAnmVel;
    int _1a0; // duration of vel?
    bool mWinEffectOn;
    JPABaseEmitter *mNormalEmitter[3];
    JPABaseEmitter *mWinnerEmitter[2];
}; // class GeoShine

#endif // GEOSHINE_H
