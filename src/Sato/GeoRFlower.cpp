#include "Sato/Objects/GeoRFlower.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
#include "JSystem/J3D/J3DModel.h"
#include "Sato/AnmController.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoRFlower::GeoRFlower(const CrsData::SObject &obj) : GeographyObj(obj) {
    // UNUSED
}

void GeoRFlower::reset() {
    // UNUSED
}

void GeoRFlower::createColModel(J3DModelData *) {
    // UNUSED
}

const char *GeoRFlower::getBmdFileName() {
    // UNUSED
    //static const char *cBmdName = "/Objects/RFlower.bmd";
    //return cBmdName;
}

void GeoRFlower::calc() {}

J3DAnmTransform *GeoMarioFlower1::sMarioFlower1BckAnmTrans;
J3DMtxCalc *GeoMarioFlower1::sMarioFlower1BckMtxCalc;

GeoMarioFlower1::GeoMarioFlower1(const CrsData::SObject &obj) : GeographyObj(obj) {
    if (!mAnmCtrl) {
        mAnmCtrl = new AnmController();
    }
}

void GeoMarioFlower1::reset() {
    GeographyObj::resetObject();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();
    _58 = 0;
    J3DFrameCtrl *ctrl = getAnmCtrl()->getFrameCtrl(0);
    ctrl->setAttribute(2);
    ctrl->setRate(((getGeoRnd()->get_ufloat_1() * 2 - 1) * 0.3f + 1.0f));
}

void GeoMarioFlower1::loadAnimation() {
    J3DModelData *modelData =  mModel.getModelData();
    void *bck = ObjUtility::getPtrCourseArc("/Objects/MarioFlower1.bck");
    J3DAnmObjTrans::setupTransAnmData(&sMarioFlower1BckAnmTrans, &sMarioFlower1BckMtxCalc, modelData, bck);
}

void GeoMarioFlower1::createColModel(J3DModelData *) {}

void GeoMarioFlower1::createModel(JKRSolidHeap *heap, u32 viewNo) {
    mModel.createModel(heap, viewNo, 0);
    getAnmCtrl()->InitRegistration(1, &mModel);
    getAnmCtrl()->RegisterTrans(0, sMarioFlower1BckAnmTrans, sMarioFlower1BckMtxCalc);
}

const char *GeoMarioFlower1::getBmdFileName() {
    static const char *cBmdName = "/Objects/MarioFlower1.bmd";
    return cBmdName;
}

void GeoMarioFlower1::calc() {}
