#include "Sato/Objects/GeoTree.h"
#include "JSystem/JGeometry/Vec.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Shiraiwa/SiUtil.h"
#include "Yamamoto/kartCtrl.h"

GeoTree::GeoTree(const CrsData::SObject &object) : GeographyObj(object) {}

void GeoTree::reset() {
    GeographyObj::resetObject();
    _58 = 1;
}

void GeoTree::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 110.0f, 500.0f);
}

const char *GeoTree::getBmdFileName() {
    static const char *cTreeBmdName = "ki_00.bmd";
    return cTreeBmdName;
}

void GeoTree::calc() {}

bool GeoTree::availBckAnm() {
    return SiUtil::getConsoleNum() == 1;
}

GeoAnmTableEntry GeoMarioTree1::sBckAnmTable[1] = {
    0, "MarioTree1_wait.bck"
};

GeoMarioTree1::GeoMarioTree1(const CrsData::SObject &object) : GeoTree(object) {
    mObjFlag |= 0x20; // TODO: make up inline
}

void GeoMarioTree1::reset() {
    GeographyObj::resetObject();
    _58 = 1;
    if (getAnmCtrl()) {
        J3DFrameCtrl *ctrl = getAnmCtrl()->getFrameCtrl(0);
        ctrl->setAttribute(2);
        ctrl->setRate(((getGeoRnd()->get_ufloat_1() * 2 - 1) * 0.3f + 1.0f));
    }

}

void GeoMarioTree1::createColModel(J3DModelData *mdlData) {
    createBoundsCylinder(mdlData, 210.0f, 600.0f);
    
    // and we stumble on another nonmatch with this set of inlines...
    // screw this code anyway, it doesn't do anything(i think)
    JGeometry::TVec3f dir;
    //JGeometry::TVec3f dir(mRotMtx[0][2], mRotMtx[1][2], mRotMtx[2][2]);
    mRotMtx.getZDir(dir);
    dir.normalize();

    JGeometry::TVec3f objPos(0.0f, 0.0f, mScale.z * 50.0f);
    setColObjPos(objPos, 0);
}

void GeoMarioTree1::createModel(JKRSolidHeap *heap, u32 viewNo) {
    mModel.createModel(heap, viewNo, 0);
}

const char *GeoMarioTree1::getBmdFileName() {
    static const char *cMarioTree1BmdName = "MarioTree1.bmd";
    return cMarioTree1BmdName;
}

void GeoMarioTree1::calc() {}

// getting trolled by const again
void GeoMarioTree1::doKartColCallBack(const int kartNo) {
    if (tstHitKartSoundFlg(kartNo)) {
        GetKartCtrl()->GetKartSoundMgr(kartNo)->setCrushSe(0x1001f, getHitSoundDegree());
    }
}

GeoAnmTableEntry *GeoMarioTree1::getAnmTbl() {
    if (availBckAnm()) {
        return sBckAnmTable;
    }
    return nullptr;
}

u32 GeoMarioTree1::getSizeAnmTbl() {
    return sizeof(sBckAnmTable); // TODO: was this meant to return the count or the actual size?(possible bug)
}

GeoMarioKinoko1::GeoMarioKinoko1(const CrsData::SObject &object) : GeoTree(object) {}

void GeoMarioKinoko1::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 300.0f, 500.0f);
}

const char *GeoMarioKinoko1::getBmdFileName() {
    static const char *cMarioKinoko1BmdName = "/Objects/MarioKinoko1.bmd";
    return cMarioKinoko1BmdName;
}

void GeoMarioKinoko1::calc() {}
