#include "Kaneshige/RaceMgr.h"

#include "Sato/RivalItemCtrl.h"
#include "Sato/RivalKart.h"
#include "Sato/RivalBodyCtrl.h"
#include "Sato/RivalSpeedCtrl.h"

#include "Yamamoto/kartBody.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

RivalKart::RivalKart(KartBody *body)
{
    mBody = body;
    mNo = body->mMynum;
    _34 = 0;

    for (u8 i = 0; i < 4; i++)
        mSus[i] = nullptr;

    if (RCMGetManager()->isStaffRoll())
    {
        mBodyCtrl = new RivalAwardBodyCtrl(this);
        mSpeedCtrl = new RivalStaffRollDemoSpeedCtrl(this);
    }
    else if (RCMGetManager()->isAwardDemoMode())
    {
        mBodyCtrl = ((mNo == 0) ? new RivalAwardBodyCtrl(this) : new RivalAward2ndBodyCtrl(this));
        mSpeedCtrl = ((mNo == 0) ? new RivalAwardDemoSpeedCtrl(this) : new RivalAwardDemo2ndSpeedCtrl(this));
    }
    else
    {
        mBodyCtrl = new RivalBodyCtrl(this);
        mSpeedCtrl = new RivalSpeedCtrl(this);
    }

    mItemCtrl = new RivalItemCtrl(this);
}

RivalKart::~RivalKart() {}

void RivalKart::Init()
{
    _34 = 0;
    mBodyCtrl->init();
    mSpeedCtrl->init();
    mItemCtrl->init();
}

void RivalKart::StartInit()
{
    RivalSpeedCtrl::LoadCrsLevelParam();
    RivalBodyCtrl::reset();
    RivalSpeedCtrl::StartInit();
}

void RivalKart::Reset()
{
    RivalBodyCtrl::reset();
    RivalSpeedCtrl::ResetInit();
}

void RivalKart::ResetInit()
{
    Init();
}

void RivalKart::setRankAry()
{
    RivalBodyCtrl::chkPlayerKartCondition();
    RivalBodyCtrl::setRankArray();
}

f32 RivalKart::getMaxSpeed() const
{
    return mSpeedCtrl->getMaxSpeed();
}

f32 RivalKart::getOffsetMaxSpeed() const
{
    return mSpeedCtrl->getOffsetMaxSpeed();
}

void RivalKart::setMaxSpeed(const float &max)
{
    mSpeedCtrl->setMaxSpeed(max);
}

void RivalKart::ContempLate()
{
    mBodyCtrl->chkBodyCondition();
    mBodyCtrl->doColWallCtrl();

    mBodyCtrl->doSterrCtrl();
    mBodyCtrl->doDriftCtrl();
    mBodyCtrl->doChangeDriverCtrl();
    mSpeedCtrl->doSpeedCtrl();

    mItemCtrl->doItemCtrl();
}

void RivalKart::ActiveCtrl() {}