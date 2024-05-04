#include "Kaneshige/RaceMgr.h"

#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/KartPerCam.h"
#include "Yamamoto/kartCamera.h"

#include <std/math.h>

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartPerCam::Init(int idx)
{
    mBody = GetKartCtrl()->getKartBody(idx);
    _4c = 0;
    _4f = 0;
    mCamId = -1;
    mArea.searchCamera(mBody->mPos);
    mCamId = mArea.getCameraID();
    InitID();
}

void KartPerCam::ChaseFovy()
{
    // UNUSED, Fabricated(Size mismatch)
    // Do not remove for matching build

    f32 FOVArray[4];
    KartCam *cam = GetKartCtrl()->getKartCam(mCamId);
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);

    FOVArray[0] = cam->GetFovy();
    f32 FOV = camData->getFovy2();
    if (FOV == 0.0f)
    {
        FOV = camData->getFovy();
        cam->SetFovy(camData->getFovy());
    }
    else
    {
        GetKartCtrl()->ChaseFnumber(FOVArray,FOV, 0.01);
        cam->SetFovy(FOVArray[0]);
    }
}

void KartPerCam::InitFixCam()
{
    JGeometry::TVec3f upVec, v2;

    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    upVec.set(0.0f, 1.0f, 0.0f);

    camData->getPosition(&_94);
    mPos.set(_94);
    JGeometry::TVec3f frontDirection;
    camData->getFrontDirection(&frontDirection);

    mPos.x += 100.0f * frontDirection.x;
    mPos.y += 100.0f * frontDirection.y;
    mPos.z += 100.0f * frontDirection.z;
    mFovY = camData->getFovy();
}

void KartPerCam::InitFixSearchCam()
{
    JGeometry::TVec3f upVec, v2;

    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    upVec.set(0.0f, 1.0f, 0.0f);
    camData->getPosition(&_94);
    mPos.set(mBody->mPlayerPosMtx[0][3], mBody->mPlayerPosMtx[1][3], mBody->mPlayerPosMtx[2][3]);
    mFovY = camData->getFovy();
}

void KartPerCam::DoFixSearchCam()
{
 

}

void KartPerCam::InitFixChaseCam()
{
    JGeometry::TVec3f upVec, v2, v3;

    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);

    upVec.set(0.0f, 1.0f, 0.0f);
    camData->getPosition(&_94);
    v3.set(mBody->mPlayerPosMtx[0][3], mBody->mPlayerPosMtx[1][3], mBody->mPlayerPosMtx[2][3]);

    _b8.sub(_94, v3);
    _88 = _b8.length();
    _b8.normalize();

    _94.x = _b8.x * _88 + v3.x;
    _94.y = _b8.y * _88 + v3.y;
    _94.z = _b8.z * _88 + v3.z;

    mPos.set(_94);

    JGeometry::TVec3f frontDirection;
    camData->getFrontDirection(&frontDirection);

    mPos.x += 100.0f * frontDirection.x;
    mPos.y += 100.0f * frontDirection.y;
    mPos.z += 100.0f * frontDirection.z;
    mFovY = camData->getFovy();
}

void KartPerCam::InitChaseCam()
{
    JGeometry::TVec3f upVec;

    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    upVec.set(0.0f, 1.0f, 0.0f);
    mPos.set(mBody->mPlayerPosMtx[0][3], 30.0f + mBody->mPlayerPosMtx[1][3], mBody->mPlayerPosMtx[2][3]);
    camData->getPosition(&_94);
    _b8.sub(_94, mPos);
    _88 = _b8.length();

    _b8.normalize();

    _94.x = _b8.x * _88 + mPos.x;
    _94.y = _b8.y * _88 + mPos.y;
    _94.z = _b8.z * _88 + mPos.z;

    mFovY = camData->getFovy();
}

void KartPerCam::InitFixPathCam()
{
    JGeometry::TVec3f upVec, v2;

    CrsData *crsData = RaceMgr::getManager()->getCourse()->getCrsData();
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);

    CrsData::PathData *pathData = crsData->getPathData(camData->getPathID());
    CrsData::PointData *pointData = crsData->getPointData(camData->getPathID(), 0);

    _94.set(pointData->pos);
    upVec.set(0.0f, 1.0f, 0.0f);

    JGeometry::TVec3f frontDirection;
    camData->getFrontDirection(&frontDirection);
    f32 yaw = std::atan2f(frontDirection.x, frontDirection.z);

    GetKartCtrl()->PolarToCross((Vec *)&_94, (Vec *)&mPos, 50.0f, 0.0f, yaw);
    mFovY = camData->getFovy();
}

void KartPerCam::InitChasePathCam() {
    JGeometry::TVec3f upVec;

    CrsData *crsData = RaceMgr::getManager()->getCourse()->getCrsData();
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);

    CrsData::PathData *pathData = crsData->getPathData(camData->getPathID());
    CrsData::PointData *pointData = crsData->getPointData(camData->getPathID(), 0);

    _94.set(pointData->pos);
    upVec.set(0.0f, 1.0f, 0.0f);
    mPos.set(mBody->mPlayerPosMtx[0][3], 30.0f + mBody->mPlayerPosMtx[1][3], mBody->mPlayerPosMtx[2][3]);

    mFovY = camData->getFovy();
}

void KartPerCam::InitFixSplCam()
{
    JGeometry::TVec3f upVec, v2;

    CrsData *crsData = RaceMgr::getManager()->getCourse()->getCrsData();
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    _88 = 0.0f;
    _54 = 1;
    CrsData::PathData *pathData = crsData->getPathData(camData->getPathID());
    _8c = 0.0f;

    for (int i = 0; i < 4; i++)
    {
        CrsData::PointData *pointData = crsData->getPointData(camData->getPathID(), i);
        _d0[i].set(pointData->pos);
    }

    _34->setAll(_d0);
    _94.set(_d0[0]);
    _8c = 0.0f;

    upVec.set(0.0f, 1.0f, 0.0f);

    JGeometry::TVec3f frontDirection;
    camData->getFrontDirection(&frontDirection);
    f32 yaw = std::atan2f(frontDirection.x, frontDirection.z);

    GetKartCtrl()->PolarToCross((Vec *)&_94, (Vec *)&mPos, 50.0f, 0.0f, yaw);
    mFovY = camData->getFovy();
}

void KartPerCam::InitChaseSplCam()
{
    JGeometry::TVec3f upVec;

    CrsData *crsData = RaceMgr::getManager()->getCourse()->getCrsData();
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    _88 = 0.0f;
    _54 = 1;
    CrsData::PathData *pathData = crsData->getPathData(camData->getPathID());

    _8c = 0.0f;

    for(int i = 0; i<4; i++) {
        CrsData::PointData *pointData = crsData->getPointData(camData->getPathID(), i);
        _d0[i].set(pointData->pos);
    }

    _34->setAll(_d0);
    _94.set(_d0[0]);
    _8c = 0.0f;
    _50 = 0;
    upVec.set(0.0f, 1.0f, 0.0f);


    mPos.set(mBody->mPlayerPosMtx[0][3], 30.0f + mBody->mPlayerPosMtx[1][3], mBody->mPlayerPosMtx[2][3]);

    mFovY = camData->getFovy();
}

void KartPerCam::InitFixPalaCam() {
    JGeometry::TVec3f upVec, v2, v3, targetPos;
    
    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);
    if ((mBody->mCarStatus & 0x100000))
    {
        mPalaCamMtx[0][0] = 1.0f;
        mPalaCamMtx[1][0] = 0.0f;
        mPalaCamMtx[2][0] = 0.0f;

        mPalaCamMtx[0][1] = 0.0f;
        mPalaCamMtx[1][1] = 1.0f;
        mPalaCamMtx[2][1] = 0.0f;

        mPalaCamMtx[0][2] = 0.0f;
        mPalaCamMtx[1][2] = 0.0f;
        mPalaCamMtx[2][2] = 1.0f;

        mPalaCamMtx[0][3] = mBody->_110[0][3];
        mPalaCamMtx[1][3] = mBody->_110[1][3];
        mPalaCamMtx[2][3] = mBody->_110[2][3];
    }
    else
    {
        PSMTXCopy(mBody->_110, mPalaCamMtx);
    }
    upVec.set(0.0f, 1.0f, 0.0f);
    mFovY = camData->getFovy();
    camData->getTargetPos(&targetPos);
    mPos.set(mBody->mPos);
    mPos.y += 30.0f;
    
    if (mBody->mIdx == 0x10)
    {
        mPos.y += 40.0f;
    }
    else if (mBody->mIdx == 0x11)
    {
        mPos.y += 40.0f;
    }
    else if (mBody->mIdx == 0xd)
    {
        mPos.y += 40.0f;
    }
    else if (mBody->mIdx == 5)
    {
        mPos.y += 40.0f;
    }

    Mtx x_m;
    Mtx y_m;    

    GetKartCtrl()->RotYMatrix(x_m, (3.141f * targetPos.x) / 180.0f);
    GetKartCtrl()->RotXMatrix(y_m, (3.141f * targetPos.y) / 180.0f);
    PSMTXConcat(x_m, y_m, x_m);
    PSMTXConcat(mBody->_110, x_m, x_m);

    _94.x = x_m[0][2] * targetPos.z + mPos.x;
    _94.y = x_m[1][2] * targetPos.z + mPos.y;
    _94.z = x_m[2][2] * targetPos.z + mPos.z;

    camData->getTargetPos2(&targetPos);
    GetKartCtrl()->RotYMatrix(x_m, (3.141f * targetPos.x) / 180.0f);
    GetKartCtrl()->RotXMatrix(y_m, (3.141f * targetPos.y) / 180.0f);
    PSMTXConcat(x_m, y_m, x_m);
    PSMTXConcat(mBody->_110, x_m, x_m);

    targetPos.z = 100.0f;
    mPos.x = x_m[0][2] * targetPos.z + _94.x;
    mPos.y = x_m[1][2] * targetPos.z + _94.y;
    mPos.z = x_m[2][2] * targetPos.z + _94.z;
    return;
}

void KartPerCam::InitDemoPathCam()
{
    JGeometry::TVec3f upVec, v2;
    CrsData *crsData = RaceMgr::getManager()->getCourse()->getCrsData();
    CrsData::Camera *camData = crsData->getCameraData(mCamId);

    _52 = 0;
    CrsData::PathData *pathData = crsData->getPathData(camData->getPathID());
    CrsData::PointData *pointData = crsData->getPointData(camData->getPathID(), 0);
    _94.set(pointData->pos);
    upVec.set(0.0f, 1.0f, 0.0f);

    mFovY = camData->getFovy();
    camData->getTargetPos(&mPos);
}

void KartPerCam::InitDemoParallelView() 
{
    JGeometry::TVec3f v1, v2;

    KartBody *body = mBody;
    v1.set(body->mPlayerPosMtx[0][3], body->mPlayerPosMtx[1][3], body->mPlayerPosMtx[2][3]);
    mPos.sub(v1, mBody->_350);
    GetKartCtrl()->PolarToCross((Vec *)&mPos, (Vec *)&_94, 500.0f, 0.348888f, 0.0f);
}

void KartPerCam::InitID()
{
    mCamId = mArea.getCameraID();

    CrsData::Camera *camData = RaceMgr::getManager()->getCourse()->getCrsData()->getCameraData(mCamId);

    if (mCamId < 0)
    {
        InitDemoParallelView();
    }
    else if (camData->getKind() == 0)
    {
        InitFixCam();
    }
    else if(camData->getKind() == 0x100)
    {
        InitFixSearchCam();
    }
    else if (camData->getKind() == 1)
    {
        InitFixPathCam();
    }
    else if (camData->getKind() == 0x101)
    {
        InitChasePathCam();
    }
    else if (camData->getKind() == 2)
    {
        InitFixChaseCam();
    }
    else if (camData->getKind() == 0x102)
    {
        InitChaseCam();
    }
    else if (camData->getKind() == 7)
    {
        InitFixPalaCam();
    }
    else if (camData->getKind() == 3)
    {
        InitFixSplCam();
    }
    else if (camData->getKind() == 0x103)
    {
        InitChaseSplCam();
    }
    else if (camData->getKind() == 5)
    {
        InitDemoPathCam();
    }
}
