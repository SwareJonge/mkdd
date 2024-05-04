#include "Kaneshige/RaceMgr.h"
#include "Osako/SystemRecord.h"

#include "Yamamoto/kartCtrl.h"

#include "Yamamoto/KartHandle.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartHandle::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    mRumbleTimer = 0;
    mRumbleStrength = 0;
    _9 = 0;
}

bool KartHandle::DoMotor(MotorManager::MotorType motorType)
{
    KartGamePad::PadPort port = GetKartCtrl()->GetDriveCont(mBody->mMynum)->getPadPort();

    if (port < KartGamePad::PORT_1)
        return false;

    if (SIProbe(port) == SI_GC_STEERING)
    {
        LGWheels *wheels = JUTGamePad::getLGWheels();

        if (!gSystemRecord.isRumble() || RCMIsPause() || RCMGetManager()->isRaceEnd() || RCMGetManager()->isCrsDemoMode() || RCMGetManager()->isReplayMode() || RCMGetManager()->isWaitDemoMode())
        {
            return true;
        }
        if (mBody->mGameStatus & (0x8000 | 0x80 | 0x10 | 0x8))
        {
            return true;
        }
        switch (motorType)
        {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 18:
            mRumbleStrength = 0xf0;
            mRumbleTimer = 30;
            wheels->PlayBumpyRoadEffect(port, mRumbleStrength);
        }

        return true;
    }
    return false;
}

bool KartHandle::DoPowerMotor(f32 strength, u8 p2, u8 p3)
{
    KartGamePad::PadPort port = GetKartCtrl()->GetDriveCont(mBody->mMynum)->getPadPort();

    if (port < KartGamePad::PORT_1)
        return false;

    if (SIProbe(port) == SI_GC_STEERING)
    {
        LGWheels *wheels = JUTGamePad::getLGWheels();

        if (mBody->mGameStatus & (0x8000 | 0x80 | 0x10 | 0x8))
        {
            return true;
        }

        if (!gSystemRecord.isRumble() || RCMIsPause() || RCMGetManager()->isRaceEnd() || RCMGetManager()->isCrsDemoMode() || RCMGetManager()->isReplayMode() || RCMGetManager()->isWaitDemoMode())
        {
            return true;
        }

        mRumbleStrength = 240.0f * strength;
        mRumbleTimer = p2 + p3 + 1;
        wheels->PlayBumpyRoadEffect(port, mRumbleStrength);
        return true;
    }
    return false;
}

void KartHandle::StopMotor()
{
    KartGamePad::PadPort port = GetKartCtrl()->GetDriveCont(mBody->mMynum)->getPadPort();
    if (port >= KartGamePad::PORT_1 && SIProbe(port) == SI_GC_STEERING)
    {
        LGWheels *wheels = JUTGamePad::getLGWheels();
        mRumbleTimer = 0;
        wheels->StopBumpyRoadEffect(port);
    }
}

void KartHandle::WatchFrontalCollisionForce()
{
    KartGamePad::PadPort port = GetKartCtrl()->GetDriveCont(mBody->mMynum)->getPadPort();
    if (port >= KartGamePad::PORT_1 && SIProbe(port) == SI_GC_STEERING)
    {
        LGWheels *wheels = JUTGamePad::getLGWheels();

        if (!gSystemRecord.isRumble() || RCMIsPause() || RCMGetManager()->isRaceEnd() || RCMGetManager()->isCrsDemoMode() || RCMGetManager()->isReplayMode() || RCMGetManager()->isWaitDemoMode())
        {
            if (mRumbleTimer != 0)
            {
                mRumbleTimer = 0;
                wheels->StopBumpyRoadEffect(port);
            }
            return;
        }
        if (mBody->mGameStatus & (0x8000 | 0x80 | 0x10 | 0x8))
        {
            if (mRumbleTimer == 0)
            {
                return;
            }
            mRumbleTimer = 0;
            wheels->StopBumpyRoadEffect(port);
            return;
        }
        else if (mRumbleTimer != 0)
        {
            mRumbleTimer--;
            if (mRumbleTimer != 0)
                return;
            wheels->StopBumpyRoadEffect(port);
            return;
        }

        if (mBody->mCarStatus & 0x40)
        {
            wheels->PlayCarAirborne(port);
        }
        else
        {
            wheels->StopCarAirborne(port);
        }

        switch (mBody->mBodyGround.getMaterial())
        {
        case 0:
        case 1:
        case 2:
        case 8:
        case 9:
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xf:
        case 0x14:
        case 0x16:
            wheels->StopSlipperyRoadEffect(port);
            return;
        case 3:
        case 4:
        case 5:
        case 0xe:
            wheels->PlaySlipperyRoadEffect(port, 1);
            return;
        case 7:
            wheels->PlaySlipperyRoadEffect(port, 2);
            return;
        case 6:
            wheels->PlaySlipperyRoadEffect(port, 0x1e);
            return;

        default:
            return;
        }
    }
}