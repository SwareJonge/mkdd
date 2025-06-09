#include "Kaneshige/Objects/GeoTideSensor.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JSupport/JSUList.h"
#include "Kaneshige/Objects/GeoWater.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoTideSensorSupervisor *GeoTideSensor::sSupervisor;

f32 GeoTideSensor::sSearchDistance = 20000.0f;

GeoTideSensorSupervisor::GeoTideSensorSupervisor() {
    mList.initiate();
    mShoreSoundMgr = new GameAudio::ShoreSoundMgr(nullptr);

    _4c = 0;

    for (int i = 0; i < MAX_NUM_SENSORS; i++) {
        mSensors[i] = nullptr;
    }
}

GeoTideSensorSupervisor::~GeoTideSensorSupervisor() {
    GeoTideSensor::sSupervisor = nullptr;
}

void GeoTideSensorSupervisor::reset() {}

void GeoTideSensorSupervisor::calc() {
    GeoWater *water = GeoWater::getSupervisor()->searchWater(0);
    bool flow = false;
    if (water->isFlow()) {
        flow = true;
    }

    mShoreSoundMgr->setStatus(flow, water->getSurfaceHeight());

    for (int i = 0; i < RaceMgr::getManager()->getCameraNumber(); i++) {
        JGeometry::TVec3f *camPos = RaceMgr::getManager()->getCamera(i)->GetCameraPos();
        mSensors[0] = searchNearSensor(0, *camPos, water->getSurfaceHeight());
        mSensors[1] = searchNearSensor(1, *camPos, water->getSurfaceHeight());

        JGeometry::TVec3f *tidePos1 = nullptr;
        if (mSensors[0]) {
            tidePos1 = &mSensors[0]->mPos;
        }

        JGeometry::TVec3f *tidePos2 = nullptr;
        if (mSensors[1]) {
            tidePos2 = &mSensors[1]->mPos;
        }
        mShoreSoundMgr->setTide(i, tidePos1, tidePos2);
    }
    mShoreSoundMgr->frameWork();
}

void GeoTideSensorSupervisor::entry(GeoTideSensor *sensor) {
    mList.append(&sensor->mLink);
}

GeoTideSensor *GeoTideSensorSupervisor::searchNearSensor(int groupId, const JGeometry::TVec3f &camPos, f32 h) {
    GeoTideSensor *near = nullptr;
    f32 prevDist = -1.0f;
    for (JSUListIterator<GeoTideSensor> it(mList.getFirst()); it != mList.getEnd(); it++) {

        if (it->isSameGroup(groupId) && it->isInside(h)) {
            f32 x = camPos.x - it->mPos.x;
            f32 z = camPos.z - it->mPos.z;

            f32 distSq = x * x + z * z;
            f32 searchDistSq = GeoTideSensor::sSearchDistance * GeoTideSensor::sSearchDistance;
            if (!(distSq > searchDistSq) && (near == nullptr || distSq < prevDist)) {
                prevDist = distSq;
                near = it.getObject();
            }
        }
    }
    return near;
}

GeoTideSensor::GeoTideSensor(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {
    if (sSupervisor == nullptr) {
        sSupervisor = new GeoTideSensorSupervisor();
    }
    reset();

    sSupervisor->entry(this);
}

void GeoTideSensor::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
}

void GeoTideSensor::createColModel(J3DModelData *) {}

void GeoTideSensor::calc() {}
