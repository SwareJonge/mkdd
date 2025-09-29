#include "Yamamoto/KartCheck.h"
#include "Yamamoto/KartCtrl.h"
#include "Kaneshige/RaceMgr.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

void KartCheck::Init(int kartIdx) { 
    mBody = GetKartCtrl()->getKartBody(kartIdx); 
    mField4 = 0; 
}

bool KartCheck::CheckCrash()
{
    // Check crash status bits in mCarStatus (high: 0x3001, low: 0x500000)
    if ((mBody->mCarStatus & 0x0000300100500000ULL) != 0) {
        return true;
    } else {
        return (u8)((mBody->getDossin()->_14 & 3) != 0);
    }
}

bool KartCheck::CheckCheange(int kartIdx) {
    // Check multiple status conditions - bit 0x80 must be zero
    if (((mBody->mCarStatus & 0x0000300300500880ULL) != 0) && 
        ((mBody->mCarStatus & 0x0000000000000080ULL) == 0)) {
        return true;
    }

    if ((mBody->_57c & 9) != 0) {
        return true;
    }
    
    if ((mBody->_580 & 9) != 0) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsThrow(kartIdx)) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsDangerPlusAnime(kartIdx)) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsDamage(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRocket(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsBurnAnime(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRun(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRide(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsGaol(kartIdx) == 1) {
        return true;
    }
    
    if (mBody->getDamage()->TstSetDamage()) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsNormalStart(kartIdx) == 1) {
        return true;
    }
    
    if ((GetKartCtrl()->getKartAnime(kartIdx)->mFlags & 0x4000) != 0) {
        return true;
    }
    
    return (u8)(GetKartCtrl()->getKartAnime(kartIdx)->IsRocket(kartIdx) == 1);
}

bool KartCheck::CheckSpecialDyn() {
    if ((mBody->mCarStatus & (0x1001ULL << 32 | 0x400000ULL)) != 0) {
        return true;
    }

    if ((mBody->getCannon()->mFlags & 0x20) != 0 || (mBody->getRescue()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((mBody->getPipe()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((mBody->getAnt()->mFlags & 0x20) != 0) {
        return true;
    }
    
    return (mBody->getRescue()->mFlags >> 5) & 1;
}

bool KartCheck::CheckSpecialDyn2() {
    if ((mBody->mCarStatus & 0x0000000100000000ULL) != 0) {
        return true;
    }
    
    if ((mBody->getCannon()->mFlags & 0x20) != 0 || (mBody->getRescue()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((mBody->getPipe()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((mBody->getAnt()->mFlags & 0x20) != 0) {
        return true;
    }
    
    return (mBody->getRescue()->mFlags >> 5) & 1;
}

bool KartCheck::CheckBodyHit(int kartIdx1, int kartIdx2) {
    // Check collision validity between two karts
    KartBody* body1 = GetKartCtrl()->getKartBody(kartIdx1);
    KartBody* body2 = GetKartCtrl()->getKartBody(kartIdx2);
    
    if ((body1->getDossin()->_14 & 0x20) != 0) {
        return true;
    }
    
    if ((body2->getDossin()->_14 & 0x20) != 0) {
        return true;
    }
    
    if ((body1->getPipe()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((body2->getPipe()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((body1->getAnt()->mFlags & 0x20) != 0) {
        return true;
    }
    
    if ((body2->getAnt()->mFlags & 0x20) != 0) {
        return true;
    }
    
    u32 status1 = body1->mGameStatus;
    
    if ((status1 & 0x10) != 0) {
        return true;
    }

    u32 status2 = body2->mGameStatus;
    
    if ((status2 & 0x10) != 0) {
        return true;
    }
    
    if ((status1 & 0x8000) != 0) {
        return true;
    }
    
    if ((status2 & 0x8000) != 0) {
        return true;
    }
    
    if ((status1 & 0x2000) == 0) {
        return true;
    }
    
    if ((status2 & 0x2000) == 0) {
        return true;
    }
    
    if ((status1 & 0x400) != 0) {
        return true;
    }
    
    if ((status2 & 0x400) != 0) {
        return true;
    }
    
    if (body1->getChecker()->CheckSpecialDyn() || body2->getChecker()->CheckSpecialDyn()) {
        return true;
    }
    
    return false;
}

bool KartCheck::CheckAllClearKey() {
    u32 status = mBody->mGameStatus;
  
    if (((status & 4) != 0) && ((status & 8) != 0)) {
        return true;
    }

    return false;
}

bool KartCheck::CheckAllClearKeyT() {
    RaceMgr* raceMgr = RaceMgr::getCurrentManager();
    
    if ((mBody->mCarStatus & 0x0000000002500800ULL) != 0) {
        return true;
    }
    
    KartScene* scene = mBody->getScene();
    if (scene->mChapter <= 4) {
        return true;
    }
    
    if ((mBody->getDossin()->_14 & 1) != 0) {
        return true;
    }
    
    if ((mBody->mGameStatus & 0x400) != 0) {
        return true;
    }
    
    if ((raceMgr->isRaceEnd() && raceMgr->getRaceMode() == 4) || mBody->getGame()->CheckBalloon()) {
        return true;
    }
    
    return false;
}

bool KartCheck::CheckPartsClearKey(int kartIdx) {
    if ((mBody->mCarStatus & 0x0000300300000080ULL) != 0) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRun(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsDamage(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRide(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsBurnAnime(kartIdx) == 1) {
        return true;
    }
    
    if (mBody->getDamage()->TstSetDamage()) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsNormalStart(kartIdx) == 1) {
        return true;
    }
    
    if ((GetKartCtrl()->getKartAnime(kartIdx)->mFlags & 0x4000) != 0) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRocket(kartIdx) == 1) {
        return true;
    }
    
    return (u8)(GetKartCtrl()->getKartAnime(kartIdx)->IsDangerPlusAnime(kartIdx) != 0);
}

bool KartCheck::CheckOnlyTandemPartsClearKey(int kartIdx) {
    // Similar to CheckPartsClearKey but without IsBurnAnime check
    if ((mBody->mCarStatus & 0x0000300300000080ULL) != 0) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRun(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsDamage(kartIdx) == 1) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRide(kartIdx) == 1) {
        return true;
    }
    
    if (mBody->getDamage()->TstSetDamage()) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsNormalStart(kartIdx) == 1) {
        return true;
    }
    
    if ((GetKartCtrl()->getKartAnime(kartIdx)->mFlags & 0x4000) != 0) {
        return true;
    }
    
    if (GetKartCtrl()->getKartAnime(kartIdx)->IsRocket(kartIdx) == 1) {
        return true;
    }
    
    return (u8)(GetKartCtrl()->getKartAnime(kartIdx)->IsDangerPlusAnime(kartIdx) != 0);
}

bool KartCheck::CheckPlayer() {
    int padPort = GetKartCtrl()->GetDriveCont(mBody->mMynum)->getPadPort();
    
    if ((mBody->mGameStatus & 2) != 0 && padPort >= 0) {
        return true;
    }
    
    return false;
}

bool KartCheck::CheckDraw(u32 cameraIdx, int targetNum) {
    u8 cameraTarget = GetKartCtrl()->getKartCam(cameraIdx)->GetTargetNum();
    
    if (targetNum == cameraTarget && ((mBody->getThunder()->mFlags & 1) == 0) && ((mBody->mGameStatus & 0x1000) != 0)) {
        return true;
    }
    
    return false;
}

bool KartCheck::CheckIndication() {
    // Extract bit 13 from game status
    return (mBody->mGameStatus >> 13) & 1;
}
