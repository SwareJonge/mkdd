#ifndef RACEINFO_H
#define RACEINFO_H

#include "kartEnums.h"
#include "types.h"

#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"

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
    int getAwardKartNo() const { return mAwardKartNo; }
    int getLANLapNumber() const { return mLapNumLAN; }
    int getVSLapNumber() const { return mVsLapNum; }
    int getKartNumber() const { return mKartNum; }
    int getPlayerKartNumber() const { return mPlayerNum; }
    int getConsoleNumber() const { return mConsoleNum; }
    int getStatusNumber() const { return mStatusNum; }
    int getGpStageNo() const { return mGpStageNo; }
    int getItemSlotType() const { return mItemSlotType; }
    ERaceMode getRaceMode() const { return mRaceMode; }
    ERacePhase getDemoNextPhase() const { return mDemoNextPhase; }
    ERaceLevel getRaceLevel() const { return mRaceLevel; }
    ERaceGpCup getGpCup() const { return mGpCup; }

    bool isLANMode() { return mIsLanMode; }
    bool isTinyProcess() const { return mIsTinyProcess; }
    bool isTrueEnding() const { return mIsTrueEnding; }
    bool isMirror() const { return mIsMirror; }
    bool isWaitDemo() const  {return mDemoType != 0; }
    bool isLastWaitDemo() const { return mDemoType == 2; }  
    bool isDriverLODOn() const  { return (mLOD & 2); };
    bool isHiddingConsole(u32 viewNo) const { return (mHidingConsoles & 1 << viewNo) != 0; }
    void setAwardKartNo(int kartNo) { mAwardKartNo = kartNo; }
    void setGpCup(ERaceGpCup cup) { mGpCup = cup; }
    void setRandomSeed(u32 value) { mRandomSeed = value; }
    void setItemSlotType(int type) { mItemSlotType = type; }

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
        return mKartInfo + kartNo;
    }

//private:
    bool mIsTinyProcess;       // 000
    bool mIsLanMode;           // 001
    bool mIsTrueEnding;        // 002
    u32 mRandomSeed;           // 004
    ERaceMode mRaceMode;       // 008
    ERaceGpCup mGpCup;         // 00C
    ERaceLevel mRaceLevel;     // 010
    int mItemSlotType;         // 014 perhaps this is an enum too
    s16 mVsLapNum;             // 018
    s16 mLapNumLAN;            // 01A
    s16 mKartNum;              // 01C
    s16 mPlayerNum;            // 01E
    s16 mConsoleNum;           // 020
    s16 mStatusNum;            // 022
    u16 mLOD;                  // 024
    s16 mGpStageNo;            // 026
    int mDemoType;             // 028
    bool mIsMirror;            // 02C
    KartInfo mKartInfo[8];     // 030
    s16 mStartPosIndex[8];     // 0F0
    s16 mPointTable[8];        // 100
    s16 mRivalKarts[2];        // 110
    s16 mTargetKarts[4];       // 114
    bool mIsDemoKart[4];       // 11C
    s16 mAwardKartNo;          // 120
    ERacePhase mDemoNextPhase; // 124
    s16 mRank[8];              // 128, stores what rank you finished at previous race, basically the same as startPosIndex
    RaceTime mFinishTime[8];   // 138
    RaceTime mLapTimes[8][10]; // 158
    int mWaitDemoResult;       // 298
    u16 mHidingConsoles;       // 29c
};

extern RaceInfo gRaceInfo;

#endif // !RACEINFO_H
