#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameAudioCamera.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/system.h"
#include "Yamamoto/kartCamera.h"
#include "Yamamoto/kartCtrl.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

ViewData viewdata[8] = {
    {0, 0, 608.0f, 480.0f, 1.21875f, 80.0f, 1500.0f, 1.0f, 0.0f, 0.0f},
    {0, 0, 608.0f, 240.0f, 2.4375f, 60.0f, 1500.0f, 1.0f, 0.0f, 0.0f},
    {0, 240, 608.0f, 240.0f, 2.4375f, 60.0f, 1500.0f, 1.0f, 20.0f, 2.0f},
    {0, 0, 304.0f, 240.0f, 1.21875f, 80.0f, 1500.0f, 1.0f, 0.0f, 0.0f},
    {320, 0, 304.0f, 240.0f, 1.21875f, 80.0f, 1500.0f, 1.0f, 0.0f, 0.0f},
    {0, 240, 304.0f, 240.0f, 1.21875f, 80.0f, 1500.0f, 1.0f, 20.0f, 2.0f},
    {320, 240, 304.0f, 240.0f, 1.21875f, 80.0f, 1500.0f, 1.0f, 20.0f, 2.0f},
    {0x190, 0x12c, 240.0f, 180.0f, 1.28f, 60.0f, 1500.0f, 1.0f, 0.0f, 0.0f},
};

// starting camera positions? + fovY
MiniData balloonMinidata[3] = {
    {510.0f, 18.0f, -170.0f, 60.0f},
    {600.0f, 15.0f, -170.0f, 50.0f},
    {510.0f, 18.0f, -170.0f, 60.0f},
};

MiniData turtleMinidata[3] = {
    {600.0f, 9.0f, -100.0f, 45.0f},
    {600.0f, 16.0f, -150.0f, 45.0f},
    {520.0f, 15.0f, -140.0f, 50.0f},
};

MiniData bombMinidata[3] = {
    {550.0f, 13.0f, -200.0f, 60.0f},
    {550.0f, 13.0f, -150.0f, 50.0f},
    {550.0f, 13.0f, -200.0f, 60.0f},
};

MiniData devilMinidata[3] = {
    {450.0f, 18.0f, -170.0f, 65.0f},
    {550.0f, 15.0f, -150.0f, 50.0f},
    {450.0f, 18.0f, -170.0f, 65.0f},
};

MiniData balloonPakkunMinidata[3] = {
    {550.0f, 20.0f, -250.0f, 65.0f},
    {600.0f, 18.0f, -230.0f, 60.0f},
    {550.0f, 20.0f, -250.0f, 65.0f},
};

MiniData bombPakkunMinidata[3] = {
    {550.0f, 18.0f, -245.0f, 65.0f},
    {680.0f, 16.0f, -230.0f, 60.0f},
    {550.0f, 18.0f, -240.0f, 65.0f},
};

MiniData devilPakkunMinidata[3] = {
    {500.0f, 20.0f, -250.0f, 70.0f},
    {560.0f, 18.0f, -230.0f, 60.0f},
    {500.0f, 20.0f, -250.0f, 70.0f},
};

KartCam::KartCam(KartBody * body, KartCtrl *ctrl, u8 targetIdx, u8 p4)
{
    mTargetBody = body;
    mCtrl = ctrl;
    mTargetIdx = targetIdx;
    mCamNo = p4;
}

void KartCam::DegubCamera(char *name, u32 id) {}

void KartCam::SetStartDemoPort()
{
    mWidth = System::get3DScisW();
    mHeight = System::get3DScisH();
    mPosV = System::get3DScisX();
    mPosH = System::get3DScisY();
    mVpWidth = System::get3DVpW();
    mVpHeight = System::get3DVpH();

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpX();
    mVpPosH = System::get3DVpY();
    mAspect = System::getPerspAspect();
}

void KartCam::SetAwardDemoPort()
{
    mWidth = System::get3DAwardScisW();
    mHeight = System::get3DAwardScisH();
    mPosV = System::get3DAwardScisX();
    mPosH = System::get3DAwardScisY();
    mVpWidth = System::get3DVpW();
    mVpHeight = System::get3DVpH();

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpX();
    mVpPosH = System::get3DVpY();
    mAspect = System::getPerspAspect();
}

void KartCam::SetOneScreenPort()
{
    mWidth = System::get3DScisW();
    mHeight = System::get3DScisH();
    mPosV = System::get3DScisX();
    mPosH = System::get3DScisY();
    mVpWidth = System::get3DVpW();
    mVpHeight = System::get3DVpH();

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpX();
    mVpPosH = System::get3DVpY();
    mAspect = System::getPerspAspect();
}

void KartCam::SetSubScreenPort()
{
    mWidth = System::get3DScisSubW();
    mHeight = System::get3DScisSubH();
    mPosV = System::get3DScisSubX();
    mPosH = System::get3DScisSubY();
    mVpWidth = System::get3DVpSubW();
    mVpHeight = System::get3DVpSubW(); // Typo?

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpSubX();
    mVpPosH = System::get3DVpSubY();
    mAspect = System::getPerspAspectSub();
}

void KartCam::SetTwoScreenPort(u8 pos)
{
    mWidth = System::get3DScisDiv2W(pos);
    mHeight = System::get3DScisDiv2H(pos);
    mPosV = System::get3DScisDiv2X(pos);
    mPosH = System::get3DScisDiv2Y(pos);
    mVpWidth = System::get3DVpDiv2W(pos);
    mVpHeight = System::get3DVpDiv2H(pos);

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpDiv2X(pos);
    mVpPosH = System::get3DVpDiv2Y(pos);
    mAspect = System::getPerspAspectDiv2();
}

void KartCam::SetFourScreenPort(u8 pos)
{
    mWidth = System::get3DScisDiv4W(pos);
    mHeight = System::get3DScisDiv4H(pos);
    mPosV = System::get3DScisDiv4X(pos);
    mPosH = System::get3DScisDiv4Y(pos);
    mVpWidth = System::get3DVpDiv4W(pos);
    mVpHeight = System::get3DVpDiv4H(pos);

    _110 = ((int)GetVpHeight() - (int)GetHeight()) / 2;

    mVpPosV = System::get3DVpDiv4X(pos);
    mVpPosH = System::get3DVpDiv4Y(pos);
    mAspect = System::getPerspAspectDiv4();
}

void KartCam::DoPort(int camNo)
{

    if (RaceMgr::getCurrentManager()->isCrsDemoMode())
    {
        mPtr = 0;
        SetStartDemoPort();
    }
    else if (RaceMgr::getCurrentManager()->isAwardDemoMode())
    {
        mPtr = 0;
        SetAwardDemoPort();
    }
    else
    {
        switch (RaceMgr::getCurrentManager()->getCameraNumber())
        {
        case 1:
        {
            mPtr = 0;
            SetOneScreenPort();
            break;
        }
        case 2:
        {
            if (RaceMgr::getCurrentManager()->isSubScrExist())
            {
                switch (camNo)
                {
                case 0:
                    mPtr = 0;
                    SetOneScreenPort();
                    break;
                case 1:
                    mPtr = 7;
                    SetSubScreenPort();
                    break;
                }
            }
            else
            {
                switch (camNo)
                {
                case 0:
                    mPtr = 1;
                    SetTwoScreenPort(0);
                    break;
                case 1:
                    mPtr = 2;
                    SetTwoScreenPort(1);
                    break;
                }
            }
            break;
        }
        case 3:
        case 4:
        {
            switch (camNo)
            {
            case 0:
                mPtr = 3;
                SetFourScreenPort(0);
                break;
            case 1:
                mPtr = 4;
                SetFourScreenPort(1);
                break;
            case 2:
                mPtr = 5;
                SetFourScreenPort(2);
                break;
            case 3:
                mPtr = 6;
                SetFourScreenPort(3);
                break;
            }
            break;
        }
        default:
        {
            mPtr = 0;
            SetOneScreenPort();
            break;
        }
        }
    }

#line 340
    JUT_MINMAX_ASSERT(0, mPtr, 9);

    SetFovyData();
    mLodLen = viewdata[mPtr]._18;
    mClipperScale = viewdata[mPtr]._1c;
    _17c = viewdata[mPtr]._20;
    _180 = viewdata[mPtr]._24;
    SetClipper();

    _130 = System::get2DScisW();
    _134 = System::get2DScisH();
    _fc = System::get2DScisX();
    _100 = System::get2DScisY();

    _138 = System::get2DVpW();
    _13c = System::get2DVpH();
    _10c = 0;
    _104 = System::get2DVpX();
    _108 = System::get2DVpY();

    mStartX = System::getOrthoL();
    mEndX = System::getOrthoR();
    mStartY = System::getOrthoT();
    mEndY = System::getOrthoB();
}

void KartCam::Init(bool last)
{
    mMode = 0;
    _e2 = 0;
    _158 = 0.0f;
    mNear = 10.0f;
    mFar = 200000.0f;

    mCameraPos.zero();
    _200.zero();
    _20c.zero();
    _1dc.zero();
    _254.zero();
    _260.zero();

    _164 = 0.0f;
    mFlags = 0;
    _ea = 0;
    mCurCamIdx = -1;

    _184 = 0.0f;
    _1a4 = 0.0f;
    _1a8 = 0.0f;
    _188 = 0.0f;
    _190 = 0.0f;
    _194 = 0.0f;
    _1a0 = 0.0f;
    _1ac = 0.0f;
    _1b8 = 0.0f;
    _1c4 = 0.0f;
    _1c8 = 0.0f;
    _18c = 0.0f;
    _14c = 0.0f;
    _1c0 = 0.0f;
    _e6 = 0;
    _1c4 = 0.0f;
    _1c8 = 0.0f;
    _1a0 = 0.0f;
    _1cc = 0.0f;
    _1d0 = 0.0f;

    RaceMgr *rcm = rcm->getCurrentManager();
    SetTarget();

    if (rcm->getCurrentManager()->isAwardDemoMode())
    {
        int awardKartNo = rcm->getCurrentManager()->getAwardKartNo();
        if (awardKartNo < 0)
        {
            awardKartNo = 0;
        }
        SetTargetNum(awardKartNo);
        SetTarget();
        mTargetBody = getBody(); // ?
    }
    if (mTargetBody->mCameraNum == 100)
    {
        mTargetBody->mCameraNum = mCamNo;
    }
    mDemoCam->Init(this);
    if (mPtr == 7)
    {
        SetCameraMode(8);
    }
    else if (rcm->isStaffRoll())
    {
        SetCameraMode(15);
        GetDemoCam()->SetUpStaffID();
        if (mCamNo == 0)
        {
            mCtrl->mBitfield |=  2;
        }
    }
    else if (rcm->isWaitDemoMode() && (rcm->getCameraNumber() >= 2))
    {
        SetCameraMode(0);
        if (mCamNo == 0)
        {
            mCtrl->mBitfield |=  2;
        }
    }
    else if (rcm->isWaitDemoMode() || rcm->isAwardDemoMode() || (mTargetBody->mGameStatus & 0x80))
    {
        SetCameraMode(2);
        if (mCamNo == 0)
        {
            mCtrl->mBitfield |=  2;
        }
    }
    else if (((rcm->isDemoCamera(mCamNo) != 0) && (last)) ||
             (((mTargetBody->mGameStatus & 8) != 0 && (last))))
    {
        SetCameraMode(2);
        if (mCamNo == 0)
        {
            mCtrl->mBitfield |=  2;
        }
        if (rcm->isDemoCamera(mCamNo))
        {
            if (GetKartCtrl()->IsMiniGame())
            {
                SetCameraMode(14);
                GetDemoCam()->SetUpMapID();
            }
            else if (rcm->getCurrentManager()->isLANMode())
            {
                mFlags |= 0x1000;
            }
            else
            {
                mFlags |= 0x100;
            }
        }
    }
    else
    {
        mMode = 10;
    }

    if (mPtr != 7 && !last && mDemoCam->GetStartID() == 0)
    {
        if (mCamNo == 0)
        {
            mCtrl->mBitfield |= 2;
        }
        mMode = 0;
    }
    if (mPtr != 7 && !rcm->isDemoCamera(mCamNo))
    {
        mTargetBody->mGameStatus |= 0x1000;
    }

    SetPtr();
    switch (mMode)
    {
    case 0:
    case 1:
        InitOutView();
        break;
    case 8:
        InitBackView();
        break;
    case 2:
    case 12:
        mArea.searchCamera(mTargetBody->mPos);
        GetDemoCam()->InitID();
        break;
    case 10:
        GetDemoCam()->InitStartID(last);
        break;
    case 14:
        GetDemoCam()->InitStartMapCam();
        break;
    case 15:
        GetDemoCam()->InitStaffView();
    }

    mCourseID = rcm->getCourse()->getCourseID();
    SetClipper();
}

void KartCam::DoInit()
{
    mPathSpline = new Spline();
    mDemoCam = new KartDemoCam();
    Init(false);
}

void KartCam::DoResetInit() { Init(true); }

void KartCam::DoRaceCtrl()
{
    RaceMgr *rcm = rcm->getCurrentManager();
    if (mPtr == 7)
    {
        SetCameraMode(8);
    }
    else if (mDemoCam->_38 & 1)
    {
        mMode = 10;
    }
    else if (mDemoCam->_38 & 0x100)
    {
        mMode = 14;
    }
    else if (rcm->isStaffRoll())
    {
        mMode = 15;
    }
    else if (mFlags & 0x100)
    {
        SetCameraMode(2);
    }
    else if (mFlags & 0x1000)
    {
        SetCameraMode(2);
    }
    else if (!rcm->isWaitDemoMode() && !(mFlags & 0x800) && (rcm->isDemoCamera(mCamNo) || rcm->isAwardDemoMode() || mTargetBody->mGameStatus & (0x80 | 0x8)))
    {
        SetCameraMode(2);
    }
    else if (mFlags & 0x10)
    {
        mMode = 4;
    }
    else if (mFlags & 8)
    {
        mMode = 3;
    }
    else if (mFlags & 0x40)
    {
        mMode = 6;
    }
    else if (mFlags & 0x80)
    {
        mMode = 7;
    }
    else if (mFlags & 0x400)
    {
        mMode = 13;
    }
    else if (mFlags & 0x200)
    {
        mMode = 2;
    }
    else if (rcm->isWaitDemoMode() && rcm->getCameraNumber() >= 2)
    {
        mMode = 0;
    }
    else if (rcm->isWaitDemoMode())
    {
        SetCameraMode(2);
    }
    else
    {
        mMode = 0;
    }

    DoCameraMode();
}

void KartCam::DoCameraMode()
{
    switch (mMode)
    {
    case 0:
        OutView();
        break;
    case 1:
        ParallelView();
        break;
    case 2:
        GetDemoCam()->DemoView();
        break;
    case 10:
        GetDemoCam()->StartDemoView();
        break;
    case 15:
        GetDemoCam()->StaffView();
        break;
    case 3:
        HangRescueView();
        break;
    case 4:
        DropRescueView();
        break;
    case 6:
        LaunchView();
        break;
    case 7:
        LandView();
        break;
    case 13:
        PipeView();
        break;
    case 8:
        BackView();
        break;
    case 9:
        GetDemoCam()->MapView();
        break;
    case 11:
        GetDemoCam()->ObjectView();
        break;
    case 14:
        GetDemoCam()->StartMapCam();
        break;
    }
}

void KartCam::DoCamera()
{
    if (GetKartCtrl()->mBitfield & 1)
        return;

    SetTarget();

    switch (_e2)
    {
    case 0:
        DoRaceCtrl();
        break;
    case 1:
        DoCameraMode();
        break;
    }

    CheckClipper();

    GameAudio::CameraMgr *camMgr = GetGameAudioMain()->getCamera();

    camMgr->setCameraInfo(mCamNo, *(Vec *)&mCameraPos, mMtx, &mArea);
}
