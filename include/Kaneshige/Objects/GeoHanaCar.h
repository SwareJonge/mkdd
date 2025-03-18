#ifndef GEOHANACAR_H
#define GEOHANACAR_H

#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Kaneshige/Objects/GeoCar.h"

#define MAX_HANABODY_COUNT 20

class GeoHanaBody;

class GeoHanaCar : public GeoCar
{
public:
    GeoHanaCar(const CrsData::SObject &);                               // 0x801c2cf4
    void createBody();                                                  // 0x801c2f48
    void checkRoof();                                                   // 0x801c316c
    virtual void reset();                                               // 0x801c2ed8
    virtual void createModel(JKRSolidHeap *, unsigned long);            // 0x801c3018
    virtual void loadAnimation();                                       // 0x801c301c
    virtual void createColModel(J3DModelData *);                        // 0x801c3040
    virtual void simpleDraw(unsigned long);                             // 0x801c30b8
    virtual void calc();                                                // 0x801c30f4
    virtual const Vec &getTirePosOffset(int);                           // 0x801c3280
    //virtual ~GeoHanaCar() {}                                            // 0x801c3310
    virtual const char *getBmdFileName() { return "/Objects/Car_Hana1.bmd"; } // 0x801c3370
    virtual u32 getSoundCarType() const { return 3; };                  // 0x801c337c
    virtual void hitKart(int) {}                                        // 0x801c3384
    virtual void hitItemBomb() {}                                       // 0x801c3388
    virtual void hitItemWanWan() {}                                     // 0x801c338c
    virtual void hitItemGTurtleBig() {}                                 // 0x801c3390

    f32 getDriveVelocity() const { return _1b0.length(); }
private:
    static const Vec sTireOfsTable[4];         // 0x803790f8
    static s16 sHeadDownTime;                  // 0x804146c0
    static f32 sHeadDownHeight;                // 0x804146c4
    static f32 sHeadHeight;                    // 0x804146c8
    
    GeoHanaShadow *mShadow;
    s16 mBodyCount;
    GeoHanaBody *mBodies[MAX_HANABODY_COUNT];

}; // class GeoHanaCar

#endif // GEOHANACAR_H
