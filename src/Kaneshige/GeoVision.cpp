#include "Kaneshige/Objects/GeoVision.h"
#include "JSystem/J3D/J3DMaterial.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/J3D/J3DTexMtx.h"
#include "JSystem/J3D/J3DTexture.h"

#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/ResTIMG.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/StateObserver.h"
#include "dolphin/gx/GXEnum.h"
#include "macros.h"
#include "types.h"
#include "string.h"

const char *cChangeMaterialName = "wl_screen1_v";

const int GeoVisionSupervisor::cCaptureWidth = 200;
const int GeoVisionSupervisor::cCaptureHeight = 120;

StateObserver::StateFuncSet<GeoVision> GeoVision::sTable[1] = {
    {0, &GeoVision::initFuncWait, &GeoVision::doFuncWait}
};

u16 GeoVision::sSupervisorCreateNum;
GeoVisionSupervisor *GeoVision::sSupervisor;

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoVisionSupervisor::GeoVisionSupervisor() {
    mList.initiate();
    mCaptureTex = nullptr;
    mCaptureTex = new JUTTexture(cCaptureWidth, cCaptureHeight, GX_TF_RGB565);
}

GeoVisionSupervisor::~GeoVisionSupervisor() {
    GeoVision::sSupervisor = nullptr;
}

void GeoVisionSupervisor::reset() {}

void GeoVisionSupervisor::calc() {}

void GeoVisionSupervisor::entry(GeoVision *vision) {
    mList.append(&vision->mLink);
}

void GeoVisionSupervisor::capture() {
    
    if (!isCaptureEnable()) {
        return;
    }
    int x, y;
    KartCam *camera = RaceMgr::getManager()->getCamera(0);

    x = ALIGN_PREV(camera->GetPosv() + 1 + (((int)camera->GetWidth() - cCaptureWidth) / 2), 2);
    y = ALIGN_PREV(camera->GetPosh() + 1 + (((int)camera->GetHeight() - cCaptureHeight) / 2), 2);

    mCaptureTex->capture(x, y);
}

GeoVision::GeoVision(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {

    if (sSupervisorCreateNum == 0) {
        sSupervisor = new GeoVisionSupervisor();
        sSupervisorCreateNum++;
    }

    memset(&mImg, 0, sizeof(ResTIMG));
    reset();
    sSupervisor->entry(this);
}

void GeoVision::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();
    StateObserver::ResetState();
    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);

    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
}

void GeoVision::createModel(JKRSolidHeap *slid, u32 viewNo) {
    J3DModelData *mdlData = mModel.getModelData();
    J3DTexture *tex = mdlData->getTexture();
    mImg = *tex->getResTIMG(0);
    
    tex->setResTIMG(0, *getGeoVisionSupervisor()->getCaptureTexture()->getTexInfo());

    int scrIdx = mdlData->getMaterialName()->getIndex(cChangeMaterialName);
#line 282
    JUT_ASSERT_MSG(scrIdx >= 0, "NOT FOUNT SCR MAT");

    if (RaceMgr::getManager()->isMirror() && scrIdx >= 0) {
        J3DMaterial *node = mdlData->getMaterialNodePointer(scrIdx);
        J3DTexCoord *coord = node->getTexGenBlock()->getTexCoord(0);
        coord->setTexGenMtx(30);
        coord->setTexMtxReg(30);
        J3DTexMtxInfo &mtxInfo = node->getTexMtx(0)->getTexMtxInfo();
        mtxInfo.mSRT.mScaleX = -1.0f;
    }

    mModel.createDifferedModel(slid, viewNo, 0x4021200, false);
}

void GeoVision::calc() {
    StateObserver::ExecuteState();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
}

void GeoVision::InitExec() {
    Observer_FindAndInit(GeoVision, 1);
}

void GeoVision::MoveExec() {
    Observer_FindAndExec(GeoVision, 1);
}

void GeoVision::initFuncWait() {}

void GeoVision::doFuncWait() {}
