#include "Yamamoto/KartAnt.h"
#include "Yamamoto/kartCtrl.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartAnt::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    mFlags = 0;
    mState = 0;
}

void KartAnt::DoKeep()
{
    KartBody *body = mBody;

    body->mVel.zero();
    body->_2cc.zero();
    body->mWg.zero();
    body->_2c0.zero();
    body->_2d8.zero();

    body->_4d8 = 0.0f;
    body->_4d4 = 0.0f;
    body->_4c4 = 0.0f;
    body->_4c0 = 0.0f;
    body->_3c8 = 0.0f;
    body->_3cc = 0.0f;
    body->mTireAngle = 0.0f;
    body->_394 = 0.0f;
    body->mLiftframe = 0.0f;
}

void KartAnt::DoCatch()
{
    KartBody *body = mBody;
    mFlags |= 0x20;
    SetPose();
    body->mShadowModel->clrVisibleAll();
}

void KartAnt::SetPose()
{
    KartBody *body = mBody;

    JGeometry::TVec3f pos, zDir, v3, v4, upVec, v6, v7;

    mAntLion->getZDir(&zDir);
    mAntLion->getPosition(&pos);
    pos.y = pos.y - 300.0f;
    upVec.set(0.0f, 1.0f, 0.0f);
    v6.cross(upVec, zDir);
    v6.normalize();
    v7.cross(v6, upVec);
    v7.normalize();
    upVec.cross(v7, v6);
    upVec.normalize();

    body->_110[0][0] = v6.x;
    body->_110[1][0] = v6.y;
    body->_110[2][0] = v6.z;
    body->_110[0][1] = upVec.x;
    body->_110[1][1] = upVec.y;
    body->_110[2][1] = upVec.z;
    body->_110[0][2] = v7.x;
    body->_110[1][2] = v7.y;
    body->_110[2][2] = v7.z;
    body->_110[0][3] = pos.x;
    body->_110[1][3] = pos.y;
    body->_110[2][3] = pos.z;

    body->mPos.set(pos);
    PSMTXCopy(body->_110, body->mPlayerPosMtx);

    f32 offset = GetKartCtrl()->GetKartBodyOffset(body->mMynum);
    body->mPlayerPosMtx[0][3] += body->mPlayerPosMtx[0][1] * offset;
    body->mPlayerPosMtx[1][3] += body->mPlayerPosMtx[1][1] * offset;
    body->mPlayerPosMtx[2][3] += body->mPlayerPosMtx[2][1] * offset;
    body->_2f0.set(body->_110[0][0], body->_110[1][0], body->_110[2][0]);
    body->_2fc.set(body->_110[0][1], body->_110[1][1], body->_110[2][1]);
    body->_308.set(body->_110[0][2], body->_110[1][2], body->_110[2][2]);
}

void KartAnt::DoShoot()
{
    KartBody *body = mBody;

    f32 scale = 300.0f * body->_3a4;

    _20.set(body->_308);

    body->_2cc.x = body->_308.x * scale;
    body->_2cc.y = body->_308.y * scale;
    body->_2cc.z = body->_308.z * scale;
    body->_2cc.y += scale;

    body->mShadowModel->setVisibleAll();
    mFlags &= ~0x2;
}

void KartAnt::DoCheckEnd()
{
    KartBody *body = mBody;

    if (mTimer <= 40)
    {
        mTimer++;
        f32 scale = 10.0f * body->_3a4;
        body->_2cc.x = _20.x * scale;
        body->_2cc.y = _20.y * scale;
        body->_2cc.z = _20.z * scale;
        body->_2cc.y = scale;
    }
    else
    {
        mFlags &= ~0x1;
    }

    if (body->getTouchNum() != 0)
    {
        body->mCarStatus &= ~0x200000000000;
        body->_584 = 0;
        mState = 6;
        mTimer = 0;
    }
}

void KartAnt::DoAntCrl()
{
    KartBody *body = mBody;

    switch (mState)
    {
    case 0:
        DoCatch();
        mState = 1;
        mTimer = 0;
        break;
    case 1:
        mTimer++;
        if (mTimer == 3)
        {
            mAntLion->eat();
        }
        if (mTimer == 50)
        {
            mState = 2;
            mTimer = 0;
            DoShoot();
        }
        break;
    case 2:
    {
        mTimer++;
        if (mTimer == 10)
        {
            mFlags &= ~0x20;
            mState = 3;
            mTimer = 0;
        }
        break;
    }
    case 3:
        DoCheckEnd();
        break;
    }
}

void KartAnt::DoAfterAntCrl()
{
    KartBody *body = mBody;

    switch (mState)
    {
    case 0:
        DoKeep();
        break;
    case 1:
        DoKeep();
        break;
    case 2:
    case 3:
        body->mWg.zero();
        break;
    }
}
