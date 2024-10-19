#ifndef RACELIGHT_H
#define RACELIGHT_H

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/LightMgr.h"

class RaceSceneLight : public LtObjDiffuse
{
public:
    RaceSceneLight(const char *, u32, const JUtility::TColor &, const JGeometry::TVec3f &);
    void getTagName(u32); // UNUSED

    virtual ~RaceSceneLight();     // overide
    virtual MtxPtr getEffectMtx(); // overide
    virtual void calc();           // overide
private:
    u8 _90[0xf4 - 0x90];
};

class RaceBalloonLight : public LtObjDiffuse
{
public:
    RaceBalloonLight(u32);
    virtual ~RaceBalloonLight();   // overide
    virtual MtxPtr getEffectMtx(); // overide
    virtual void draw();           // overide

private:
    u8 _90[0xf4 - 0x90];
};

class RaceKartLight : public LtObjDiffuse
{
public:
    RaceKartLight(RaceSceneLight *, int);
   
    virtual ~RaceKartLight();      // overide
    virtual MtxPtr getEffectMtx(); // overide
    virtual void draw();           // overide
    virtual void calc();           // overide
    
private:
    u8 _90[0x128 - 0x90];
};

class RaceCupLight : public LtObjDiffuse
{
public:
    RaceCupLight(RaceSceneLight *);

    virtual ~RaceCupLight(); // overide
    virtual void draw();     // overide
    virtual void calc();     // overide

private:
    u8 _90[0xd0 - 0x90];
};

#endif
