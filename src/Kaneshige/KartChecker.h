#ifndef KARTCHECKER_H
#define KARTCHECKER_H

#include <types.h>
#include <JSystem/JGeometry.h>

#include "KartInfo.h"
#include "RaceTime.h"
//#include "JugemPoint.h"
//#include "Course.h"
//#include "KartGamePad"


class KartChecker {
public:
    KartChecker(int, KartInfo *, int, int);

    void setPlayerKartColor(KartInfo *);
private:
    u16 raceFlags;
    s16 kartIndex;
    s32 sectorCount;
    s32 bitfieldCnt;
    s32 trackLapCount;
    s32 _0x10; // i think this stores the index of the fastest lap
    RaceTime* laptimes1;
    RaceTime* laptimes2;
    s32 playerKartColor;
    void* kartGamePad1;
    void* kartGamePad2;
    bool showLapSplit;
    bool raceEnd;
    u8 _0x2a; // only seems to get set in the constructor
    u8 _0x2b; // probably padding
    s32 lapIndex;
    f32 sectorProgression;
    s32 warpState;
    s32 _0x38;
    s32 sectorIndex;
    Course::Sector * sector1;
    Course::Sector * sector2;
    f32 lapProgession;
    f32 prevlapProgession;
    f32 lapProgression2; // might be max Lap Progression
    f32 raceProgression;
    s32* cpBitfields; // seems to store what checkpoint have been passed
    TVec3<f32> curPos;
    TVec3<f32> prevPos;
    JugemPoint * jugemPoint;
    bool _0x78; // true = in race | false = finished
    u8 _0x79[3];
    s32 curFrame;
    s32 goalFrame;
    RaceTime Timer;
    s32 rank;
    s16 battleFlags;
    s16 _0x8e;
    s16 balloonNumber;
    u8 _0x92[2]; // this is probaby padding
    RaceTime deathTime;
    RaceTime markTime;
    u8 _0x9c[010];
    s16 bombPoint;
    s16 rabbitWinFrame;
    s32 demoPoint;
    // these only get set in the constructor?
    TVec3<f32> _0xb0;
    s32 _0xbc;
};

#endif // !KARTCHECKER_H
