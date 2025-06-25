#include "Kaneshige/RaceMgr.h"
#include "Sato/JPEffectPerformer.h"
#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/kartCamera.h"
#include "Yamamoto/KartDamage.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartDamage::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    mFlags = 0;
    _6 = 0;
    mDamageState = 0;
    mTimer = 0;
}

void KartDamage::ClrSetDamage()
{
    mFlags &= ~(0x80 | 0x8 | 0x4 | 0x1);
}

void KartDamage::ClrAllDamage()
{
    mFlags &= ~(0x100 | 0x80 | 0x40 | 0x20 | 0x10 | 0x8 | 0x4 | 0x2 | 0x1);
}

bool KartDamage::TstSetDamage()
{
    return mFlags & (0x100 | 0x80 | 0x40 | 0x20 | 0x10 | 0x8 | 0x4 | 0x2 | 0x1);
}

void KartDamage::SetDamager()
{
    if (mFlags & 1)
        MakeDamage();

    if (mFlags & 8)
        MakeBigDamage();

    if (mFlags & 0x80)
        MakeBurnDamage();

    ClrSetDamage();
    SetDriverEndDamageAnime();
}

void KartDamage::SetSetDamage()
{
    mDamageState = 0;

    if (GetKartCtrl()->getKartAnime(mBody->mMynum)->IsDamageL(mBody->mMynum))
    {
        mFlags = 0;
        mFlags |= 8;
        mFlags &= ~(0x80 | 0x1);
    }
    else
    {
        mFlags = 0;
        mFlags |= 1;
        mFlags &= ~(0x80 | 0x8);
    }
}

void KartDamage::SetSetBurn()
{
    mDamageState = 0;
    KartBody *body = mBody;

    if (GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
    {
        mFlags = 0;
        mFlags |= 8;
        mFlags &= ~(0x80 | 0x1);
    }
    else if (GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageS(body->mMynum))
    {
        mFlags = 0;
        mFlags |= 1;
        mFlags &= ~(0x80 | 0x8);
    }
    else
    {
        mFlags = 0;
        mFlags |= 0x80;
        mFlags &= ~(0x8 | 0x1);
    }
}

void KartDamage::SetSetBigDamage()
{
    mDamageState = 0;
    mFlags = 0;
    mFlags |= 8;
    mFlags &= ~(0x80 | 0x1);

    if (GetKartCtrl()->CheckCamera(mBody->mMynum))
    {
        int camNo = GetKartCtrl()->GetCameraNum(mBody->mMynum);
        GetKartCtrl()->getKartCam(camNo)->MakeShaker(0.4f);
    }

    mBody->getGame()->DoItmCancel();
}

void KartDamage::SetDriverDamageAnime()
{
    KartBody *body = mBody;

    if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDrive(body->mMynum))
        GetKartCtrl()->getKartAnime(body->mMynum)->mDamageFlags |= 0x20000;

    GetKartCtrl()->getKartAnime(body->mMynum)->mDamageFlags &= ~0x40000;
}

void KartDamage::SetDriverEndDamageAnime()
{
    KartBody *body = mBody;
    if (GetKartCtrl()->getKartAnime(body->mMynum)->IsDrive(body->mMynum))
        GetKartCtrl()->getKartAnime(body->mMynum)->mDamageFlags |= 0x40000;
}

void KartDamage::SetDamageAnime()
{
    KartBody *body = mBody;
    SetSetDamage();
    if ((body->mCarStatus & 0x2000000000000ull) == 0)
    {
        SetDriverDamageAnime();
    }

    if ((body->getDamage()->mFlags & 8))
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x80000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~0x20000;
    }
    else
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageS(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x20000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~0x80000;
    }

    if (body->getThunder()->mFlags & 1)
        return;

    if (GetKartCtrl()->CheckCamera(body->mMynum))
    {
        GetKartCtrl()->getKartCam(GetKartCtrl()->GetCameraNum(body->mMynum))->MakeShaker(0.4f);
    }
}

void KartDamage::SetBurnAnime()
{
    KartBody *body = mBody;
    SetSetBurn();
    SetDriverDamageAnime();

    if ((body->getDamage()->mFlags & 8))
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x80000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x40000000 | 0x20000);
    }
    else if ((body->getDamage()->mFlags & 1))
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageS(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x20000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x40000000 | 0x20000);
    }
    else
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsBurnAnime(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x40000000;
        }
        JPEffectPerformer::setEffect(JPEffectPerformer::Effect_Burn, body->mMynum, body->mPos, 2);

        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x80000 | 0x20000);
    }
}

void KartDamage::SetBigDamageAnime()
{
    KartBody *body = mBody;

    if (RaceMgr::getCurrentManager()->isTinyProcess())
    {
        SetDamageAnime();
    }
    else
    {
        SetDriverDamageAnime();
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x80000;
        SetSetBigDamage();
    }
}

void KartDamage::MakeHalfDamage()
{
    KartBody *body = mBody;
    body->getGame()->MakeClear();

    if (GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
    {
        mFlags = 0;
        mFlags |= 0x48;
    }
    else
    {
        mFlags = 0;
        mFlags |= 0x20;
    }

    if ((body->getDamage()->mFlags & 8))
    {
        mFlags &= ~8;
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageL(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x80000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x20000);
    }
    else
    {
        if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamageS(body->mMynum))
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x20000;
        }
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x80000);
    }
    mDamageState = 1;
    mTimer = 60;
}

void KartDamage::MakeDamage()
{
    mBody->getGame()->MakeClear();
    mFlags |= 0x10;
    mDamageState = 1;
    mTimer = 20;
}

void KartDamage::MakeBigDamage()
{
    KartBody *body = mBody;
    body->getGame()->MakeClear();
    mFlags |= 0x40;
    mDamageState = 1;
    mTimer = 300;
    GetKartCtrl()->getKartSound(body->mMynum)->DoDragVoice();
}

void KartDamage::MakeBurnDamage()
{
    mBody->getGame()->MakeClear();
    mFlags |= 0x100;
    mDamageState = 1;
    mTimer = 90;
}

void KartDamage::DoDamageCrl()
{
    KartBody *body = mBody;

    if (body->mCarStatus & 0x2000000000000ull)
        return;

    if (!(mFlags & (0x100 | 0x40 | 0x20 | 0x10)))
        return;

    if (mFlags & 0x10)
    {
        switch (mDamageState)
        {
        case 1: // Damage Timer Active
        {
            mTimer--;
            if (mTimer == 0)
            {
                GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x40000;
                mDamageState = 2;
                mTimer = 0; // ah yes, check if timer is zero and set it to zero if it's zero
            }
            break;
        }
        case 2: // Damage End
        {
            if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamage(body->mMynum))
            {
                mDamageState = 0;
                mFlags &= ~0x10;
                body->mCarStatus &= ~0x80000000ull;
            }
            break;
        }
        }
    }
    else if (mFlags & 0x20)
    {
        switch (mDamageState)
        {
        case 1: // Damage Timer Active
        {
            mTimer--;
            if (mTimer == 0)
            {
                GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x40000;
                mDamageState = 2;
                mTimer = 0; // ah yes, check if timer is zero and set it to zero if it's zero
            }
            break;
        }
        case 2: // Damage End
        {
            if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsDamage(body->mMynum))
            {
                mDamageState = 0;
                mFlags = 0;
                body->mCarStatus &= ~0x80000000ull;
            }
            break;
        }
        }
    }
    else if (mFlags & 0x100)
    {
        switch (mDamageState)
        {
        case 1: // Damage Timer Active
        {
            mTimer--;
            if (mTimer == 0)
            {
                GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x80000000;
                mDamageState = 2;
                mTimer = 0; // ah yes, check if timer is zero and set it to zero if it's zero
            }
            break;
        }
        case 2: // Damage End
        {
            if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsBurnAnime(body->mMynum))
            {
                mDamageState = 0;
                mFlags &= ~0x100;
                body->mCarStatus &= ~0x80000000ull;
            }
            break;
        }
        }
    }
    else
    {
        if (GetKartCtrl()->CheckTandemItmGet(body->mMynum) && mDamageState == 1)
        {
            GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x100000;
            GetKartCtrl()->getKartSound(body->mMynum)->DoTandemVoice(0);
            mDamageState = 2;
            mTimer = 10;
        }
        switch (mDamageState)
        {
        case 1: // Damage Timer Active
        {
            mTimer--;
            if (mTimer == 0)
            {
                GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= 0x100000;
                GetKartCtrl()->getKartSound(body->mMynum)->DoTandemVoice(0);
                mDamageState = 2;
                mTimer = 10;
            }
            break;
        }
        case 2: // Damage End
        {
            if (!GetKartCtrl()->getKartAnime(body->mMynum)->IsNormalStart(body->mMynum))
            {
                mDamageState = 0;
                mFlags &= ~0x40;
                body->mCarStatus &= ~0x80000000ull;
            }
            break;
        }
        }
    }
}
