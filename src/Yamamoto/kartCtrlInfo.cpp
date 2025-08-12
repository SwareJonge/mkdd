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

void KartCtrl::DoContCtl(int index) {
    KartBody *kartBody;
    const RaceMgr *raceMgr;
    KartCheck *kartCheck;
    KartGamePad *kartGamePad;
    RivalKart *rivalKart;
    KartHandle *kartHandle;
    
    kartBody = getKartBody(index);
    raceMgr = RaceMgr::getCurrentManager();
    if (raceMgr->isMirror() != 0) {
        kartGamePad = GetDriveCont(index);
        kartGamePad->mMainStick.mX = -(kartGamePad->mMainStick.mX);
        if ((kartBody->mGameStatus & 1) != 0) {
            kartGamePad = GetCoDriveCont(index);
            kartGamePad->mMainStick.mX = -(kartGamePad->mMainStick.mX);
        }
    }
    DoContCopy(index);
    kartCheck = kartBody->getChecker();
    if (kartCheck->CheckAllClearKey() != 0) {
        kartGamePad = GetDriveCont(index);
        SetPadClear(kartGamePad);
        kartGamePad = GetCoDriveCont(index);
        SetPadClear(kartGamePad);
        rivalKart = getKartEnemy(index);
        rivalKart->ContempLate();
    }
    kartHandle = kartBody->getHandle();
    kartHandle->WatchFrontalCollisionForce();
    if ((kartBody->mCarStatus & 0x1000) != 0) {
        kartGamePad = GetDriveCont(index);
        SetWanWanPadClear(kartGamePad);
        kartGamePad = GetCoDriveCont(index);
        SetWanWanPadClear(kartGamePad);
    }
    else {
        kartCheck = kartBody->getChecker();
        if (kartCheck->CheckAllClearKeyT() != 0) {
            kartGamePad = GetDriveCont(index);
            SetPadClear(kartGamePad);
            kartGamePad = GetCoDriveCont(index);
            SetPadClear(kartGamePad);
        }
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

void KartCtrl::SetKartRpm(KartBody *kartBody, float unknownRPM1, float unknownRPM2) {
    float unknownMultiplier = kartBody->_458 / 60.0f;

    if (unknownMultiplier > 2.0f) {
        unknownMultiplier = 2.0f;
    }

    // Presumably this checks if there was a collision...?
    if (kartBody->getTouchNum() == 0) {
        kartBody->mCarRPM = unknownRPM1;
        unknownMultiplier = 2.0f;
    }
    else {
        kartBody->mCarRPM = unknownRPM2;
    }

    // Ensure car RPM can't be negative.
    if (kartBody->mCarRPM < 0) {
        kartBody->mCarRPM = 0;
    }

    kartBody->mCarRPM *= 40.0f;
    kartBody->mCarRPM *= unknownMultiplier;
    kartBody->mCarRPM += 300.0f;

    // Set upper bounds for car RPM.
    if (kartBody->mCarRPM > 16300.0f) {
        kartBody->mCarRPM = 16300.0f;
    }
    return;
}

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

f32 KartCtrl::GetMaxSpeed(int kartIndex) {
    KartBody *kartBody;
    CrsGround::EAttr attrBodyGround;
    float splashHeight;
    float maxSpeed;
    
    kartBody = getKartBody(kartIndex);
    attrBodyGround = kartBody->mBodyGround.getAttribute();
    switch (attrBodyGround) {
        case 0x13:
            maxSpeed = kartBody->_3f0;
            splashHeight = kartBody->mBodyGround.getSplashHeight();
            if (splashHeight > 0) {
                splashHeight = kartBody->mBodyGround.getSplashHeight();
                if (kartBody->mPlayerPosMtx[1][3] - splashHeight < 0) {                
                    maxSpeed *= 0.7f;
                }
            }
            break;
        case 0:
        case 3:
            maxSpeed = kartBody->_3f4;
            splashHeight = kartBody->mBodyGround.getSplashHeight();
            if (splashHeight > 0) {
                splashHeight = kartBody->mBodyGround.getSplashHeight();
                if (kartBody->mPlayerPosMtx[1][3] - splashHeight < 0) {                
                    maxSpeed *= 0.7f;
                }
            }
            break;
        case 1:
        case 4:
        case 0xb:
            maxSpeed = kartBody->_3fc;
            splashHeight = kartBody->mBodyGround.getSplashHeight();
            if (splashHeight > 0) {
                splashHeight = kartBody->mBodyGround.getSplashHeight();
                if (kartBody->mPlayerPosMtx[1][3] - splashHeight < 0) {                
                    maxSpeed *= 0.7f;
                }
            }
            break;
        case 0xc:
            maxSpeed = kartBody->_3f8;
            splashHeight = kartBody->mBodyGround.getSplashHeight();
            if (splashHeight > 0) {
                splashHeight = kartBody->mBodyGround.getSplashHeight();
                if (kartBody->mPlayerPosMtx[1][3] - splashHeight < 0) {                
                    maxSpeed *= 0.7f;
                }
            }
            break;
        case 0x11:
            // Presumably, slow down when in water, e.g. Peach Beach...?
            maxSpeed = kartBody->_3f0;
            splashHeight = kartBody->mBodyGround.getWaterHeight();
            if ((kartBody->_1a0[1][3] - splashHeight) < -50.0f) {
                maxSpeed *= 0.3f;
            }
            else {
                splashHeight = kartBody->mBodyGround.getWaterHeight();
                if ((kartBody->_1a0[1][3] - splashHeight) < 0) {
                    maxSpeed *= 0.7f;
                }
            }
            break;
        default:
            maxSpeed = kartBody->_3f0;
            splashHeight = kartBody->mBodyGround.getSplashHeight();
            if (splashHeight > 0) {
                splashHeight = kartBody->mBodyGround.getSplashHeight();
                if (kartBody->mPlayerPosMtx[1][3] - splashHeight < 0) {                
                    maxSpeed *= 0.7f;
                }
            }
            break;
    }
    return maxSpeed;
}

f32 KartCtrl::GetDownSlopeAcc(int kartIndex) {
    const KartBody *kartBody = getKartBody(kartIndex);
    float slopeNumerator = kartBody->_464;
    
    if (slopeNumerator < 0.0f && kartBody->_32c.y != 1.0f) {
        slopeNumerator = -slopeNumerator;
        
        if (slopeNumerator <= 0.0872222f) {
            return 0;
        } else if (slopeNumerator >= 0.785f) {
            slopeNumerator = 0.785f;
        }
        
        slopeNumerator /= 0.785f;
        return 0.2f * slopeNumerator + 1.0f;
    }
    
    return 1.0f;
}

f32 KartCtrl::GetDownSlopeSpeed(int kartIndex) {
    const KartBody *kartBody = getKartBody(kartIndex);
    float slopeNumerator;

    if (kartBody->_464 < 0.0f && kartBody->_32c.y != 1.0f) {
        slopeNumerator = -kartBody->_464;
        if (slopeNumerator <= 0.0523333f) {
            return 0.0f;
        } else if (slopeNumerator >= 0.174444f) {
            slopeNumerator = 0.174444f;
        }
        slopeNumerator /= 0.174444f;
        return 3.0f * slopeNumerator;
    }
    return 0.0f;
}

f32 KartCtrl::GetTireAngle(int kartIndex) {
    return getKartBody(kartIndex)->mTireAngle;
}

bool KartCtrl::GetTandemDir(int index) {
    int isBack = getKartAnime(index)->IsBack(index);
    return u8(isBack != 0);
}

f32 KartCtrl::GetWaterHeight(int kartIndex) {
    KartBody *kartBody = getKartBody(kartIndex);
    float waterHeight;
    float splashHeight;
    
    if (kartBody->mBodyGround.getAttribute() == 0x11) {
        waterHeight = kartBody->mBodyGround.getWaterHeight();
        waterHeight = kartBody->_1a0[1][3] - waterHeight;
        return waterHeight;
    }
    else {
        splashHeight = kartBody->mBodyGround.getSplashHeight();
        waterHeight = 0.0f;
        if (splashHeight > waterHeight) {
            waterHeight = kartBody->mBodyGround.getSplashHeight();
            waterHeight = kartBody->_1a0[1][3] - waterHeight;
        }
        return waterHeight;
    }
}

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

bool KartCtrl::MakeChangePossible(int index) {
    KartBody *kartBody;
    ItemObjMgr *itemObjMgr;
    const ItemObj *itemObj;
    u8 isRollingSlot;

    kartBody = getKartBody(index);
    itemObjMgr = GetItemObjMgr();
    if ((kartBody->mGameStatus & 1) != 0) {
        return false;
    }
    isRollingSlot = ItemObjMgr::getNowTandemDriverNum(index);
    if (isRollingSlot == 0) {
        if (((itemObjMgr->IsRollingSlot(index, 1) != 0) ||
            (itemObjMgr->getKartEquipItem(index, 1) != (ItemObj *)0x0)) &&
            (itemObjMgr->getKartEquipItem(index, 0) != (ItemObj *)0x0))
        {
            *((u32 *)(&kartBody->mCarStatus)) |= 0x40;       // 0x570
        }
    }
    else {
        isRollingSlot = itemObjMgr->IsRollingSlot(index, 0);
        if (((isRollingSlot != 0) ||
            (itemObjMgr->getKartEquipItem(index, 0) != (ItemObj *)0x0)) &&
            (itemObjMgr->getKartEquipItem(index, 1) != (ItemObj *)0x0))
        {
            *((u32 *)(&kartBody->mCarStatus)) |= 0x40;       // 0x570
        }
    }
    // FIX: 0x0 and 0x40 are reversed... need to swap them.
    if ((kartBody->mCarStatus & 0x40) == 0) {
        return false;
    }
    else {
        if (isRollingSlot == 0) {
            isRollingSlot = itemObjMgr->IsRollingSlot(index, 1);
            if (((isRollingSlot != 0) ||
                (itemObjMgr->getKartEquipItem(index, 1) != (ItemObj *)0x0)) &&
                (itemObjMgr->getKartEquipItem(index, 0) == (ItemObj *)0x0))
            {
                return true;
            }
        } else {
            isRollingSlot = itemObjMgr->IsRollingSlot(index, 0);
            if (((isRollingSlot != 0) ||
                (itemObjMgr->getKartEquipItem(index, 0) != (ItemObj *)0x0)) &&
                (itemObjMgr->getKartEquipItem(index, 1) == (ItemObj *)0x0))
            {
                return true;
            }
        }
    }
    return false;
}

bool KartCtrl::CheckTandemItmGet(int kartIndex) {
    const ItemObj *kartEquipItem;
    getKartBody(kartIndex);
    ItemObjMgr *itemObjMgr = GetItemObjMgr();
    u8 tandemDriverNum = ItemObjMgr::getNowTandemDriverNum(kartIndex);

    if (tandemDriverNum == 0) {
        kartEquipItem = itemObjMgr->getKartEquipItem(kartIndex, 0);
        if (kartEquipItem != 0) {
            return true;
        }
    }
    else {
        kartEquipItem = itemObjMgr->getKartEquipItem(kartIndex, 1);
        if (kartEquipItem != 0) {
            return true;
        }
    }
    return false;
}

bool KartCtrl::CheckTandemItmRollingGet(int kartIndex) {
    getKartBody(kartIndex);
    ItemObjMgr *itemObjMgr = GetItemObjMgr();
    u8 isRollingSlot = ItemObjMgr::getNowTandemDriverNum(kartIndex);
    
    if (isRollingSlot == 0) {
        isRollingSlot = itemObjMgr->IsRollingSlot(kartIndex, 0);
        if (isRollingSlot != 0) {
            return true;
        }
    }
    else {
        isRollingSlot = itemObjMgr->IsRollingSlot(kartIndex, 1);
        if (isRollingSlot != 0) {
            return true;
        }
    }
    return false;
}

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

bool KartCtrl::IsMiniGame() {
    const RaceMgr *raceMgr = RaceMgr::getCurrentManager();
    bool isMiniGame;
    
    if (
        raceMgr->getRaceMode() == 4 ||
        raceMgr->getRaceMode() == 5 ||
        raceMgr->getRaceMode() == 6 ||
        raceMgr->getRaceMode() == 7
    ) {
        isMiniGame = true;
    } else {
        isMiniGame = false;
    }
    return isMiniGame;
}

bool KartCtrl::IsMiniGameEnd() {
    if (GetKartCtrl()->IsMiniGame()) {
        const RaceMgr *raceMgr = RaceMgr::getCurrentManager();
        if (raceMgr->isRaceEnd() != 0) {
            return true;
        }
    }
    return false;
}

u8 KartCtrl::CheckWinner() {
    const KartBody *kartBody;
    const KartChecker *kartChecker;
    int kartNumber = GetKartCtrl()->GetKartNumber();
    int rank;
    
    for (int kartIndex = 0; kartIndex < kartNumber; kartIndex++) {
        kartBody = GetKartCtrl()->getKartBody(kartIndex);
        kartChecker = RCMGetKartChecker(kartBody->mMynum);
        kartChecker->getRank();
        rank = kartChecker->getRank();
        if (rank == 1) {
            return kartBody->mMynum;
        }
    }
    return 1;
}

void KartCtrl::GetKartEffctVel(int kartIndex, JGeometry::TVec3f *vecKartEffectVel) {
    const KartBody *kartBody = getKartBody(kartIndex);

    if ((kartBody->mGameStatus & 0x2000) != 0) {
        vecKartEffectVel->set(kartBody->mEffctVel);
    }
    else {
        vecKartEffectVel->zero();
    }
    return;
}

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

void KartStrat::DoStepAccel() {
    const KartGamePad *kartGamePadDriver;
    const KartGamePad *kartGamePadPassenger;
    const KartPad *kartPad;
    KartSus *kartSus;
    KartBody *kartBody;
    u8 testButton;
    float carRPM;
    
    kartBody = *(KartBody **)this;
    u8 myNum = kartBody->mMynum;

    KartCtrl *kartCtrl = GetKartCtrl();
    kartGamePadDriver = kartCtrl->GetDriveCont(myNum);
    kartGamePadPassenger = GetKartCtrl()->GetCoDriveCont(myNum);

    kartPad = GetKartCtrl()->getKartPad(myNum);
    testButton = kartGamePadDriver->testButton(kartPad->mAccelBtn);
    if (testButton != 0) {
        kartBody->_3c8 = GetKartCtrl()->fcnvge(kartBody->_3c8, kartBody->_3d0, kartBody->_3d4, kartBody->_3d4);
    } else {
        GetKartCtrl()->ChaseFnumber(&kartBody->_3c8, 0.0f, 0.8f);
    }

    kartPad = GetKartCtrl()->getKartPad(myNum);
    testButton = kartGamePadPassenger->testButton(kartPad->mAccelBtn);
    if (testButton != 0) {
        GetKartCtrl()->getKartAnime(myNum)->DoStartRunStartAnime(myNum);
    } else {
        GetKartCtrl()->getKartAnime(myNum)->DoStartRunEndAnime(myNum);
    }

    carRPM = 0.0f;
    for (int i = 0; i < 4; i++) {
        kartSus = GetKartCtrl()->getKartSus(i + myNum * 4);
        kartSus->_278 += kartBody->_3c8;
        kartSus->_278 -= GetKartCtrl()->LimmtNumber(kartSus->_278, 1.0f);
        kartSus->_278 = GetKartCtrl()->LimmtNumber(kartSus->_278, kartBody->_3e4);
        carRPM += kartSus->_278;
    }
    kartBody->mCarRPM = carRPM;
    if (kartBody->mCarRPM < 0.0f) {
        kartBody->mCarRPM = 0.0f;
    }
    kartBody->mCarRPM *= 40.0f;
    kartBody->mCarRPM *= 2.0f;
    kartBody->mCarRPM += 300.0f;
    return;
}

void KartStrat::DoStepSterr() {
    uint flagGameStatus;
    const RaceMgr *raceMgr;
    KartSus *kartSus1;
    KartSus *kartSus2;
    int i;
    KartBody *kartBody;
    uint myNum;
    KartCtrl *kartCtrl;
    float stepSterr;
    float stepSterrUnknown;
    
    kartBody = *(KartBody **)this;
    myNum = kartBody->mMynum;

    raceMgr = RaceMgr::getCurrentManager();
    kartCtrl = GetKartCtrl();
    
    i = myNum * 4;
    kartSus1 = kartCtrl->getKartSus(i + 0);
    kartCtrl = GetKartCtrl();
    kartSus2 = kartCtrl->getKartSus(i + 1);
    GetKartCtrl()->getKartSus(i + 2);
    GetKartCtrl()->getKartSus(i + 3);

    stepSterr = GetKartCtrl()->GetDriveCont(myNum)->getMainStickX();
    flagGameStatus = kartBody->mGameStatus & 8;
    stepSterrUnknown = 10.0f * stepSterr;

    if (((flagGameStatus != 0) && (stepSterrUnknown < 4.0f)) && (stepSterrUnknown > -4.0f)) {
        stepSterrUnknown = 0.0f;
    }

    stepSterr = 10.0f - stepSterrUnknown;

    if (flagGameStatus != 0) {
        stepSterr = GetKartCtrl()->fcnvge(kartBody->_38c, stepSterr, 0.4f, 0.4f);
        kartBody->_38c = stepSterr;
    } else {
        stepSterr = GetKartCtrl()->fcnvge(kartBody->_38c, stepSterr, 1.8f, 1.8f);
        kartBody->_38c = stepSterr;
    }

    kartBody->_388 = (kartBody->_38c - 10.0f) / 10.0f;
    GetKartCtrl()->RotYMatrix33(kartSus1->_188, 0.680333f * kartBody->_388);
    GetKartCtrl()->RotYMatrix33(kartSus2->_188, 0.680333f * kartBody->_388);

    if (raceMgr->getRaceMode() != 8) {
        GetKartCtrl()->getKartAnime(myNum)->DoDriveAnime(myNum);
    }
    return;
}

void KartStrat::DoSterr() {}

void KartStrat::DoAccel() {
    const KartGamePad *kartGamePad;
    const KartPad *kartPad;
    KartSound *kartSound;
    KartSus *kartSus;
    u8 checkCrash;
    u8 accelButtonPressed;
    float kartSpeed;
    float unknownRPM1;
    float unknownRPM2;
    int index;
    KartBody *kartBody;
    int offset;
    
    kartBody = *(KartBody **)this;
    index = kartBody->mMynum;
    kartGamePad = GetKartCtrl()->GetDriveCont(index);
    kartBody->_5c3 &= -6;

    if ((kartBody->mGameStatus & 4) == 0) {
        return;
    }

    checkCrash = GetKartCtrl()->getKartBody(index)->getChecker()->CheckCrash();
    if (checkCrash != 0) {
        return;
    }

    unknownRPM2 = kartBody->_3d0;
    if ((kartBody->mCarStatus & 0x8000020) != 0) {
        unknownRPM2 *= 0.2f;
    }

    accelButtonPressed = kartGamePad->testButton(GetKartCtrl()->getKartPad(index)->mAccelBtn);
    if (accelButtonPressed != 0) {
        kartBody->_3c8 = GetKartCtrl()->fcnvge(
            kartBody->_3c8, unknownRPM2, kartBody->_3d4, kartBody->_3d4
        );
    }
    else {
        GetKartCtrl()->ChaseFnumber(&kartBody->_3c8, 0.0f, 0.1f);
        if (kartBody->_3c8 < 0.5f) {
            kartBody->_3c8 = 0.0f;
        }
    }

    unknownRPM1 = 0.0f;
    offset = index * 4;
    unknownRPM2 = unknownRPM1;

    for (int i = 0; i < 4; i++) {
        kartSus = GetKartCtrl()->getKartSus(i + offset);
        unknownRPM2 += kartSus->_110;
    }

    if ((kartBody->mBodyGround.getAttribute() != 6) && (kartBody->getTouchNum() != 0)) {
        kartPad = GetKartCtrl()->getKartPad(index);
        accelButtonPressed = kartGamePad->testButton(kartPad->mBtnB);

        if (accelButtonPressed != 0) {
            GetKartCtrl()->ChaseFnumber(&kartBody->_3cc, kartBody->_3d8, kartBody->_3dc);
            kartBody->_5c3 |= 4;
            if ((kartBody->_5c3 & 2) == 0) {
                kartSound = GetKartCtrl()->getKartSound(index);
                kartSound->DoBrakeSound();
            }
        } else {
            goto lblDoAccelResetBitfield1;
        }
    } else {
        lblDoAccelResetBitfield1:
        kartBody->_3cc = 0.0f;
    }
    
    for (int i = 0; i < 4; i++) {
        kartSus = GetKartCtrl()->getKartSus(i + offset);
        kartSus->_110 += kartBody->_3c8;
        kartSus->_278 += kartBody->_3c8;
        kartSus->_110 -= GetKartCtrl()->LimmtNumber(kartSus->_110, kartBody->_3e0);
        kartSus->_278 -= GetKartCtrl()->LimmtNumber(kartSus->_278, 1.0f);
        kartSus->_110 = GetKartCtrl()->LimmtNumber(kartSus->_110, kartBody->_3e4);
        kartSus->_278 = GetKartCtrl()->LimmtNumber(kartSus->_278, kartBody->_3e4);
        kartSus->_110 -= kartBody->_3cc;
        unknownRPM1 += kartSus->_278;
    }

    JGeometry::TVec3f vecVelocity;
    GetKartCtrl()->DevMatrixByVector(&vecVelocity, (Vec *)&kartBody->mVel.x, kartBody->_110);
    kartSpeed = GetKartCtrl()->GetCarSpeed(kartBody->mMynum);

    if (kartSpeed <= 50.0f) {
        kartSpeed = GetKartCtrl()->GetCarSpeed(kartBody->mMynum);
        if ((kartSpeed > 10.0f) && (kartBody->_510 > 2.44222f)) {
            kartBody->_5c3 &= -6;
            kartBody->_5c3 |= 2;
        } else {
            goto lblDoAccelResetBitfield2;
        }
    } else {
    lblDoAccelResetBitfield2:
        kartBody->_5c3 &= ~0x2;
    }

    for (int i = 0; i < 4; i++) {
        kartSus = GetKartCtrl()->getKartSus(i + offset);
        GetKartCtrl()->SetTireDispRound(kartBody, kartSus, vecVelocity.z);
    }
    
    GetKartCtrl()->SetKartRpm(kartBody, unknownRPM1, unknownRPM2);
    return;
}

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
