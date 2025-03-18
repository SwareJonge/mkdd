#ifndef GEOWATER_H
#define GEOWATER_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "Sato/GeographyObj.h"
#include "Sato/J3DAnmObject.h"
#include "Sato/StateObserver.h"

class GeoWater;

class GeoWaterSupervisor : public GeoObjSupervisor, JKRDisposer
{
public:
    GeoWaterSupervisor();
    virtual ~GeoWaterSupervisor(); // 0x801c4378
    virtual void reset();
    virtual void calc();
    void entry(GeoWater *);
    GeoWater *searchWater(s32);
private:
    JSUList<GeoWater> mList;
};

class GeoSubWater : public GeographyObj
{
public:
    GeoSubWater(u32);                              // 0x801c66bc
    virtual void reset();                          // 0x801c671c
    virtual void createModel(JKRSolidHeap *, u32); // 0x801c6760
    virtual ~GeoSubWater() {}                      // 0x801c67e4
    virtual void calc() {}                         // 0x801c6be8

    void setPosition(JGeometry::TVec3f &);         // 0x801c6764
    // Inline
    void getPosition(JGeometry::TVec3f *out) const { out->set(mPos); }

private:
    friend class GeoWater;
};

class GeoWaterUp2 : public GeoSubWater
{
public:
    GeoWaterUp2();
    virtual void reset();
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void simpleDraw(u32);
    virtual void calc();
    virtual ~GeoWaterUp2() {}                                          // 0x801c6b10
    virtual const char *getBmdFileName() { return "/Objects/sea2_tex.bmd"; } // 0x801c6bdc
private:

    static J3DAnmTextureSRTKey *sBtkAnm;

    J3DAnmObjMaterial mMat;
};

class GeoWaterUp : public GeoSubWater
{
public:
    GeoWaterUp();
    virtual void simpleDraw(u32);
    virtual ~GeoWaterUp() {}                                            // 0x801c6b84
    virtual const char *getBmdFileName() { return "/Objects/sea3_dark.bmd"; } // 0x801c6bdc
};

class GeoWaterDown : public GeoSubWater
{
public:
    GeoWaterDown();
    virtual void simpleDraw(u32);
    virtual ~GeoWaterDown() {}            // 0x801c6bec
    virtual const char *getBmdFileName() { return "/Objects/sea5_sand.bmd"; } // 0x801c6c44
};

class GeoWaterMiddleD : public GeoSubWater
{
public:
    GeoWaterMiddleD();

    virtual void reset();
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void simpleDraw(u32);
    virtual void calc();
    virtual ~GeoWaterMiddleD() {} // 0x801c6c50
    virtual const char *getBmdFileName() { return "/Objects/sea4_nami.bmd"; } // 0x801c6ca8
};

class GeoWater : public GeographyObj, StateObserver
{
public:
    GeoWater(const CrsData::SObject &);            // 0x801c45f8
    virtual void reset();                          // 0x801c4780
    virtual void loadAnimation();                  // 0x801c49b4
    virtual void createModel(JKRSolidHeap *, u32); // 0x801c4a08
    virtual void simpleDraw(u32);                  // 0x801c4a70
    virtual void update();                         // 0x801c4abc
    virtual void calc();                           // 0x801c4adc
    f32 getSurfaceHeight();                        // 0x801c4c90
    void initFuncWait();                           // 0x801c4c98
    void doFuncWait();                             // 0x801c4c9c

    virtual void InitExec(); // 0x801c4b28
    virtual void MoveExec(); // 0x801c4c3c

    // Inline/Unused
    // void sCreateNum;
    // Inline
    bool isSameID(s32 id) const { return mObjData->mParam1 == id; }; // 0x801c4594
    bool isFlow() const { return mAngle >= -0x4000 && mAngle < 0x4000; }

    //virtual ~GeoWater() {}                                             // 0x801c4e94
    virtual const char *getBmdFileName() { return "/Objects/sea1_spc.bmd"; } // 0x801c4f1c
    virtual void initClassCreateNum() { sSupervisorCreateNum = 0; }    // 0x801c4f28
    virtual GeoWaterSupervisor *getSupervisor() { return sSupervisor; }
    static GeoWaterSupervisor *getSupervisor() { return sSupervisor; }
 
    friend class GeoWaterSupervisor;
private:
    static StateFuncSet<GeoWater> sTable[1];       // 0x80396f88
    static f32 sStartAngle;                 // 0x804146d8
    static u16 sSupervisorCreateNum;        // 0x80416460
    static GeoWaterSupervisor *sSupervisor; // 0x80416464
    static J3DAnmTextureSRTKey *sBtkAnm;    // 0x80416468
    
    JSULink<GeoWater> mLink;
    GeoSubWater *mSubs[4];
    JGeometry::TVec3f mFrDir;
    JGeometry::TVec3f mUpDir;
    JGeometry::TVec3f mLfDir;
    f32 _19c;
    f32 _1a0;
    s16 mAngle;
    s16 mAngleInc;
    f32 _1a8;
    f32 _1ac;
    f32 _1b0;
    f32 _1b4;
    f32 _1b8;
    J3DAnmObjMaterial mMat;

}; // class GeoWater

#endif // GEOWATER_H
