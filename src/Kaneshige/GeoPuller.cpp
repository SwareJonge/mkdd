#include "Kaneshige/Objects/GeoPuller.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"
#include "kartEnums.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoPullerSupervisor *GeoPuller::sSupervisor;

GeoPullerSupervisor::GeoPullerSupervisor() {
    mList.initiate();
}

GeoPullerSupervisor::~GeoPullerSupervisor() {
    GeoPuller::sSupervisor = nullptr;
}

void GeoPullerSupervisor::reset() {}

void GeoPullerSupervisor::calc() {

}

void GeoPullerSupervisor::entry(GeoPuller *puller) {
    mList.append(&puller->mPullerLink);
}

GeoPuller *GeoPullerSupervisor::search(u32 id) {
    GeoPuller *puller = nullptr;
    for (JSUListIterator<GeoPuller> it(mList.getFirst()); it != mList.getEnd(); it++) {
        if (it->isSameID(id)) {
            puller = it.getObject();
            break;
        }
    }
#line 131
    JUT_ASSERT_F(puller, "PULLER NOT FOUND ID:%d", id);
    return puller;
}

GeoPuller::GeoPuller(const CrsData::SObject &obj) : GeographyObj(obj), mPullerLink(this) {
    if (sSupervisor == nullptr) {
        sSupervisor = new GeoPullerSupervisor();
    }

    switch (RaceMgr::getManager()->getRaceLevel()) {
    case LVL_50CC: {
        _15c = mObjData->mParam2;
        break;
    }
    case LVL_100CC: {
        _15c = mObjData->mParam3;
        break;
    }
    case LVL_150CC: 
    case LVL_MIRROR: {
        _15c = mObjData->mParam4;
        break;
    }
    default: {
        _15c = 0.0f;
    }
    }

    _160 = _15c;
    _164 = mObjData->mParam5;
    reset();
    sSupervisor->entry(this);
}

void GeoPuller::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::setObjFlagCheckItemHitting();
}

void GeoPuller::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsSphere(350.0f, 1.0f);
}

void GeoPuller::calc() {
    bool b = false;

    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        if (RaceMgr::getManager()->getKartInfo(i)->isPlayerKart()) {
            JGeometry::TVec3f kartPos;
            ObjUtility::getKartPos(i, &kartPos);
            JGeometry::TVec3f vel;
            vel.sub(kartPos, mPos);
            if (vel.length() <= 7000.0f) {
                b = true;
                break;
            }
        }
    }

    if (b) {
        _15c = _160;
    }
    else {
        _15c = 0.0f;
    }
}

void GeoPuller::getVelocity(JGeometry::TVec3f *out, const JGeometry::TVec3f &targetPos) {
    out->sub(mPos, targetPos);
    f32 velScale = 0.0f;
    f32 len = out->length();
    if (len > 0.001f) {
        f32 tmp = (((_164 - 100.0f) * _15c) / 100000.0f);
        f32 scale = _15c + tmp * len;
        if (scale < 0.0f) {
            scale = 0.0f;
        }
        velScale = scale / len;
    }

    out->scale(velScale);
}
