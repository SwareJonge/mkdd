#include "Inagaki/GameAudioMain.h"

#include "Kaneshige/RaceBGMPlayer.h"
#include "Kaneshige/RaceMgr.h"

#include <JSystem/JAudio/JASFakeMatch2.h>

s16 RaceBGMPlayer::sCrsBGMDelayTime = 60;

RaceBGMPlayer::RaceBGMPlayer() { reset(); }

void RaceBGMPlayer::reset()
{
    mState = 1;
    mFlags = 0;
    mDelay = 0;
    mFinalLapKartNo = -1;
    mGoalKartNo = -1;
    mLowestKartNo = -1;
    _e = false;

    if (RaceMgr::getManager()->isCrsDemoMode())
        GetGameAudioMain()->startSequenceBgm(getCrsDemoBGMLabel());
    else
        stopCrsDemo();
}

u32 RaceBGMPlayer::getCrsDemoBGMLabel()
{
    u32 ret = 0x1000004;

    if (RaceMgr::getManager()->isRaceModeMiniGame())
        ret = 0x1000007;

    return ret;
}

u32 RaceBGMPlayer::getRaceStartBGMLabel()
{
    u32 ret = 0x1000005;

    if (RaceMgr::getManager()->isRaceModeTA())
        ret = 0x1000006;
    else if (RaceMgr::getManager()->isRaceModeVs())
        ret = 0x1000006;
    else if (RaceMgr::getManager()->isRaceModeMiniGame())
        ret = 0x1000008;
    return ret;
}

void RaceBGMPlayer::calc()
{
    Course *course = RaceMgr::getManager()->getCourse();
    if (RaceMgr::getManager()->isLANMode() && !_e && mLowestKartNo >= 0)
    {
        mState = 5;
        mDelay = 0;
        mGoalKartNo = mLowestKartNo;
    }

    switch (mState)
    {
    case 1:
    {
        if (isCrsDemoEnd())
        {
            if (!RaceMgr::getManager()->isWaitDemoMode() && !RaceMgr::getManager()->isAwardDemoMode() && !RaceMgr::getManager()->isStaffRoll())
                GetGameAudioMain()->startSequenceBgm(getRaceStartBGMLabel());
            mState = 2;
        }
        break;
    }
    case 2:
    {
        if (isPrepare())
        {
            bool doPrepare = true;
            if (RaceMgr::getManager()->isAwardDemoMode() && GetGameAudioMain()->getPlayingStreamID() == 0x2000022)
                doPrepare = false;

            if (doPrepare)
            {
                GetGameAudioMain()->prepareStreamBgm(course->getCourseBGM());
                execPrepare();
            }
            mState = 3;
            mDelay = sCrsBGMDelayTime;
        }
        break;
    }
    case 3:
    {
        if (isPlay())
        {
            mDelay--;
        }
        if (mDelay <= 0)
        {
            if (RaceMgr::getManager()->isRaceModeMiniGame())
            {
                mState = 5;
            }
            else
            {
                mState = 4;
            }

            if (isExecPrepare())
            {
                GetGameAudioMain()->startStreamBgm(course->getCourseBGM());
            }
        }
        break;
    }
    case 4:
    {
        if (mFinalLapKartNo >= 0 || isLastTime())
        {
            mState = 5;
            mDelay = 0;
            GetGameAudioMain()->prepareStreamBgm(course->getFinalLapBGM());
            GetGameAudioMain()->startSequenceBgm(0x100000a);
        }
        break;
    }
    case 5:
    {
        if (mGoalKartNo >= 0 || isMiniGameEnd())
        {
            mState = 6;
            _e = true;
            if (!RaceMgr::getManager()->isRaceModeMiniGame())
            {
                mDelay = GameAudio::GoalFanfareDelayFrame;
            }
            else
            {
                mDelay = 0;
            }
            GetGameAudioMain()->prepareStreamBgm(getGoalBGMLabel());
        }
        break;
    }
    case 6:
    {
        mDelay--;
        if (mDelay <= 0)
        {
            mState = 7;
            mDelay = 0;
            GetGameAudioMain()->startStreamBgm(getGoalBGMLabel());
        }
        break;
    }
    }
}

u32 RaceBGMPlayer::getGoalBGMLabel()
{
    if (RaceMgr::getManager()->isRaceModeMiniGame())
        return 0x2000026;

    if (RaceMgr::getManager()->isRaceModeTA())
    {
        KartChecker *kartChecker = RaceMgr::getManager()->getKartChecker(0);
        if (kartChecker->isBestTotalTimeRenewal(0))
            return 0x2000020;
        if (kartChecker->isBestTotalTimeRenewal(4) || kartChecker->isBestLapTimeRenewal())
            return 0x2000026;
        else
            return 0x200002a;
    }

    int rank = 0;

    if (mGoalKartNo >= 0)
        rank = RaceMgr::getManager()->getKartChecker(mGoalKartNo)->getRank();

    // Removing the temp var ret causes MW to optimize the if statements

    if (!RaceMgr::getManager()->isLANMode())
    {
        u32 ret;
        if (rank <= 1)
            ret = 0x2000020;
        else if (rank <= 3)
            ret = 0x2000021;
        else
            ret = 0x2000022;
        return ret;
    }

    u32 ret;
    int kartNum = RaceMgr::getManager()->getKartNumber();
    
    if (rank <= 1)
        ret = 0x200002f;
    else if (rank <= kartNum - 1)
        ret = 0x2000030;
    else
        ret = 0x2000031;
    return ret;

}