#ifndef RACEDIRECTOR_H
#define RACEDIRECTOR_H

#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/RaceTime.h"
#include "kartEnums.h"

class RaceDirector
{
public:
    enum ERace2DMode {
        Race2DMode_0,
        Race2DMode_1,
    };

    static u8 sForceStopFrame;
    static s16 sForceWaitDemoTime;
    static s16 sRace2DDelayTime;
    static const int cFadeOutTime;

    // Global
    RaceDirector(bool, DemoTimeKeeper *, int); // 0x801c752c
    void reset(); // 0x801c7680
    void resetCommon(); // 0x801c76d0
    void appearRace2D(ERace2DMode); // 0x801c775c
    void calc(); // 0x801c77bc
    void doCrsDemo(); // 0x801c78d4
    void doNoCrsDemo(); // 0x801c7968
    void checkCrsDemoEvent(); // 0x801c79b0
    void checkCrsDemoEventHanabi(s16); // 0x801c7afc
    void doReset(); // 0x801c7b74
    void doWait(); // 0x801c7b80
    void doSafety(); // 0x801c7bb4
    void doRunning(bool); // 0x801c7c14
    s16 checkZoomWinConsoleNo(); // 0x801c8008
    bool isAllGoalAnmEnd(); // 0x801c8108
    void doEnding(); // 0x801c81a8
    void setEndingPhaseZoomConsole(); // 0x801c8608
    void checkPauseChoice(); // 0x801c865c
    bool isExitWaitDemo(); // 0x801c8808
    void setPhaseWait(ERacePhase phase, bool fadeAudio, bool fadeVideo, int fadeOutTime=cFadeOutTime); // 0x801c88a8
    bool checkRaceEnd(); // 0x801c8a24
    bool checkRaceEndForRace(); // 0x801c8afc
    bool checkRaceEndForBalloonBattle(); // 0x801c8c10
    bool checkRaceEndForBombBattle(); // 0x801c8c84
    bool checkRaceEndForEscapeBattle(); // 0x801c8cf8
    int requestDemoCameraID() const; // 0x801c8d60
    bool isPlayingDemoWinnerAnm() const; // 0x801c8dac
    u32 getDemoCameraTagName() const; // 0x801c8df8
    bool isFrameRenewal() const {
#ifdef DEBUG
        bool ret = false;
        if (mFrameRenewal && !sForceStopFrame) {
            ret = true;
        }
        return ret;
#else
        return mFrameRenewal;
#endif
    }
    bool isRaceEnd() const { return mIsRaceEnd; };
    bool isZoomWinConsole() const { return mZoomWinCnsNo >= 0; }
    ERacePhase getRacePhase() const { return mRacePhase; };

    int getWaitDemoResult() const { return mWaitDemoResult; }
    const RaceTime &getMiniGameTime() { return mMinigameTime; }

private:
    DemoTimeKeeper *mTimeKeeper;
    ERace2DMode mRace2DMode;
    s16 mRace2DDelayTime;
    ERacePhase mRacePhase;
    ERacePhase _10;
    int mRunningState;
    int mWaitDemoTimer;
    int mWaitDemoResult;
    int mWaitTimer;
    int mFadeOutTime;
    bool mFrameRenewal;
    u8 _29;
    bool mIsRaceEnd;
    s16 mZoomWinCnsNo;
    int mRaceRunState;
    int mEndingState;
    bool mPlayingWinAnm;
    bool mMovingToStart;
    bool _3a;
    s16 _3c;
    s16 _3e;
    int mMiniGameFrame;
    RaceTime mMinigameTime;
}; // Size 0x48

#endif
