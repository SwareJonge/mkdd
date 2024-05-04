#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/SimpleDrawer.h"
#include "Kaneshige/KartMtx.h"
#include "Osako/system.h"
#include "Sato/RivalKart.h"
#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/kartCamera.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

KartCtrl *KartCtrl::mCtrl;

// why is GetKartCtrl() used here sometimes?

void KartCtrl::Init(KartBody *body, KartSus **sus, KartAppendix **appendix, KartSound *sound, KartTarget *target, KartDisp *disp, int idx)
{
    RaceMgr *rcm = RaceMgr::getCurrentManager();
    mGamePads[idx][0] = rcm->getKartChecker(idx)->getDriverPad(0);
    mGamePads[idx][1] = rcm->getKartChecker(idx)->getDriverPad(1);
    mKartLoaders[idx] = rcm->getKartLoader(idx);

    mKartBodies[idx] = body;
    mKartBodies[idx]->mKartLoader = mKartLoaders[idx];
    mKartSounds[idx] = sound;
    mKartTargets[idx] = target;
    mKartDisps[idx] = disp;

    mKartBodies[idx]->mKartSus[0] = sus[0];
    mKartBodies[idx]->mKartSus[1] = sus[1];
    mKartBodies[idx]->mKartSus[2] = sus[2];
    mKartBodies[idx]->mKartSus[3] = sus[3];

    mKartSus[idx][0] = sus[0];
    mKartSus[idx][1] = sus[1];
    mKartSus[idx][2] = sus[2];
    mKartSus[idx][3] = sus[3];

    mKartSus[idx][0]->mBody = mKartBodies[idx];
    mKartSus[idx][1]->mBody = mKartBodies[idx];
    mKartSus[idx][2]->mBody = mKartBodies[idx];
    mKartSus[idx][3]->mBody = mKartBodies[idx];

    mKartSus[idx][0]->mLoader = mKartLoaders[idx];
    mKartSus[idx][1]->mLoader = mKartLoaders[idx];
    mKartSus[idx][2]->mLoader = mKartLoaders[idx];
    mKartSus[idx][3]->mLoader = mKartLoaders[idx];

    mKartAppendix[idx][0] = appendix[0];
    mKartAppendix[idx][1] = appendix[1];

    mKartAppendix[idx][0]->mModelWheel = nullptr;
    mKartAppendix[idx][1]->mModelWheel = nullptr;
    mKartAppendix[idx][0]->mKartLoader = mKartLoaders[idx];
    mKartAppendix[idx][1]->mKartLoader = mKartLoaders[idx];
}

void KartCtrl::DoBodyHit()
{
    bool hitBody = false;
    int kartCount = GetKartCtrl()->GetKartNumber();

    for (int i = 0; i < kartCount; i++)
        getKartBody(i)->mCarStatus &= ~0x1000000000;

    for (int i = 0; i < kartCount; i++)
    {
        KartBody *body = getKartBody(i);
        for (int j = 0; j < kartCount; j++)
        {
            KartBody *enemyBody = getKartBody(j);
            body->_5b8[j] = 0;

            if (body != enemyBody)
                hitBody = body->CheckBodyHit(body, enemyBody);
        }

        if (!hitBody)
        {
            body->mCarStatus &= ~0x10;
        }
    }
}

KartCtrl *KartCtrl::makeKartCtrl()
{
    mCtrl = new KartCtrl();
    return mCtrl;
}

void KartCtrl::DynamicsInit(bool videoFlag)
{
    RaceMgr *rcm = RaceMgr::getCurrentManager();
    mBitfield = 0;
    _228 = 0;
    mKartCount = rcm->getKartNumber();

    for (int i = 0; i < 8; i++)
        mRivalKarts[i] = nullptr;

    if (IsMiniGame() == false)
        RivalKart::StartInit();

    for (int i = 0; i < GetKartNumber(); i++)
    {
        KartBody *body = new KartBody();
        KartSus *sus[4];
        for (int j = 0; j < 4; j++)
            sus[j] = new KartSus();

        KartAppendix *appendix[2];
        for (int j = 0; j < 2; j++)
            appendix[j] = new KartAppendix();

        KartSound *sound = new KartSound();
        KartTarget *target = new KartTarget();
        KartDisp *disp = new KartDisp();

        Init(body, sus, appendix, sound, target, disp, i);

        body->mKartRescue = new KartRescue();
        body->mKartCannon = new KartCannon();
        body->mKartScene = new KartScene();
        body->mKartDossin = new KartDossin();
        body->mKartTumble = new KartTumble();        
        body->mKartCheck = new KartCheck();
        body->mKartDamage = new KartDamage();
        body->mKartStar = new KartStar();
        body->mKartTire = new KartTire();
        body->mKartThunder = new KartThunder();
        body->mKartCrash = new KartCrash();
        body->mKartHandle = new KartHandle();
        body->mKartPipe = new KartPipe();
        body->mKartAnt = new KartAnt();
        body->mKartStrat = new KartStrat();
        body->mKartGame = new KartGame();
        body->mKartItem = new KartItem();
        body->mKartPerCam = new KartPerCam();

        BodyInit(i);

        if (!IsMiniGame())
        {
            mRivalKarts[i] = new RivalKart(body);
            mRivalKarts[i]->mBody = body;
            mRivalKarts[i]->mSus[0] = &sus[0][0];
            mRivalKarts[i]->mSus[1] = &sus[0][1];
            mRivalKarts[i]->mSus[2] = &sus[0][2];
            mRivalKarts[i]->mSus[3] = &sus[0][3];
        }
        SusInit(i);
        BodyPoseInit(i);

        if (!IsMiniGame())
            EnemyInit(i);

        if (!IsMiniGame())
            TargetInit(i);

        getKartDisp(i)->Init(i);

        KartPad *pad = new KartPad();
        setKartPad(pad, i);

        KartAnime *anime = new KartAnime();
        setKartAnime(anime, i);
        AnimeInit(i);
        ApeendixInit(i);
    }

    for (int i = 0; i < rcm->getCameraNumber(); i++)
    {
#line 263
        int num = rcm->getTargetKartNo(i);
        JUT_MINMAX_ASSERT(0, num, 8);
        KartCam *cam = new KartCam(mKartBodies[num], mCtrl, num, i);
        setKartCam(cam, i);
        getKartCam(i)->DoPort(i);
        getKartCam(i)->DoInit();
        getKartCam(i)->setVideo(videoFlag);
    }

    for (int i = 0; i < GetKartCtrl()->GetKartNumber(); i++)
        getKartSound(i)->Init(i);
}

void KartCtrl::DynamicsReset()
{
    RaceMgr *rcm = RaceMgr::getCurrentManager();
    mBitfield = 0;
    _228 = 0;
    mKartCount = rcm->getKartNumber();

    for (int i = 0; i < mKartCount; i++)
    {
        BodyInit(i);
        ResetSusInit(i);
        BodyPoseInit(i);

        if (IsMiniGame() == false)
            EnemyResetInit(i);

        if (IsMiniGame() == false)
            TargetInit(i);

        getKartDisp(i)->Init(i);
        PadAllClear(i);
        getKartPad(i)->Init();
        AnimeInit(i);
    }

    for (int i = 0; i < RaceMgr::getCurrentManager()->getCameraNumber(); i++)
    {
        getKartCam(i)->DoPort(i);
        getKartCam(i)->DoResetInit();
    }
}

void KartCtrl::DynamicsStage1() { 
    int i;
    int kartCount = GetKartNumber();

    if (mBitfield & 1)
        return;
    
    RivalKart::setRankAry();

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
            getKartBody(i)->getItem()->CheckHitItem();
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            getKartBody(i)->getStrat()->GetBodyRoll();
            getKartBody(i)->getStrat()->GetRoadBodyRoll();
            getKartBody(i)->getStrat()->GetBodyNorm();
            getKartBody(i)->getStrat()->GetBodySpeed();
            getKartBody(i)->getGame()->DoElementForce();
        }
        getKartBody(i)->getGame()->DoStatus();
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            DoGForce(i);
            getKartBody(i)->DegubBody(0x28);
        }            
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            getKartBody(i)->getStrat()->DoSterr();
            getKartBody(i)->DegubBody(0x29);
        }
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            for (int j = 0; j < 4; j++)
                DoSusAction(j + i * 4);

            getKartBody(i)->DegubBody(0x2a);
            for (int j = 0; j < 4; j++)
                DoTireAction(j + i * 4);

            getKartBody(i)->DegubBody(0x2b);
        }
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication()) {
            CheckVertexReflection(i);
            getKartBody(i)->DegubBody(0x2c);
        }
           
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            CheckObjectReflection(i);
            getKartBody(i)->DegubBody(0x2d);
        }
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            CheckRoofReflection(i);
            getKartBody(i)->DegubBody(0x2e);
        }
    }

    DoBodyHit();

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            getKartBody(i)->DegubBody(0x2f);
            DoPose(i);
            getKartBody(i)->DegubBody(0x30);
        }
    }

    for (i = 0; i < kartCount; i = i + 1)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            KartBody *body = getKartBody(i);
            if ((body->mGameStatus & 8))
            {
                getKartEnemy(i)->ActiveCtrl();
                getKartTarget(i)->DoSearchTarget(i);

            }
            else
            {
                getKartTarget(i)->DoSearchTarget(i);
            }
        }
    }
}

void KartCtrl::DynamicsStage2()
{
    int i; // -_-
    RaceMgr *rcm = RaceMgr::getCurrentManager();
    int kartCount = GetKartCtrl()->GetKartNumber();

    if (mBitfield & 1)
        return;

    for (i = 0; i < kartCount; i++)
    {
        if (getKartBody(i)->getChecker()->CheckIndication())
        {
            getKartBody(i)->getStrat()->DoAccel();
            DoContPaste(i);
            getKartSound(i)->DoFinishSound(i);
        }
    }

    DoLod();
    HaveRabbit();

    for (i = 0; i < rcm->getCameraNumber(); i++)
        getKartCam(i)->DoCamera();

    for (i = 0; i < kartCount; i++)
        getKartDisp(i)->MakeKartDisp();
}

void KartCtrl::DrawKartInit(int camNo)
{
    GrafPort graf(getKartCam(camNo)->GetVpPosv(), getKartCam(camNo)->GetVpPosh(), getKartCam(camNo)->GetVpWidth(), getKartCam(camNo)->GetVpHeight(), getKartCam(camNo)->GetFovy(), getKartCam(camNo)->GetNear(), getKartCam(camNo)->GetFar());

    graf.scissor(getKartCam(camNo)->GetPosv(), getKartCam(camNo)->GetPosh(), getKartCam(camNo)->GetWidth(), getKartCam(camNo)->GetHeight());

    graf.SetParam(getKartCam(camNo)->GetAspect(), getKartCam(camNo)->GetNear(), getKartCam(camNo)->GetFar());
    graf.setPort();
}

void KartCtrl::SetGrafPort()
{
    GrafPort graf(System::get3DVpX(), System::get3DVpY(), System::get3DVpW(), System::get3DVpH(), viewdata[0].fovY, getKartCam(0)->GetNear(), getKartCam(0)->GetFar());

    graf.scissor((int)System::get3DScisX(), (int)System::get3DScisY(), System::get3DScisW(), System::get3DScisH());

    graf.SetParam(getKartCam(0)->GetAspect(), getKartCam(0)->GetNear(), getKartCam(0)->GetFar());
    graf.setPort();
}

void KartCtrl::DrwaBody(int idx, u32 viewNo, Mtx44 m)
{
    SimpleDrawer drawer;
    drawer.drawInit(getKartBody(idx)->mKartLoader->getExModelBody());

    while (drawer.loadPreDrawSetting())
        getKartBody(idx)->mKartLoader->getExModelBody()->simpleDraw(viewNo, m, 1);
}

void KartCtrl::DrwaParts(int idx, u32 viewNo, Mtx44 m)
{
    KartBody *body = getKartBody(idx);
    SimpleDrawer drawer;
    drawer.drawInit(getKartSus(idx * 4 + 0)->mWheel);

    while (drawer.loadPreDrawSetting())
    {
        getKartSus(idx * 4 + 0)->mWheel->simpleDraw(viewNo, m, 1);

        if (getKartAppendix(idx * 2 + 0)->mModelWheel)
            getKartAppendix(idx * 2 + 0)->mModelWheel->simpleDraw(viewNo, m, 1);

        getKartSus(idx * 4 + 2)->mWheel->simpleDraw(viewNo, m, 1);

        if (mKartLoaders[idx]->isCommonWheelModelData())
        {
            getKartSus(idx * 4 + 1)->mWheel->simpleDraw(viewNo, m, 1);
            getKartAppendix(idx * 2 + 1)->mModelWheel->simpleDraw(viewNo, m, 1);
            getKartSus(idx * 4 + 3)->mWheel->simpleDraw(viewNo, m, 1);
        }
    }

    if (!mKartLoaders[idx]->isCommonWheelModelData())
    {
        drawer.drawInit(getKartSus(idx * 4 + 1)->mWheel);
        while (drawer.loadPreDrawSetting())
        {
            getKartSus(idx * 4 + 1)->mWheel->simpleDraw(viewNo, m, 1);

            if (getKartAppendix(idx * 2 + 0)->mModelWheel) // typo/copy paste error?
                getKartAppendix(idx * 2 + 1)->mModelWheel->simpleDraw(viewNo, m, 1);

            getKartSus(idx * 4 + 3)->mWheel->simpleDraw(viewNo, m, 1);
        }
    }

    if (body->getChecker()->CheckDraw(viewNo, idx))
    {
        drawer.drawInit(getKartSus(idx * 4 + 0)->mArm);
        while (drawer.loadPreDrawSetting())
        {
            getKartSus(idx * 4 + 0)->mArm->simpleDraw(viewNo);
            getKartSus(idx * 4 + 1)->mArm->simpleDraw(viewNo);
            getKartSus(idx * 4 + 2)->mArm->simpleDraw(viewNo);
            getKartSus(idx * 4 + 3)->mArm->simpleDraw(viewNo);
        }

        if (!mKartLoaders[idx]->isShockExist())
            return;

        drawer.drawInit(getKartSus(idx * 4 + 0)->mShock);
        while (drawer.loadPreDrawSetting())
        {
            getKartSus(idx * 4 + 0)->mShock->simpleDraw(viewNo);
            getKartSus(idx * 4 + 1)->mShock->simpleDraw(viewNo);
            getKartSus(idx * 4 + 2)->mShock->simpleDraw(viewNo);
            getKartSus(idx * 4 + 3)->mShock->simpleDraw(viewNo);
        }
    }
}
