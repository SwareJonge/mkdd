#ifndef RACELIGHT_H
#define RACELIGHT_H

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/LightMgr.h"

class RaceSceneLight : public LtObjDiffuse {
public:
    RaceSceneLight(const char *, u32, const JUtility::TColor &, const JGeometry::TVec3<float> &);
    void calc();
};

class RaceBalloonLight : public LtObjDiffuse {
public:
    RaceBalloonLight(u32);
};

class RaceKartLight : public LtObjDiffuse {
public:
    RaceKartLight(RaceSceneLight *, int);
    void calc();
    void draw();
};

class RaceCupLight : public LtObjDiffuse {
public:
    RaceCupLight(RaceSceneLight *);
    void calc();
    void draw();
};

#endif