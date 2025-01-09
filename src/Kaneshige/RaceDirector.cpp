#include "Kaneshige/RaceDirector.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JUtility/TColor.h"
#include "Kameda/J2DManager.h"
#include "Kameda/PauseManager.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/Objects/GeoRabbitMark.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Shiraiwa/MapObjHanabi.h"
#include "kartEnums.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

#ifdef DEBUG
u8 RaceDirector::sForceStopFrame;
s16 RaceDirector::sForceWaitDemoTime;
#endif

s16 RaceDirector::sRace2DDelayTime = 30;
const int RaceDirector::cFadeOutTime = 35;

RaceDirector::RaceDirector(bool waitForDemo, DemoTimeKeeper *demoTimeKeeper, int numConsoles) {
    mTimeKeeper = demoTimeKeeper;
    mRace2DMode = Race2DMode_0;
    mRace2DDelayTime = sRace2DDelayTime;

    if (numConsoles >= 3) {
        mRace2DMode = Race2DMode_1;
    }

    if (waitForDemo) {
        mRacePhase = PHASE_CRS_DEMO;
    }
    else if (RaceMgr::getManager()->isWaitDemoMode()) {
        mRacePhase = PHASE_RUNNING;
    }
    else {
        mRacePhase = PHASE_NO_CRS_DEMO;
    }

    resetCommon();
    if (RaceMgr::getManager()->isStaffRoll()) {
        GameAudioParameters::setDemoMode(2);
    }
    else {
        GameAudioParameters::setDemoMode(1);
    }
    GameAudioParameters::setMirrorSwitch(RaceMgr::getManager()->isMirror());

    bool isStartFade = true;
    if (mTimeKeeper) {
        isStartFade = mTimeKeeper->isStartFadeIn();
    }
    if (isStartFade) {
        SYSTEM_StartFadeIn(15);
    }
}

void RaceDirector::reset() {
    mRacePhase = PHASE_3;
    resetCommon();
    if (mTimeKeeper) {
        mTimeKeeper->reset();
    }
}

void RaceDirector::resetCommon() {
    mRunningState = 0;
    mRace2DDelayTime = sRace2DDelayTime;
    mWaitDemoTimer = 0;
    mWaitDemoResult = 0;
    mWaitTimer = 0;
    mFadeOutTime = cFadeOutTime;
    mFrameRenewal = true;
    _29 = 0;
    mIsRaceEnd = false;
    mZoomWinCnsNo = -1;
    _10 = PHASE_0;
    mRaceRunState = 0;
    mEndingState = 0;
    mPlayingWinAnm = false;
    mMovingToStart = false;
    _3a = false;
    _3c = -1;
    _3e = 0;
    mMiniGameFrame = 0;
    mMinigameTime.setFrame(mMiniGameFrame);
}

void RaceDirector::appearRace2D(ERace2DMode race2dMode) {
    if (!RaceMgr::getManager()->isReplayMode() && mRace2DMode == race2dMode) {
        GETJ2DManager()->setRace2DDrawFlag(true);
    }    
}

void RaceDirector::calc() {
    bool raceEnd = false;
    if (mRacePhase == PHASE_RUNNING ) {
        raceEnd = checkRaceEnd();
        if (raceEnd) {
            PauseManager::getManager()->setPauseEndFlag(true);
        }
    }
    PauseManager::getManager()->exec();
    if (!PauseManager::tstPause()) {
        mFrameRenewal = true;
    }
    else {
        mFrameRenewal = false;
    }

    if (mTimeKeeper) {
        mTimeKeeper->calc();
    }

    switch (mRacePhase) {
        case PHASE_CRS_DEMO: {
            doCrsDemo();
            break;
        case PHASE_NO_CRS_DEMO: {
            doNoCrsDemo();
            break;
        }
        case PHASE_3:
        case PHASE_4:
        case PHASE_5: {
            doReset();
            break;
        }
        case PHASE_RUNNING:
            doRunning(raceEnd);
            break;
        case PHASE_ENDING:
            doEnding();
            break;
        case PHASE_WAIT:
            doWait();
            break;
        case PHASE_SAFETY:
            doSafety();
            break;
        }
    }
}

void RaceDirector::doCrsDemo() {
    if (!RaceMgr::getManager()->isLANMode()) {
        checkCrsDemoEvent();
    }
    mWaitTimer++;
    switch (GETJ2DManager()->getPreRaceStatus()) {
    case 1: {
        RaceMgr::getManager()->mRaceBGMPlayer->stopCrsDemo();
        break;
    }
    case 2: {
        mRacePhase = PHASE_3;
        GameAudioParameters::setDemoMode(0);
        mWaitTimer = 0;
        break;
    }
    }
}

void RaceDirector::doNoCrsDemo() {
    mRacePhase = PHASE_3;
    RaceMgr::getManager()->mRaceBGMPlayer->stopCrsDemo();

    if (GameAudioParameters::getDemoMode() != 2) {
        GameAudioParameters::setDemoMode(0);
    }
}

void RaceDirector::checkCrsDemoEvent() {
    s16 startId = RCMGetCamera(0)->GetDemoCam()->mStartId;
    if (startId != _3c) {
        _3c = startId;
        _3e = 0;
    }

    CrsData::Camera *camData =RCMGetCourse()->getCrsData()->getCameraData(startId);
    if (RaceMgr::getManager()->isRaceModeGp()) {
        if (RaceMgr::getManager()->getGPStageNo() == 0) {
            if (camData && camData->getNextCameraID() < 0 && _3e == 0) {
                GetJPAMgr()->createCupStartDemoEmt();
            }

            for (s16 i = 1; i <= 8; i++) {
                checkCrsDemoEventHanabi(i);
            }
        }
    }

    _3e++;
}

void RaceDirector::checkCrsDemoEventHanabi(s16 idx) {
    s16 fireFrame = -1;
    if (TMapObjHanabi::getSupervisor()) {
        fireFrame = TMapObjHanabi::getSupervisor()->getFireFrame(idx);
    }

    if (fireFrame >= 0 && mWaitTimer == fireFrame) {
        TMapObjHanabi::getSupervisor()->fire(idx);
    }
}

void RaceDirector::doReset() {
    mRacePhase = PHASE_RUNNING;
}

void RaceDirector::doWait() {
    mWaitTimer++;
    if (mWaitTimer > mFadeOutTime) {
        mRacePhase = PHASE_SAFETY;
        mWaitTimer = 0;
        mFrameRenewal = false;
    }
}

void RaceDirector::doSafety() {
    mWaitTimer++;
    if (mWaitTimer <= 10) {
        return;
    }
    
    if (_10 == PHASE_0) {
        mRacePhase = PHASE_6;
    }
    else {
        mRacePhase = _10;
    }
    mWaitTimer = 0;
    RaceMgr::getManager()->setRaceResult();
}

void RaceDirector::doRunning(bool raceEnd) {
    if (RaceMgr::getManager()->isRaceModeMiniGame()) {
        if (RaceMgr::getManager()->getRaceMode() == ESCAPE_BATTLE) {
            int time = GeoRabbitMark::getSupervisor()->getWinFrame();
            for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                KartChecker *kartChecker = RaceMgr::getManager()->getKartChecker(i);
                if (kartChecker->isRabbit()) {
                    time = kartChecker->getRabbitTimer();
                }
            }
            mMinigameTime.setFrame(time);
        }

    }

    if (mRace2DDelayTime > 0) {
        mRace2DDelayTime--;
        if (mRace2DDelayTime == 0) {
            appearRace2D(Race2DMode_1);
            mRace2DDelayTime = -1;
        }
    }

    switch (mRaceRunState) {
    case 0: {
        if (RaceMgr::getManager()->isJugemCountStart()) {
            mRaceRunState = 1;
            RaceMgr::getManager()->mRaceBGMPlayer->prepare();
            appearRace2D(Race2DMode_0);
        }
        break;
    }
    case 1: {
        if (RaceMgr::getManager()->isAbleStart()) {
            mRaceRunState = 2;
            RaceMgr::getManager()->mRaceBGMPlayer->play();
            RaceMgr::getManager()->mRaceDrawer->showAllGhost();
        }
        break;
    }
    case 2:
        break;
    }
    checkPauseChoice();

    switch (mRunningState) {
    case 5: {
        mRacePhase = PHASE_5;
        mRunningState = 0;
        break;
    }
    case 4: {
        mRacePhase = PHASE_4;
        GetGameAudioMain()->fadeOutAll(cFadeOutTime);
        mRunningState = 0;
        break;
    }
    case 6: {
        setPhaseWait(PHASE_6, true, true);
        mRunningState = 0;
        break;
    }
    }

    if (RaceMgr::getManager()->isWaitDemoMode()) {
        if (isExitWaitDemo()) {
            bool fadeAudio = false;
            if (!RaceMgr::getManager()->isLastWaitDemo()) {
                if (mWaitDemoResult == 2) {
                    fadeAudio = true;
                }
            }
            else {
                fadeAudio = false;
                mWaitDemoResult = 1;
            }
            setPhaseWait(PHASE_6, fadeAudio, true);
        }
    } 
    else if (RaceMgr::getManager()->isAwardDemoMode() || RaceMgr::getManager()->isStaffRoll()) {
        if (mTimeKeeper->isDemoEnd()) {
            setPhaseWait(RaceMgr::getManager()->getDemoNextPhase(), true, true, 90);
            GetGameAudioMain()->changeSection(0);
        }
    }

    if (raceEnd) {
        mIsRaceEnd = true;

        if (!RaceMgr::getManager()->isReplayMode()) {
            mRacePhase = PHASE_ENDING;
            mEndingState = 1;
            mWaitTimer = 0;
        }

        for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
            KartChecker *kartChecker = RCMGetKartChecker(i);
            kartChecker->fixMiniPoint();
            if (kartChecker->getRank() != 1) {
                kartChecker->setDead();
            }
        }
        if (RaceMgr::getManager()->isRaceModeMiniGame()) {
            RCMGetCourse()->disableShaking();
        }
    }
}

s16 RaceDirector::checkZoomWinConsoleNo() {
    s16 ret = RaceMgr::getManager()->getConsoleNumber() - 1;

    int maxKartIdx = RaceMgr::getManager()->getKartNumber() - 1;
    for (s16 i = 0; i < RaceMgr::getManager()->getConsoleNumber(); i++) {
        RaceMgr::Console *console = &RaceMgr::getManager()->mConsole[i];
        if (!console->isDemo()) {
            bool winningPlayer = false;
            s16 cnsTarget = console->getTargetNo();
            KartChecker *kartChecker = RCMGetKartChecker(cnsTarget);
            if (kartChecker->getRank() < maxKartIdx) {
                winningPlayer = true;
            }
            else {
                
                if (maxKartIdx == kartChecker->getRank() && i < ret) {
                    winningPlayer = true;
                } 
            }

            if (winningPlayer) {
                ret = i;
                maxKartIdx = kartChecker->getRank();
            }
            
        }
    }
    return ret;
}

bool RaceDirector::isAllGoalAnmEnd() {
    bool ret = true;

    for (int i = 0; i < RaceMgr::getManager()->getConsoleNumber(); i++) {
        RaceMgr::Console *console = &RaceMgr::getManager()->mConsole[i];
        if (!console->isDemo()) {
            int cnsTarget = console->getTargetNo();
            KartChecker *kartChecker = RCMGetKartChecker(cnsTarget);
            if (kartChecker->isGoal() && !J2DManager::getManager()->isGoalAnmEnd(i)) {
                ret = false;
                break;            
            }
        }
    }
    return ret;
}

void RaceDirector::doEnding() {
    int oldEndState = mEndingState;
    switch (mEndingState) {
    case 1: {
        if (!RaceMgr::getManager()->isRaceModeMiniGame()) {
            mEndingState = 2;
        }
        else {
            mEndingState = 5;
        }
        mWaitTimer = 0;
        break;
    }
    case 2: {
        bool endRace = false;
        if (RaceMgr::getManager()->isLANMode()) {
            mWaitTimer++;
            if (mWaitTimer >= 300) {
                endRace = true;
                GETPauseManager()->setGameOverFlag(true);
            }
        }
        else {
            endRace = isAllGoalAnmEnd();
        }

        if (endRace) {
            J2DManager::getManager()->hideRace2D();
            mEndingState = 3;
        }
        break;
    }
    case 3: {
        if (J2DManager::getManager()->isHideRace2D()) {
            if (RaceMgr::getManager()->isLANMode()) {
                for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                    if (RCMGetKartChecker(i)->getRank() == 1) {
                        J2DManager::getManager()->startLANResultAnm(i);
                        J2DManager::getManager()->startLANNumResultAnm();
                        break;
                    }
                }                
            }
            setEndingPhaseZoomConsole();
        }
        break;
    }
    case 4: {
        bool isWinConsole = true;
        if (isZoomWinConsole() && RaceMgr::getManager()->getConsoleNumber() > 1) {
            isWinConsole = GetKartCtrl()->CheckZoomWinConsole();
        }
        if (isWinConsole) {
            mWaitTimer++;
        }
        int waitTimeEnd = 60;
        if (RaceMgr::getManager()->isLANMode()) {
            waitTimeEnd = 180;
        }
        if (mWaitTimer >= waitTimeEnd) {
            mEndingState = 7;
            mWaitTimer = 0;
            PauseManager::getManager()->setResultStartFlag(true);
        }

        break;
    }
    case 5: {
        if (mWaitTimer == 0) {
            mPlayingWinAnm = true;
        }
        else if (mWaitTimer >= 30) {
            mEndingState = 6;
            mWaitTimer = 0;
        }
        if (mEndingState == oldEndState) {
            mWaitTimer++;
        }
        break;
    }
    case 6: {
        int waitTimeEnd = 110;
        if (RaceMgr::getManager()->isLANMode()) {
            waitTimeEnd = 270; // whoa mkw mt charge value
        }

        if (mWaitTimer == 0) {
            for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                if (RCMGetKartChecker(i)->getRank() == 1) {
                    GETJ2DManager()->startWinnerAnm(i);
                }
            }
            RaceMgr::getManager()->mRaceBGMPlayer->setMiniGameEnd();
        }
        else if (mWaitTimer == 70) {
            J2DManager::getManager()->hideRace2D();
        }
        else if (mWaitTimer == 90) {
            mMovingToStart = true;
            _3a = true;
            for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                if (GetGeoObjMgr()->getBalloonMgr(i)) {
                    GetGeoObjMgr()->getBalloonMgr(i)->changeWinningPos();
                }
            }

            if (GeoRabbitMark::getSupervisor()) {
                GeoRabbitMark::getSupervisor()->setWinnerPosition();
            }

            if (RaceMgr::getManager()->isLANMode()) {
                for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                    if (RCMGetKartChecker(i)->getRank() == 1) {
                        J2DManager::getManager()->startLANResultAnm(i);
                        J2DManager::getManager()->startLANNumResultAnm();
                        break;
                    }
                }                
            }
        }
        else if (mWaitTimer >= waitTimeEnd) {
            setEndingPhaseZoomConsole();
            if (RaceMgr::getManager()->isLANMode()) {
                GETPauseManager()->setGameOverFlag(true);
            }
        }
        if (mEndingState == oldEndState) {
            mWaitTimer++;
        }
        break;
    }
    case 7: {
        checkPauseChoice();
        break;
    }

    }
}

void RaceDirector::setEndingPhaseZoomConsole() {
    mEndingState = 4;
    mWaitTimer = 0;
    if (!RaceMgr::getManager()->isLANMode()) {
        mZoomWinCnsNo = checkZoomWinConsoleNo();
    }
}

void RaceDirector::checkPauseChoice() {
    switch(PauseManager::getPauseChoice()) {
    case 1:
        mRacePhase = PHASE_4;
        break;
    case 7:
        mRacePhase = PHASE_5;
        break;
    case 4: {
        setPhaseWait(PHASE_7, true, true);
        break;
    }

    case 2:
        setPhaseWait(PHASE_8, true, true);
        break;
    case 3:
        setPhaseWait(PHASE_14, true, true);
        break;
    case 6:
        setPhaseWait(PHASE_15, true, true);
        break;
    case 9:
        setPhaseWait(PHASE_10, true, true);
        break;
    case 5:
        setPhaseWait(PHASE_11, true, true);
        break;
    case 11:
        setPhaseWait(PHASE_11, true, true); 
        GetGameAudioMain()->changeSection(2);
        break;
    case 12: {
        bool fadeAudio = true;
        if (GetGameAudioMain()->getPlayingStreamID() == 0x2000022)
            fadeAudio = false;
        setPhaseWait(PHASE_11, fadeAudio, true); 
        GetGameAudioMain()->changeSection(2);
        break;
    }
    case 8:
        setPhaseWait(PHASE_6,true, false);
        break;
    case 10:
        setPhaseWait(PHASE_13, true, true);
        break;
    }
}

bool RaceDirector::isExitWaitDemo() {
    u32 waitTimeFrames = 1800;
    if (sForceWaitDemoTime != 0) {
        waitTimeFrames = sForceWaitDemoTime * 60u;
    }

    bool skip = false;
    mWaitDemoTimer++;
    if (mWaitDemoTimer >= 60u) {
        if (gGamePad1P.testTrigger(KartGamePad::A | KartGamePad::START)) {
            skip = true;
            mWaitDemoResult = 1;
        }
    }

    if (!skip && mWaitDemoTimer >= waitTimeFrames) {
        skip = true;
        mWaitDemoResult = 2;
    }
    return skip;
}

void RaceDirector::setPhaseWait(ERacePhase phase, bool fadeAudio, bool fadeVideo, int fadeOutTime) {
    mRacePhase = PHASE_WAIT;
    _10 = phase;
    mWaitTimer = 0;
    mFadeOutTime = fadeOutTime;

    JUTColor fadeColor(0x00, 0x00, 0x00, 0xff);
    SYSTEM_GetFader()->setColor(fadeColor);

    if (fadeVideo) {
        SYSTEM_StartFadeOut(mFadeOutTime);
    }
    else {
        PauseManager::getManager()->wipeOut(mFadeOutTime);
    }

    if (fadeAudio) {
        GetGameAudioMain()->fadeOutAll(mFadeOutTime);
    }

    switch (phase) {
        case PHASE_9:
        case PHASE_11:
            break;
        case PHASE_6:
        case PHASE_7:
        case PHASE_8:
        case PHASE_10:
        case PHASE_13:
        case PHASE_14:
        case PHASE_15: {
            GetGameAudioMain()->changeSection(0);
            break;
        }
    }
}

bool RaceDirector::checkRaceEnd() {
    bool ret = false;

    if (!RaceMgr::getManager()->isWaitDemoMode()) {
        switch(RaceMgr::getManager()->getRaceMode()) {
        case BALLOON_BATTLE: {
            ret = checkRaceEndForBalloonBattle();
            break;
        }
        case ROBBERY_BATTLE: {
            if (mMiniGameFrame <= 0) {
                ret = true;
            }
            break;
        }
        case BOMB_BATTLE: {
            ret = checkRaceEndForBombBattle();
            break;
        }
        case ESCAPE_BATTLE: {
            ret = checkRaceEndForEscapeBattle();
            break;
        }
        case TIME_ATTACK: {
            if (RaceMgr::getManager()->getKartChecker(0)->isGoal()) {
                ret = true;
            }
            break;
        }
        case AWARD_DEMO: {
            break;
        }
        default: {
            ret = checkRaceEndForRace();
            break;
        }
        }
    }
    return ret;
}

bool RaceDirector::checkRaceEndForRace() {
    bool ret = true;

    if (RaceMgr::getManager()->getKartInfo(0)->isPlayerKart() && RaceMgr::getManager()->mRaceInfo->getPlayerKartNumber() != RaceMgr::getManager()->getKartNumber() || RaceMgr::getManager()->getKartNumber() == 1) {
        int playersNotFinished = 0;
        for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
            KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(i);
            KartChecker *kartChecker = RaceMgr::getManager()->getKartChecker(i);
            if (kartInfo->isPlayerKart() && !kartChecker->isGoal()) {
                playersNotFinished++;
            }
        }

        if (playersNotFinished > 0) {
            ret = false;
        }
    }
    
    else if (RaceMgr::getManager()->getGoalKartNumber() < RaceMgr::getManager()->getKartNumber() - 1) {
        ret = false;
    }            

    return ret;
}

bool RaceDirector::checkRaceEndForBalloonBattle() {
    bool ret = false;
    int balloonNum = 0;
    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        if (RaceMgr::getManager()->getKartChecker(i)->getBalloonNumber() > 0) {
            balloonNum++;
        }
    }
    
    if (balloonNum <= 1)
        ret = true;
    return ret;
}

bool RaceDirector::checkRaceEndForBombBattle() {
    bool ret = false;
    int bombPoints = 0;
    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        if (RaceMgr::getManager()->getKartChecker(i)->isBombPointFull()) {
            bombPoints++;
        }
    }

    if (bombPoints > 0)
        ret = true;
    return ret;
}

bool RaceDirector::checkRaceEndForEscapeBattle() {
    bool ret = false;

    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        if (RaceMgr::getManager()->getKartChecker(i)->isRabbitWinner()) {
            ret = true;
            break;
        }
    }
    return ret;
}

int RaceDirector::requestDemoCameraID() const {
    int ret = -1;
    if (mTimeKeeper) {
        ret = mTimeKeeper->requestDemoCameraID();
    }
    return ret;
}

bool RaceDirector::isPlayingDemoWinnerAnm() const {
    bool ret = false;
    if (mTimeKeeper) {
        ret = mTimeKeeper->isPlayingEndingWinAnm();
    }
    return ret;
}

u32 RaceDirector::getDemoCameraTagName() const {
    u32 ret = 0;
    if (mTimeKeeper) {
        ret = mTimeKeeper->getDemoCameraTagName();
    }
    return ret;
}
