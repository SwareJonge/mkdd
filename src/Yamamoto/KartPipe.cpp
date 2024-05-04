#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Sato/RivalBodyCtrl.h"
#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/kartCamera.h"
#include "Yamamoto/KartPipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartPipe::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    mFlags = 0;
    mState = 0;
}

void KartPipe::MakePipe() {
    KartBody *body = mBody;
    u8 kartNo = body->mMynum;

    body->getGame()->MakeClear();
    body->mCarStatus &= (0x40000 | 0x1000);
    body->_584 = 0x10;

    mState = 0;
    mTimer = 0;

    body->getItem()->ReleseWanWan();

    body->mCarStatus |= (0x100000000000ull);
    body->mCarStatus &= ~(0x80000000ull | 0xB00000ull | 0x2000ull);

    GetKartCtrl()->getKartAnime(body->mMynum)->mFlags &= ~(0x400000ull | 0x2000ull);
    body->mCarStatus &= ~(0x1000000000000ull | 0x800000000000ull | 0x2ull | 0x1ull);

    body->getDamage()->ClrAllDamage();

    if( GetKartCtrl()->getKartAnime(body->mMynum)->IsDamage(body->mMynum) == true) {
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= (0x40000ull);
    }

    body->getDamage()->SetDriverEndDamageAnime();
#ifndef REGION_EU // NOTE: got added in NTSC-U build?
    if (GetKartCtrl()->getKartAnime(body->mMynum)->IsBurnAnime(body->mMynum))
    {
        GetKartCtrl()->getKartAnime(body->mMynum)->mFlags |= (0x80000000ull);
    }
#endif

    mSpeed = GetKartCtrl()->GetCarSpeed(kartNo);
    RCMGetKartChecker(kartNo)->stillRabbitTimer();
}

void KartPipe::DoKeep()
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

void KartPipe::DoCatch()
{
    KartBody *body = mBody;
    mPoint = body->mBodyGround.getPipePoint();
    mCameraID = mPoint->getCameraID();
    RCMGetKartChecker(body->mMynum)->beginWarp(nullptr);
}

void KartPipe::SetPose()
{
    KartBody *body = mBody;

    JGeometry::TVec3f pos, zDir, v3, v4, upVec, v6, v7;

    zDir.set(body->_308);
    pos.set(body->mPos);

    if(mPoint) {
        CLPoint *clPoint = mPoint->getCLPoint();
        GetKartCtrl()->getKartTarget(body->mMynum)->DoSearchRescueTarget(clPoint);

        if(body->mGameStatus & 8) {

            GetKartCtrl()->getKartEnemy(body->mMynum)->getBodyCtrl()->comebackRescure(clPoint, false);
        }

        mPoint->getPosition(&pos);
        mPoint->getFrDirection(&zDir);
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

    }

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

    GetKartCtrl()->getKartTarget(body->mMynum)->DoSearchTarget(body->mMynum);
}

void KartPipe::DoShoot()
{
    KartBody *body = mBody;

    f32 scale = 800.0f * body->_3a4;

    body->_2cc.x = body->_308.x * scale;
    body->_2cc.y = body->_308.y * scale;
    body->_2cc.z = body->_308.z * scale;
    f32 yInc = 800.0f * body->_3a4;
    body->_2cc.y += (yInc);

    RCMGetKartChecker(body->mMynum)->endWarp();
}
void KartPipe::DoCheckOutView() 
{
    KartBody *body = mBody;
    RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCameraID);
     
    if(_c < 0xffff)
        _c++;

    if (_c == 15)
    {
        if (GetKartCtrl()->CheckCamera(body->mMynum))
        {
            int camNum = GetKartCtrl()->GetCameraNum(body->mMynum);

            if (GetKartCtrl()->getKartCam(camNum)->GetCameraMode() != 2)
            {
                GetKartCtrl()->getKartCam(camNum)->ClrFlag(0x400);
            }
        }
        this->mState = 5;
        this->mTimer = 0;
    }
}

void KartPipe::DoCheckEnd() 
{
    KartBody *body = mBody;
    mTimer++;

    if(mTimer != 10)
        return;

    RCMGetKartChecker(body->mMynum)->resumeRabbitTimer();
    mFlags &= ~0x20;

    body->mCarStatus &= ~0x100000000000;
    body->_584 = 0;

    mState = 6;
    mTimer = 0;
}

void KartPipe::DoPipeCrl()
{
    KartBody *body = mBody;
    switch (mState)
    {
    case 0:
        DoCatch();
        mState = 1;
        mTimer = 0;
        body->getStrat()->DoMotor(MotorManager::MotorType_22);
        GetKartCtrl()->getKartSound(body->mMynum)->DoKartsetSeSound(0x10055);
        break;
    case 1:
        mSpeed = GetKartCtrl()->fcnvge(mSpeed, 0.0f, 5.0f, 5.0f);
        if (mSpeed > 100.0f)
        {
            mSpeed = 100.0f;
        }
        mTimer++;
        body->mBodyGround.reset();
        mSpeed = 140.0f;
        
        SetPose();

        if(GetKartCtrl()->CheckCamera(body->mMynum)) {
            int camNum = GetKartCtrl()->GetCameraNum(body->mMynum);

            if (GetKartCtrl()->getKartCam(camNum)->GetCameraMode() == 2)
            {
                GetKartCtrl()->getKartCam(camNum)->GetDemoCam()->InitID();
            }
            else
            {
                GetKartCtrl()->getKartCam(camNum)->InitPipeView();
                GetKartCtrl()->getKartCam(camNum)->SetFlag(0x400);
            }
        }

        this->mState = 2;
        this->mTimer = 0;
        break;
    case 2:
    {
        mFlags |= 0x20;
        mTimer++;
        if (mTimer == 2)
        {
            mState = 3;
            mTimer = 0;
        }
        break;
    }
    case 3:
    {
        DoShoot();
        mState = 4;
        mTimer = 0;
        _c = 0;
        break;
    }
    case 4:
        DoCheckOutView();
        break;
        break;
    case 5:
        DoCheckEnd();
        break;
    }
}

void KartPipe::DoAfterPipeCrl()
{
    switch (mState)
    {
    case 2:
        DoKeep();
        break;
    }
}