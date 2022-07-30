#ifndef KARTCHECKER_H
#define KARTCHECKER_H

#include "types.h"

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbPrint.h"

#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/Course/Course.h"

#include "Osako/kartPad.h"

//#include "JugemPoint.h"
//#include "Course.h"

class KartChecker
{
public:
    KartChecker(int, KartInfo *, int, int);

    int getRank() const
    {
        return mRank;
    }

    bool isBestTotalTimeRenewal(int);
    bool isBestLapTimeRenewal();

    bool isLapRenewal() const
    {
        return mLapRenewal;
    }

    void printPass(int x, int y)
    {
        for (int i = 0; i < bitfieldCnt; i++)
        {
            JUTReport(x, (y + 16) + (i * 16), "[%d]:%08X", i, cpBitfields[i]);
        }
    }

    void setLapChecking()
    {
        raceFlags |= 1;
    }

    void setDemoRank()
    {
        raceFlags |= 16;
    }

    void setRabbitCtrl()
    {
        raceFlags |= 8;
    }

    void setBombCtrl()
    {
        raceFlags |= 4;
    }

    void setBalloonCtrl()
    {
        raceFlags |= 2;
    }

    void setRank(int rank)
    {
        mRank = rank;
    }

    void clrCheckPointIndex();

    void clrRank()
    {
        mRank = 0;
    }

    bool tstDemoRank() const
    {
        return raceFlags & 16;
    }

    void setPlayerKartColor(KartInfo *);
    void createGamePad(KartInfo *);
    void reset();

    // private: // i'm not really sure how else KartChkUsrPage got acces to this
    u16 raceFlags;
    s16 kartIndex;
    s32 sectorCount;
    s32 bitfieldCnt;
    s32 trackLapCount;
    s32 _0x10; // i think this stores the index of the fastest lap
    RaceTime *laptimes1;
    RaceTime *laptimes2;
    s32 playerKartColor;
    KartGamePad *kartGamePad1;
    KartGamePad *kartGamePad2;
    bool mLapRenewal;
    bool mRaceEnd;
    u8 _0x2a; // only seems to get set in the constructor
    u8 _0x2b; // probably padding
    s32 mLap;
    f32 sectorProgression;
    s32 warpState;
    s32 _0x38;
    s32 sectorIndex;
    Course::Sector *sector1;
    Course::Sector *sector2;
    f32 lapProgression;
    f32 prevlapProgression;
    f32 lapProgression2; // might be max Lap Progression
    f32 raceProgression;
    s32 *cpBitfields; // seems to store what checkpoint have been passed
    JGeometry::TVec3<f32> curPos;
    JGeometry::TVec3<f32> prevPos;
    void *jugemPoint;
    bool _0x78; // true = in race | false = finished
    u8 _0x79[3];
    s32 curFrame;
    s32 goalFrame;
    RaceTime mTotalTime;
    s32 mRank;
    s16 battleFlags;
    s16 mBalForbiddenTime;
    s16 balloonNumber;
    u8 _0x92[2]; // this is probaby padding
    RaceTime mDeathTime;
    RaceTime mMarkTime;
    s8 bombPointTable[0xa6 - 0x9C];
    s16 bombPoint;
    s16 rabbitWinFrame;
    s32 demoPoint;
    // these only get set in the constructor?
    JGeometry::TVec3<f32> _0xb0;
    s32 _0xbc;
};

class KartChkUsrPage : public SysDbUsrPage
{
public:
    KartChkUsrPage(KartChecker *kartChecker);
    virtual ~KartChkUsrPage();
    virtual void draw();

private:
    KartChecker *mKartChecker;
};

#endif // !KARTCHECKER_H
