#include "Kaneshige/RaceMgr.h"
#include "Sato/JPEffectPerformer.h"
#include "Yamamoto/kartCtrl.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartStar::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    _04 = 0;
    _05 = 0;
    mTimer = 0;
}

void KartStar::MakeStar()
{
    KartBody *body = mBody;
    RaceMgr *raceMgr = RaceMgr::getCurrentManager();
    body->mCarStatus |= 0x40000;

    if (raceMgr->getRaceMode() == ESCAPE_BATTLE)
        mTimer = 360;
    else
        mTimer = 450;

    JPEffectPerformer::setEffect(JPEffectPerformer::Effect_Star, body->mMynum, body->mPos, 2);
    GetKartCtrl()->getKartSound(body->mMynum)->DoStarStartSound();
    RaceMgr::getManager()->getKartDrawer(body->mMynum)->playTevAnm(0);
}

void KartStar::StopStar()
{
    KartBody *body = mBody;
    body->mCarStatus &= ~0x40000;

    GetKartCtrl()->getKartSound(body->mMynum)->DoStarStopSound();
    RaceMgr::getManager()->getKartDrawer(body->mMynum)->stopTevAnm();
}

void KartStar::DoStarCrl()
{
    mTimer--;
    if (mTimer != 0)
        return;

    StopStar();
}