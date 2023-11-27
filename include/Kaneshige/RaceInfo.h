#ifndef RACEINFO_H
#define RACEINFO_H

#include "types.h"

#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"

enum ERaceMode
{
    INV_MODE = 0,
    TIME_ATTACK = 0x1,
    GRAND_PRIX = 0x2,
    VERSUS_RACE = 0x3,
    BALLOON_BATTLE = 0x4,
    ROBBERY_BATTLE = 0x5,
    BOMB_BATTLE = 0x6,
    ESCAPE_BATTLE = 0x7,
    AWARD_DEMO = 0x8,
    STAFF_ROLL = 0x9,
};

enum ERaceLevel
{ // unsure of this
    LVL_INV = -1,
    LVL_50CC = 0,
    LVL_100CC = 1,
    LVL_150CC = 2,
    LVL_MIRROR = 3,
};

enum ERaceGpCup
{
    INV_CUP = -1,
    MUSHROOM_CUP = 0,
    FLOWER_CUP = 1,
    STAR_CUP = 2,
    SPECIAL_CUP = 3,
    ALL_CUP_TOUR = 4
};

class RaceInfo
{
public:
    RaceInfo();
    ~RaceInfo() { }

    static u16 sWaitDemoSelector;
    static ERaceGpCup sAwardDebugCup;

    static u32 sForceDemoNo;
    static u32 sForceRandomSeed;
    static ERaceLevel sAwardDebugLevel;
    static s16 sAwardDebugRank;

    static EKartID sAwardDebugKartIDTable[];
    static ECharID sAwardDebugDriver1IDTable[];
    static ECharID sAwardDebugDriver2IDTable[];
    
    void reset();
    void setConsoleTarget(int idx, int p2, bool p3);
    void settingForWaitDemo(bool demoSettingThing);
    void settingForAwardDemo();
    void settingForStaffRoll(bool trueEnding);
    void setRace(ERaceMode RaceMode, int kartCount, int playerCount, int consoleCount, int p5);
    void setRaceLevel(ERaceLevel raceLvl);
    void shuffleRandomSeed();
    void shuffleStartNo();
    void hideConsole(u32 param_2);
    void setKart(int, EKartID, ECharID, KartGamePad *, ECharID, KartGamePad *);

    // Inline Functions
    int getLANLapNumber() const { return mLapNumLAN; }
    int getVSLapNumber() const { return mVsLapNum; }
    int getKartNumber() const { return mKartNum; }
    int getConsoleNumber() const { return mConsoleNum; }
    int getStatusNumber() const { return mStatusNum; }
    ERaceMode getRaceMode() const { return mRaceMode; }
    int getItemSlotType() const { return mItemSlotType; }
    ERaceLevel getRaceLevel() const { return mRaceLevel; }
    ERaceGpCup getGpCup() const { return mGpCup; }

    bool isLANMode() { return mIsLanMode; }
    bool isTrueEnding() const { return mIsTrueEnding; }
    bool isMirror() const { return mIsMirror; }
    bool isWaitDemo() const  {return mDemoType != 0; }
    bool isDriverLODOn() const  { return (mLOD & 2); };
    bool isHiddingConsole(u32 viewNo) const { return (mHidingConsoles & 1 << viewNo) != 0; }
    void setAwardKartNo(int kartNo) { mAwardKartNo = kartNo; }
    void setGpCup(ERaceGpCup cup) { mGpCup = cup; }
    void setRandomSeed(u32 value) { mRandomSeed = value; }

    void setRivalKartNo(int rivalNo, int kartNo) {
#line 114
        JUT_MINMAX_ASSERT(0, rivalNo, 2);
        JUT_MINMAX_ASSERT(0, kartNo, 8)
        mRivalKarts[rivalNo] = kartNo;
    }

    int getConsoleTarget(int cnsNo) const {
#line 124
        JUT_MINMAX_ASSERT(0, cnsNo, 4);
        return mTargetKarts[cnsNo];
    }

    bool isDemoConsole(int cnsNo) const {
        JUT_MINMAX_ASSERT(0, cnsNo, 4);
        return mIsDemoKart[cnsNo];
    }

    KartInfo *getKartInfo(int kartNo) {
#line 173
        JUT_MINMAX_ASSERT(0, kartNo, 8);
        return &mKartInfo[kartNo];
    }

//private:
    bool mIsTinyProcess;
    bool mIsLanMode;
    bool mIsTrueEnding;
    u32 mRandomSeed;
    ERaceMode mRaceMode;
    ERaceGpCup mGpCup;
    ERaceLevel mRaceLevel;
    int mItemSlotType; // perhaps this is an enum too
    s16 mVsLapNum;
    s16 mLapNumLAN;
    s16 mKartNum;
    s16 mPlayerNum;
    s16 mConsoleNum;
    s16 mStatusNum;
    u16 mLOD;
    s16 mGpStageNo;
    int mDemoType;
    bool mIsMirror;
    // padding bytes
    KartInfo mKartInfo[8];
    s16 mStartPosIndex[8];
    s16 mPointTable[8];
    s16 mRivalKarts[2];
    s16 mTargetKarts[4];
    bool mIsDemoKart[4];
    s16 mAwardKartNo;
    // padding bytes
    int mDemoNextPhase;
    s16 mRank[8]; // stores what rank you finished at previous race, basically the same as startPosIndex
    RaceTime mFinishTime[8];
    RaceTime mLapTimes[8][10];
    int mWaitDemoResult; 
    u16 mHidingConsoles;
};

extern RaceInfo gRaceInfo;

#endif // !RACEINFO_H
