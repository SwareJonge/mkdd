#include "Osako/kartPad.h"
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

void KartCtrl::DoContCtl(int) {}

void KartCtrl::DoContCopy(int) {}

void KartCtrl::DoContPaste(int) {
}

f32 KartCtrl::GetItemStickY(int) {}
f32 KartCtrl::GetItemStickX(int) {}

void KartCtrl::GetItemButton(int) {}

void KartCtrl::DoLod() {
    // void KartCam::GetClipperScale() {}
    // void KartCam::GetLodLen() {}
}

void KartCtrl::GetPortPtr(int) {}

void KartCtrl::GetCamFovy(int) {}

void KartCtrl::GetCamAspect(int) {}

void KartCtrl::GetBodyGround(int) {}

void KartCtrl::GetRRTireGround(int) {}

void KartCtrl::GetRLTireGround(int) {}

void KartCtrl::GetBodyPos(int, JGeometry::TVec3<float> *) {}

void KartCtrl::GetBodyVel(int, JGeometry::TVec3<float> *) {}

bool KartCtrl::GetLeftFTirePos(int, Vec *) {}

bool KartCtrl::GetRightFTirePos(int, Vec *) {}

void KartCtrl::GetLeftTirePos(int, Vec *) {}

void KartCtrl::GetRightTirePos(int, Vec *) {}

void KartCtrl::GetTirePos(int, int, Vec *) {}

void KartCtrl::GeTireG(int) {}

f32 KartCtrl::GetCarSpeed(int) {}

f32 KartCtrl::GetCarRpm(int) {}

f32 KartCtrl::GetKartRpm(int) {}

KartGamePad *KartCtrl::GetDriveCont(int) {}

KartGamePad *KartCtrl::GetCoDriveCont(int) {}

u64 KartCtrl::GetCarStatus(int idx) {
    return getKartBody(idx)->mCarStatus;
}

u32 KartCtrl::GetGameStatus(int idx) {
    return getKartBody(idx)->mGameStatus;
}

void KartCtrl::SetTireDispRound(KartBody *, KartSus *, float) {}

void KartCtrl::SetKartRpm(KartBody *, float, float) {}

u8 KartCtrl::WhichDriver(int) {}

u8 KartCtrl::WhichNowDriver(int) {}

bool KartCtrl::CheckCamera(int) {}

int KartCtrl::GetCameraNum(int) {}

bool KartCtrl::CheckItem(int) {}

f32 KartCtrl::GetMaxSpeed(int) {}

f32 KartCtrl::GetDownSlopeAcc(int) {}

f32 KartCtrl::GetDownSlopeSpeed(int) {}

f32 KartCtrl::GetTireAngle(int) {}

f32 KartCtrl::GetTandemDir(int) {}

f32 KartCtrl::GetWaterHeight(int) {}

bool KartCtrl::CheckJugemuSignal() {}

GameAudio::KartSoundMgr *KartCtrl::GetKartSoundMgr(int) {
    // void DriverModel::getGameAudioCharacterSoundMgr() {}
}

f32 KartCtrl::GetKartBodyOffset(int) {}

void KartCtrl::MakeChangePossible(int) {}

bool KartCtrl::CheckTandemItmGet(int) {}

bool KartCtrl::CheckTandemItmRollingGet(int) {}

void KartCtrl::SetObjectPos(int, JGeometry::TVec3<float>) {}

bool KartCtrl::CheckThunderBolt(int) {}

void KartCtrl::GetTireRadius(int) {}

void KartCtrl::IsBurn(int) {}

void KartCtrl::IsWallReact(int) {}

void KartCtrl::HaveBalloon(int) {}

int KartCtrl::GetDriftCnt(int) {}

bool KartCtrl::IsMiniGame() {}

bool KartCtrl::IsMiniGameEnd() {}

bool KartCtrl::CheckWinner() {}

void KartCtrl::GetKartEffctVel(int, JGeometry::TVec3<float> *) {}

bool KartCtrl::CheckChange(int) {}

bool KartCtrl::CheckMatchless(int) {}

bool KartCtrl::CheckReverse(int) {}

f32 KartCtrl::GetKartScale(int) {}

RivalKart *KartCtrl::getKartEnemy(int) {}

CLPoint *KartCtrl::GetKartCenterPtr(int) {}

void KartCtrl::EnemyInit(int) {}

void KartCtrl::EnemyResetInit(int) {}

bool KartCtrl::CheckZoomWinConsole() {}

void KartCtrl::SetUpMiniTurbo(int, unsigned char) {}

void KartStrat::DoStepAccel() {}

void KartStrat::DoStepSterr() {}

void KartStrat::DoSterr() {}

void KartStrat::DoAccel() {}

bool KartCtrl::HaveRabbit() {}
