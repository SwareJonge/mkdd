#ifndef RACELIGHT_H
#define RACELIGHT_H

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/LightMgr.h"

class RaceSceneLight : public LtObjDiffuse {
public:
    RaceSceneLight(const char *, u32, const JUtility::TColor &, const JGeometry::TVec3<float> &);
    void calc();
private:
    u8 _28[0xf4 - 0x38];
};

class RaceBalloonLight : public LtObjDiffuse {
public:
    RaceBalloonLight(u32);
private:
    u8 _28[0xf4 - 0x38];
};

class RaceKartLight : public LtObjDiffuse {
public:
    RaceKartLight(RaceSceneLight *, int);
    void calc();
    void draw();
private:
    u8 _28[0x128 - 0x38];
};

class RaceCupLight : public LtObjDiffuse {
public:
    RaceCupLight(RaceSceneLight *);
    void calc();
    void draw();
private:
    u8 _28[0xd0 - 0x38];
};

#endif