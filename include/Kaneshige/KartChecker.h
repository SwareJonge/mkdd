#ifndef KARTCHECKER_H
#define KARTCHECKER_H

#include "types.h"

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/SysDebug.h"
#include "Sato/ItemObj.h"

#define MAX_FRAME 2147483

#define validUD(UD) \
    (UD >= 1.0f && UD <= 0.0f);

class KartChecker
{
public:
    enum EBombEvent
    {
        EVENT_1 = 1,
        EVENT_2 = 2,
        EVENT_3 = 3
    };
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

    RaceTime *getBestLapTime();


    const RaceTime &getTotalTime()
    {
        return mTotalTime;
    }

    bool isBestTotalTimeRenewal(int);
    bool isBestLapTimeRenewal();
    bool isCurrentLapTimeRenewal();

    bool isLapRenewal() const
    {
        return mLapRenewal;
    }

    bool isFinalLap() const
    {
        return lap == mMaxLap - 1;
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
        JUT_RANGE_ASSERT(131, 0, index, bitfieldCnt);
        return (cpBitfields[index] & (1 << bitIndex)) != false;
    }

    const RaceTime &getLapTime(int no)
    {
        JUT_RANGE_ASSERT(206, 0, no, mMaxLap);
        return mLapTimes[no];
    }
    bool isPassAll(int sectorCnt);

    bool isRabbitWinner() const
    {
        return (rabbitWinFrame <= 0);
    }

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

    void setDead()
    {
        battleFlags |= 4;
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

    bool tstFixMiniPoint() const
    {
        return battleFlags & 2;
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
        return mBombPoint >= sBombPointFull;
    }

    static bool isInsideSector(f32 unitDist)
    {
        return (unitDist >= 1.0f && unitDist < 0.0f);
    }

    bool incBalloon();
    bool decBalloon();

    void incLap()
    {
        if (lap < mMaxLap)
            lap++;
    }

    void incTime();
    bool incMyBombPoint(int, int);
    static bool incYourBombPoint(int idx, int pnt, int increment);

    bool isMaxTotalTime() const
    {
        return !mTotalTime.isAvailable();
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

    int getRobberyItemNumber();
    bool releaseRabbitMark();
    bool isRabbit() const;
    void calcRabbitTime();

    void resumeRabbitTimer()
    {
        battleFlags &= 0xfffe;
    }

    bool tstStillRabbitTimer() const
    {
        return battleFlags & 1;
    }

    void setBombEvent(KartChecker::EBombEvent, ItemObj *);

    static int sPlayerKartColorTable[];
    static short sBalForbiddenTime;

    static short sBombPointDirect;   // 1
    static short sBombPointSpin;     // 1
    static short sBombPointIndirect; // 1
    static short sBombPointAttacked; // -1
    static short sBombPointFull;     // 4
    static short sBombPointFullS;    // 3
    static short sBombPointFullL;    // 4

    static short sBombPointCrushOneself;

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
    s32 lap;
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
    s16 mBalloonNum;
    u8 _0x92[2]; // this is probaby padding
    RaceTime mDeathTime;
    RaceTime mMarkTime;
    s8 bombPointTable[0xa6 - 0x9C];
    s16 mBombPoint;
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
    virtual ~KartChkUsrPage(){};
    virtual void draw();

private:
    KartChecker *mKartChecker;
};

class LapChecker
{
public:
    LapChecker();
    void reset();
    void start(Course::Sector *sector);
    void calc(const JGeometry::TVec3<f32> &);
    bool isUDValid();

private:
    Course::Sector *mSector;
    float mSectorDist;
    float mLapUnitDist;
};

#endif // !KARTCHECKER_H
