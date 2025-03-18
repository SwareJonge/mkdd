#include "Kaneshige/Objects/GeoKuriCar.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

#include "Sato/StateObserver.h"
#include "mathHelper.h"

const Vec GeoKuriCar::sTireOfsTable[4] = {
    45.0f, 0.0f, 10.0f,
    -45.0f, 0.0f, 10.0f,
    45.0f, 0.0f, -10.0f,
    -45.0f, 0.0f, -10.0f,
};

GeoKuriCar::GeoKuriCar(const CrsData::SObject &obj) : GeoCar(obj) {
    if (obj.mParam1 != 0) {
        _160 = obj.mParam1;
    }
    else {
        _160 = 30.0f;
    }
    
    mFrScale.add(getTirePosOffset(0), getTirePosOffset(1));
    mFrScale.scale(0.5f);

    mBcScale.add(getTirePosOffset(2), getTirePosOffset(3));
    mBcScale.scale(0.5f);

    reset();
}

void GeoKuriCar::reset() {
    GeoCar::reset();
    _58 = 0;
}

void GeoKuriCar::createColModel(J3DModelData *mdlData) {
    createBoundsCylinder(mdlData, 60.0f, 120.0f);
    _158 = 120.0f;
}

const Vec &GeoKuriCar::getTirePosOffset(int tireNo) {
#line 153
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoKuriCar::hitKart(int) {
    if (StateObserver::getState() != 4) {
        StateObserver::setState(4);
    }
}

void GeoKuriCar::initCallBackDeath() {
    mDeathTimer = 0;
    mScale.y = 0.1f;
    clrCarCollision();
}

void GeoKuriCar::doCallBackDeath() {
    mDeathTimer++;

    if (mDeathTimer >= 180) {
        mScale.y = mObjData->scale.y;
        if (IsPathMove()) {
            StateObserver::setState(1);
            setTargetPoint(0);
        }
        else {
            StateObserver::setState(0);
        }
        setCarCollision();
    }
}
