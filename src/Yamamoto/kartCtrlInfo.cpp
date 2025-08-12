#include "Kaneshige/RaceMgr.h"
#include "Osako/kartPad.h"
#include "Sato/ItemObjMgr.h"
#include "Yamamoto/kartCtrl.h"

#include "Jsystem/JAudio/JASFakeMatch2.h"

// comments inside functions are inline functions being called in that function

void KartCtrl::SetPadClear(KartGamePad *kartGamePad) {
    kartGamePad->mButtons.mButton = 0;
    kartGamePad->mButtons.mTrigger = 0;
    kartGamePad->mButtons.mRelease = 0;
    kartGamePad->mMainStick.mX = 0;
    kartGamePad->mMainStick.mY = 0;
    kartGamePad->mSubStick.mX = 0;
    kartGamePad->mSubStick.mY = 0;
    return;
}

void KartCtrl::SetWanWanPadClear(KartGamePad *kartGamePad) {
    kartGamePad->mButtons.mButton = 0;
    kartGamePad->mButtons.mTrigger = 0;
    kartGamePad->mButtons.mRelease = 0;
    kartGamePad->mMainStick.mY = 0;
    kartGamePad->mSubStick.mX = 0;
    kartGamePad->mSubStick.mY = 0;
    return;
}

void KartCtrl::SetGhostPadClear(KartGamePad *kartGamePad) {
    kartGamePad->mButtons.clear();
    kartGamePad->mMainStick.clear();
    kartGamePad->mSubStick.clear();
    return;
}

void KartCtrl::PadAllClear(int gamePadIndex) {
    // void KartCtrl::getCoPad(int) {}
    // void KartCtrl::getPad(int) {}
    const KartBody *kartBody;

    kartBody = getKartBody(gamePadIndex);
    // Clear First Kart Player (Driver) Game Pad.
    SetPadClear(getPad(gamePadIndex));
    if ((kartBody->mGameStatus & 1) != 0) {
        // Clear Second Kart Player (Passenger) Game Pad.
        SetPadClear(getCoPad(gamePadIndex));
    }
    return;
}

void KartCtrl::DoContCopy(int index) {
    u32 control;
    
    control = GetDriveCont(index)->mButtons.mButton;
    getKartPad(index)[0]._24[0] = control;

    control = GetCoDriveCont(index)->mButtons.mButton;
    getKartPad(index)[0]._24[1] = control;

    control = GetDriveCont(index)->mButtons.mTrigger;
    getKartPad(index)[0]._24[2] = control;

    control = GetCoDriveCont(index)->mButtons.mTrigger;
    getKartPad(index)[0]._24[3] = control;

    control = GetDriveCont(index)->mButtons.mRelease;
    getKartPad(index)[0]._24[4] = control;
    
    control = GetCoDriveCont(index)->mButtons.mRelease;
    getKartPad(index)[0]._24[5] = control;
    return;
}

void KartCtrl::DoContPaste(int index) {
    u32 value;

    value = getKartPad(index)->_24[0];
    GetDriveCont(index)->mButtons.mButton = value;

    value = getKartPad(index)->_24[1];
    GetCoDriveCont(index)->mButtons.mButton = value;

    value = getKartPad(index)->_24[2];
    GetDriveCont(index)->mButtons.mTrigger = value;

    value = getKartPad(index)->_24[3];
    GetCoDriveCont(index)->mButtons.mTrigger = value;

    value = getKartPad(index)->_24[4];
    GetDriveCont(index)->mButtons.mRelease = value;

    value = getKartPad(index)->_24[5];
    GetCoDriveCont(index)->mButtons.mRelease = value;
    return;
}

f32 KartCtrl::GetItemStickY(int kartPadIndex) {
    return getKartPad(kartPadIndex)->mItemStickY;
}

f32 KartCtrl::GetItemStickX(int kartIndex) {
    if ((getKartBody(kartIndex)->mGameStatus & 0x1) != 0) {
        return 0;
    }
    return 0;
}

u32 KartCtrl::GetItemButton(int kartIndex) {
    return (getKartBody(kartIndex)->mGameStatus & 1) != 0
        ? getKartPad(kartIndex)->mFaceBtnsMask
        : getKartPad(kartIndex)->mItemMask;
}

void KartCtrl::DoLod() {
    // void KartCam::GetClipperScale() {}
    // void KartCam::GetLodLen() {}
}

void KartCtrl::GetPortPtr(int) {}

void KartCtrl::GetCamFovy(int camIndex) {
    getKartCam(camIndex)->GetFovy();
    return;
}

void KartCtrl::GetCamAspect(int camIndex) {
    getKartCam(camIndex)->GetAspect();
    return;
}

CrsGround *KartCtrl::GetBodyGround(int kartIndex) {
    return &getKartBody(kartIndex)->mBodyGround;
}

// MJB - Regarding functions `GetRRTireGround` and `GetRLTireGround`:
//       I believe the addition of `0x14` just accesses the protected field
//       `_14` under the ExModel* for `mWheel`: `mKartSus[i]->mWheel->_14;`
int KartCtrl::GetRRTireGround(int kartIndex) {
    return *(int *)&getKartBody(kartIndex)->mKartSus[2] + 0x14; // Rear Right Tire Ground
}

int KartCtrl::GetRLTireGround(int kartIndex) {
    return *(int *)&getKartBody(kartIndex)->mKartSus[3] + 0x14; // Rear Left Tire Ground
}

void KartCtrl::GetBodyPos(int kartIndex, JGeometry::TVec3f *vecBodyPos) {
    const KartBody *kartBody = getKartBody(kartIndex);
    vecBodyPos->x = kartBody->mPos.x;
    vecBodyPos->y = kartBody->mPos.y;
    vecBodyPos->z = kartBody->mPos.z;
}

void KartCtrl::GetBodyVel(int kartIndex, JGeometry::TVec3f *vecBodyVel) {
    const KartBody *kartBody = getKartBody(kartIndex);
    vecBodyVel->x = kartBody->mVel.x * kartBody->_470;
    vecBodyVel->y = kartBody->mVel.y * kartBody->_470;
    vecBodyVel->z = kartBody->mVel.z * kartBody->_470;
}

// FIX: Add return statements for the following two functions:
bool KartCtrl::GetLeftFTirePos(int kartIndex, Vec *vec) {
    getKartBody(kartIndex);
    GetTirePos(kartIndex, 1, vec);
}

bool KartCtrl::GetRightFTirePos(int kartIndex, Vec *vec) {
    getKartBody(kartIndex);
    GetTirePos(kartIndex, 0, vec);
}

void KartCtrl::GetLeftTirePos(int kartIndex, Vec *vec) {
    getKartBody(kartIndex);
    RaceMgr::getCurrentManager();
    GetTirePos(kartIndex, 3, vec);
}

void KartCtrl::GetRightTirePos(int kartIndex, Vec *vec) {
    getKartBody(kartIndex);
    RaceMgr::getCurrentManager();
    GetTirePos(kartIndex, 2, vec);
}

void KartCtrl::GetTirePos(int, int, Vec *) {}

f32 KartCtrl::GeTireG(int kartIndex) {
    return getKartBody(kartIndex)->mTireG.x;
}

f32 KartCtrl::GetCarSpeed(int kartIndex) {
    return (getKartBody(kartIndex)->mGameStatus & 0x200) != 0
        ? 0.0f
        : 2.16f * getKartBody(kartIndex)->mSpeed;
}

f32 KartCtrl::GetCarRpm(int kartIndex) {
    float carRpm = getKartBody(kartIndex)->mCarRPM / 5000.0f;
    if (carRpm > 1.0f) {
        carRpm = 1.0f;
    }
    return carRpm;
}


f32 KartCtrl::GetKartRpm(int kartIndex) {
    return getKartBody(kartIndex)->mKartRPM;
}

// MJB - The following two functions are the same, just with
//       the pads for the driver/co-driver inverted.
KartGamePad *KartCtrl::GetDriveCont(int index) {
    if ((getKartBody(index)->mGameStatus & 1) == 0) {
        return getPad(index);
    }
    
    return (GetKartCtrl()->WhichDriver(index) == 0)
        ? getPad(index)     // Driver
        : getCoPad(index);  // Passenger
}

KartGamePad *KartCtrl::GetCoDriveCont(int index) {
    if ((getKartBody(index)->mGameStatus & 1) == 0) {
        return getPad(index);
    }
    
    return (GetKartCtrl()->WhichDriver(index) == 0)
        ? getCoPad(index)   // Passenger
        : getPad(index);    // Driver
}

u64 KartCtrl::GetCarStatus(int idx) {
    return getKartBody(idx)->mCarStatus;
}

u32 KartCtrl::GetGameStatus(int idx) {
    return getKartBody(idx)->mGameStatus;
}

void KartCtrl::SetTireDispRound(KartBody *, KartSus *, float) {}

bool KartCtrl::WhichDriver(int kartIndex) {
    return getKartBody(kartIndex)->mDriver != 0;
}

u8 KartCtrl::WhichNowDriver(int kartIndex) {
    return getKartBody(kartIndex)->mDriver;
}

bool KartCtrl::CheckCamera(int kartIndex) {
    const KartBody *kartBody = getKartBody(kartIndex);
    return -(uint)kartBody->mCameraNum + 100 != 0;
}

int KartCtrl::GetCameraNum(int kartIndex) {
    return getKartBody(kartIndex)->mCameraNum;
}

bool KartCtrl::CheckItem(int) {}

f32 KartCtrl::GetMaxSpeed(int) {}

f32 KartCtrl::GetDownSlopeAcc(int) {}

f32 KartCtrl::GetTireAngle(int kartIndex) {
    return getKartBody(kartIndex)->mTireAngle;
}

bool KartCtrl::GetTandemDir(int index) {
    int isBack = getKartAnime(index)->IsBack(index);
    return u8(isBack != 0);
}

f32 KartCtrl::GetWaterHeight(int) {}

bool KartCtrl::CheckJugemuSignal() { return (mBitfield >> 1) & 1; }

GameAudio::KartSoundMgr *KartCtrl::GetKartSoundMgr(int kartIndex) {
    // void DriverModel::getGameAudioCharacterSoundMgr() {}
    KartSound *kartSound = getKartSound(kartIndex);
    return *(GameAudio::KartSoundMgr **)kartSound;
}

f32 KartCtrl::GetKartBodyOffset(int kartIndex) {
    getKartBody(kartIndex);     // MJB - should this be used...?
    return 60.0f;
}

bool KartCtrl::CheckTandemItmRollingGet(int) {}

void KartCtrl::SetObjectPos(int camIndex, JGeometry::TVec3f vecObjectPos) {
    KartDemoCam *kartDemoCam = getKartCam(camIndex)->GetDemoCam();
    kartDemoCam->_8c.set(vecObjectPos);
    return;
}

bool KartCtrl::CheckThunderBolt(int kartIndex) {
    return getKartBody(kartIndex)->getThunder()->mFlags & 1;
}

double KartCtrl::GetTireRadius(int kartIndex) {
    return *(float *)(&getKartBody(kartIndex)->mKartSus[2]->_fc[0]);
}

bool KartCtrl::IsBurn(int kartIndex) {
    const KartDamage *kartDamage = getKartBody(kartIndex)->getDamage();
    return (u8)((kartDamage->mFlags & 0x180) != 0);
}

bool KartCtrl::IsWallReact(int kartIndex) {
    KartBody *kartBody = getKartBody(kartIndex);
    const KartTumble *kartTumble = kartBody->getTumble();
    
    if ((kartTumble->_0[4] & 1) != 0) {
        return true;
    }
    else {
        return u8((kartBody->mCarStatus & 0x100000) != 0);
    }
}

u32 KartCtrl::HaveBalloon(int kartIndex) {
    getKartBody(kartIndex);
    KartChecker *kartChecker = RCMGetKartChecker(kartIndex);
    u32 haveBalloon = __cntlzw(kartChecker->getBalloonNumber());
    return (haveBalloon >> 5) & 0xff;
}

int KartCtrl::GetDriftCnt(int kartIndex) {
    const KartBody *kartBody = getKartBody(kartIndex);
    
    if ((kartBody->mCarStatus & 3) != 0) {
        return (u8)(kartBody->mMTState + 1);
    }
    return 0;
}

bool KartCtrl::IsMiniGame() {}

bool KartCtrl::IsMiniGameEnd() {}

bool KartCtrl::CheckWinner() {}


// FIX - See below:
// MJB - This code seems really strange... not only do the offsets not
//       correlate to proper field members of the KartBody class itself,
//       (specifically `mDriverModels` being index 2 and 3, not 0 and 1,
//       meaning its out of bounds of the array), but the structure of
//       the jumps don't make sense either...
//
//       It's like someone used explicit gotos, or the compiler optimised
//       something in a weird way when it comes to values returned.
//
//       Yuck, yuck, yuck! - Work out nicer C code later.
bool KartCtrl::CheckChange(int kartIndex) {
    const KartBody* kartBody = getKartBody(kartIndex);

    if (kartBody->mDriverModels[2]->IsChange() != 0) {
        goto lblCheckChangeTrue;    // return true;
    }

    if (kartBody->mDriverModels[3]->IsChange() == 0) {
        goto lblCheckChangeFalse;    // return false;
    }
    
lblCheckChangeTrue:
    return true;

lblCheckChangeFalse:
    return false;
}

void KartCtrl::CheckMatchless(int kartIndex) {
    KartCrash *kartCrash = getKartBody(kartIndex)->getCrash();
    kartCrash->CheckMatchlessTimer();
    return;
}

bool KartCtrl::CheckReverse(int kartIndex) {
    return (getKartBody(kartIndex)->_5c3 >> 1) & 1;
}

double KartCtrl::GetKartScale(int kartIndex) {
    KartBody *kartBody = getKartBody(kartIndex);
    const KartThunder *kartThunder = kartBody->getThunder();
    float kartScale;
    
    if ((kartThunder->mFlags & 1) != 0) {
        kartThunder = kartBody->getThunder();
        kartScale = kartThunder->mScale;
    } else {
        kartScale = 1.0f;
    }
    return (double)kartScale;
}

RivalKart *KartCtrl::getKartEnemy(int kartIndex) {
    return mRivalKarts[kartIndex];
}

CLPoint *KartCtrl::GetKartCenterPtr(int kartIndex) {
    return *(CLPoint **)getKartTarget(kartIndex);
}

void KartCtrl::EnemyInit(int kartIndex) {
    getKartEnemy(kartIndex)->Init();
    return;
}

void KartCtrl::EnemyResetInit(int kartIndex) {
    getKartEnemy(kartIndex)->ResetInit();
    return;
}

bool KartCtrl::CheckZoomWinConsole() { return mBitfield >> 2 & 1; }

void KartCtrl::SetUpMiniTurbo(int kartIndex, unsigned char setupMiniTurbo) {
    getKartBody(kartIndex)->mMTState = setupMiniTurbo;
}

void KartStrat::DoStepAccel() {}

void KartStrat::DoStepSterr() {}

void KartStrat::DoSterr() {}

void KartCtrl::HaveRabbit() {
    const RaceMgr *raceMgr = RaceMgr::getCurrentManager();
    int kartNumber = GetKartNumber();
    KartBody *kartBody;
    bool haveRabbit;
    
    if (raceMgr->getRaceMode() == 7) {
        haveRabbit = false;
        for (int i = 0; i < kartNumber; i++) {
            kartBody = getKartBody(i);
            if (kartBody->getItem()->_0[8] != 0) {
                haveRabbit = true;
            }
        }

        // MJB - Shine thief's music controls, maybe...?
        //       Rabbit might refer to the person with the shine...
        if (haveRabbit && (kartBody->getItem()->_0[9] == 0)) {
            getKartSound(kartBody->mMynum)->DoShineStartBGM();
            kartBody->getItem()->_0[9] = 1;
        } else if (!haveRabbit && (kartBody->getItem()->_0[9] != 0)) {
            getKartSound(kartBody->mMynum)->DoShineStopBGM();
            kartBody->getItem()->_0[9] = 0;
        }
    }
    return;
}
