#include "Kaneshige/Objects/GeoWater.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/J3DAnmObject.h"
#include "Sato/ObjUtility.h"

J3DAnmTextureSRTKey *GeoWaterUp2::sBtkAnm;

GeoSubWater::GeoSubWater(u32 id) : GeographyObj(id) {
    GeographyObj::setObjFlagSimpleDraw();
    GeographyObj::clrObjFlagLODBias();
    reset();
}

void GeoSubWater::reset() {
    GeographyObj::resetObject();
    GeographyObj::clrObjFlagCheckGeoHitting();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();
}

void GeoSubWater::createModel(JKRSolidHeap *, u32) {}

void GeoSubWater::setPosition(JGeometry::TVec3f &pos) {
    mPos.set(pos);
}

GeoWaterMiddleD::GeoWaterMiddleD() : GeoSubWater(0xda3) {}

void GeoWaterMiddleD::reset() {
    GeoSubWater::reset();
}

void GeoWaterMiddleD::loadAnimation() { // does this return J3DModelData *?
    mModel.getModelData();
}

void GeoWaterMiddleD::createModel(JKRSolidHeap *, u32) {

}

void GeoWaterMiddleD::simpleDraw(u32 viewNo) {
    GeographyObj::simpleDraw(viewNo);
}

void GeoWaterMiddleD::calc() {}

GeoWaterDown::GeoWaterDown() : GeoSubWater(0xda2) {}

void GeoWaterDown::simpleDraw(u32 viewNo) {
    GeographyObj::simpleDraw(viewNo);
}

GeoWaterUp::GeoWaterUp()  : GeoSubWater(0xda4) {}

void GeoWaterUp::simpleDraw(u32 viewNo) {
    GeographyObj::simpleDraw(viewNo);
}

GeoWaterUp2::GeoWaterUp2() : GeoSubWater(0xda5) {
}

void GeoWaterUp2::reset() {
    GeoSubWater::reset();
    mMat.resetFrame();
}

void GeoWaterUp2::loadAnimation() {
    void *btk = ObjUtility::getPtrCourseArc("/Objects/sea2_tex.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sBtkAnm, mModel.getModelData(), btk);
}

void GeoWaterUp2::createModel(JKRSolidHeap *slid, u32) {
    mMat.setExModel(&mModel);
    mMat.attach(sBtkAnm);
}

void GeoWaterUp2::simpleDraw(u32 viewNo) {
    mMat.anmFrameProc();
    GeographyObj::simpleDraw(viewNo);
}

void GeoWaterUp2::calc() {
    mMat.frameProc();
}
