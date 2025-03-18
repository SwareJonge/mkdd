#include "Kaneshige/Objects/GeoDkCloud.h"
#include "JSystem/J3D/J3DModel.h"
#include "Kaneshige/Course/CrsData.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

J3DAnmTextureSRTKey *GeoDkCloud::sBtkAnm; // 0x80416498


GeoDkCloud::GeoDkCloud(const CrsData::SObject &obj) : GeographyObj(obj) {
    reset();
}

void GeoDkCloud::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrObjFlagCheckGeoHitting();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::setObjFlagSimpleDraw();
    mLfDir.set(mObjData->zNorm);
    mLfDir.scale(0.0001f);
    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);
    mFrDir.cross(mUpDir, mLfDir);
    mFrDir.normalize();
    mAnmObj.resetFrame();
}


void GeoDkCloud::loadAnimation() {
    J3DModelData *data = mModel.getModelData();
    void *obj = ObjUtility::getPtrCourseArc("/Objects/DonkyCloud.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sBtkAnm, data, obj);
}


void GeoDkCloud::createModel(JKRSolidHeap *slid, u32) {
    mAnmObj.setExModel(&mModel);
    mAnmObj.attach(sBtkAnm);
}


void GeoDkCloud::simpleDraw(u32 viewNo) {
    mAnmObj.anmFrameProc();
    GeographyObj::simpleDraw(viewNo);
}

void GeoDkCloud::calc() {
    mAnmObj.getFrameCtrl()->setRate(1.0f);
    mAnmObj.frameProc();
    mRotMtx.setXYZDir(mFrDir, mUpDir, mLfDir);
}

void GeoDkCloud::update() {
    GeographyObj::update();
}
