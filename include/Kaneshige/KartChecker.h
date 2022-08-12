#ifndef KARTCHECKER_H
#define KARTCHECKER_H

#include "types.h"

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbPrint.h"

#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/Course/Course.h"

#include "Osako/kartPad.h"

#define MAX_FRAME 2147483

class KartChecker
{
public:
    KartChecker(int, KartInfo *, int, int);

    void clrPass(int sectoridx)
    {
        int index = sectoridx / 32;
        int bitIndex = sectoridx % 32;
        cpBitfields[index] &= ~(1 << bitIndex);
    }

    int getRank() const
    {
        return mRank;
    }

    RaceTime *getBestLapTime() ;

    const RaceTime & getLapTime(int no)
    {
        bool valid = false;
        if(no >= 0 && mMaxLap)
            valid = true;

        if(!valid) {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 206, "range over: %d <= no=%d < %d", 0, no, mMaxLap);
            OSPanic(__FILE__, 206, "Halt");
        }
        return mLapTimes[no];
    }

    bool isBestTotalTimeRenewal(int);
    bool isBestLapTimeRenewal();
    bool isCurrentLapTimeRenewal();

    bool isLapRenewal() const
    {
        return mLapRenewal;
    }

    bool isFinalLap() const {
        return mLap == mMaxLap -1;
    }

    bool isFinalLapRenewal() const;

    bool isGoal() const
    {
        return mRaceEnd;
    }

    bool isPass(int sectoridx)
    {
        int index = sectoridx / 32;
        int bitIndex = sectoridx % 32;
        bool valid = false;
        if (index >= 0 && index < bitfieldCnt)
            valid = true;

        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 131, "range over: %d <= index=%d < %d", 0, index, bitfieldCnt);
            OSPanic(__FILE__, 131, "Halt");
        }
        return (cpBitfields[index] & (1 << bitIndex)) != false;
    }
    bool isPassAll(int sectorCnt);

    bool isReverse();

    // https://decomp.me/scratch/RWx4a
    void printPass(int x, int y)
    {
        for (int i = 0; i < bitfieldCnt; i++)
        {
            JUTReport(x, (y + 16) + (i * 16), "[%d]:%08X", i, cpBitfields[i]);
        }
    }

    void setGoal()
    {
        _0x78 = false;
        mRaceEnd = true;
    }

    void setForceGoal();

    void setGoalTime()
    {
        mTotalTime = mBestLapTimes[mMaxLap - 1];
        mGoalFrame = curFrame;
    }

    void setLapTime();

    void setLapChecking()
    {
        raceFlags |= 1;
    }

    void setBalloonCtrl()
    {
        raceFlags |= 2;
    }

    void setBombCtrl()
    {
        raceFlags |= 4;
    }

    void setRabbitCtrl()
    {
        raceFlags |= 8;
    }

    void setDemoRank()
    {
        raceFlags |= 16;
    }

    void setRank(int rank)
    {
        mRank = rank;
    }

    bool setPass(int index);

    void clrRank()
    {
        mRank = 0;
    }

    bool tstLapChecking() const
    {
        return raceFlags & 1;
    }

    bool tstBalloonCtrl() const
    {
        return raceFlags & 2;
    }

    bool tstBombCtrl() const
    {
        return raceFlags & 4;
    }

    bool tstRabbitCtrl() const
    {
        return raceFlags & 8;
    }

    bool tstDemoRank() const
    {
        return raceFlags & 16;
    }

    bool tstDead() const
    {
        return battleFlags & 4;
    }

    bool isDead() const
    {
        return tstDead();
    }

    bool isBombPointFull() const
    {
        return bombPoint >= sBombPointFull;
    }

    static bool isInsideSector(f32 unitDist)
    {
        return (unitDist >= 1.0f && unitDist < 0.0f);
    }

    void incLap()
    {
        if (mMaxLap > mLap)
            mLap++;
    }

    void incTime();

    bool isMaxTotalTime() const
    {
        return mTotalTime.isAvailable();
    }

    bool isUDValid();

    void reset();
    void setPlayerKartColor(KartInfo *);
    void createGamePad(KartInfo *);
    void clrCheckPointIndex();

    static Course::Sector *searchCurrentSector(f32 *, JGeometry::TVec3<f32> const &, Course::Sector *sector, int);
    void checkKart();
    void checkKartLap();
    void checkLap(bool);

    void calcRabbitTime();

    static int sPlayerKartColorTable[];
    static short sBombPointFull;

    // private: // i'm not really sure how else KartChkUsrPage got access to this
    u16 raceFlags;
    s16 mTargetKartNo;
    s32 sectorCount;
    s32 bitfieldCnt;
    s32 mMaxLap;
    s32 mBestLapIdx;     // i think this stores the index of the fastest lap
    RaceTime *mLapTimes; // i'm not sure of these 2 names, it could be the other way around or something completely different
    RaceTime *mBestLapTimes;
    s32 mPlayerKartColor;
    KartGamePad *mKartGamePads[2];
    bool mLapRenewal;
    bool mRaceEnd;
    u8 _0x2a; // only seems to get set in the constructor
    u8 _0x2b; // probably padding
    s32 mLap;
    f32 sectorProgression;
    s32 warpState;
    s32 mGeneration;
    s32 sectorIndex;
    Course::Sector *sector1;
    Course::Sector *sector2;
    f32 lapProgression;
    f32 prevlapProgression;
    f32 lapProgression2; // might be max Lap Progression
    f32 raceProgression;
    s32 *cpBitfields; // seems to store what checkpoint have been passed
    JGeometry::TVec3<f32> mPos;
    JGeometry::TVec3<f32> mPrevPos;
    JugemPoint *mJugemPoint;
    bool _0x78; // true = in race | false = finished
    u8 _0x79[3];
    s32 curFrame;
    s32 mGoalFrame;
    RaceTime mTotalTime;
    s32 mRank;
    u16 battleFlags;
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
