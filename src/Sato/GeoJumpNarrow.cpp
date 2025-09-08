#include "Sato/Objects/GeoJumpNarrow.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/ObjCollision.h"

GeoJumpNarrow::GeoJumpNarrow(const CrsData::SObject &object) : ExGeographyObj(object) {

}

void GeoJumpNarrow::reset() {
    GeographyObj::reset();
    _58 = 1;
}

void GeoJumpNarrow::createColModel(J3DModelData *mdlData) {
    mExBounds = new ObjColJump1(mdlData, mScale);
    mExBounds->setBottom(mPos.y);
    Mtx m;
    GeographyObj::getAssignedModelMatrix(m);

    mExBounds->makeParameter(mdlData, m);
    mExBounds->setAttr(6);

}

const char *GeoJumpNarrow::getBmdFileName() {
    static const char *cBmdName = "/Objects/Njump.bmd";
    return cBmdName; 
}

void GeoJumpNarrow::calc() {}
