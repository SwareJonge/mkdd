#include "Kaneshige/Objects/GeoShimmer.h"
#include "JSystem/ResTIMG.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/ShimmerScreen.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"

#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

GeoShimmerSupervisor *GeoShimmer::sSupervisor;

GeoShimmerSupervisor::GeoShimmerSupervisor() {
    mEntryLink = 0;
}

GeoShimmerSupervisor::~GeoShimmerSupervisor() {
    GeoShimmer::sSupervisor = nullptr;
}

void GeoShimmerSupervisor::reset() {}

void GeoShimmerSupervisor::calc() {}

void GeoShimmerSupervisor::entry(GeoShimmer *link) {
#line 119
    JUT_ASSERT_MSG(mEntryLink == nullptr, "SHIMMER NOT ONLY");
    mEntryLink = link;
}

void GeoShimmerSupervisor::drawScreen() {
    mEntryLink->drawScreen();
}

GeoShimmer::GeoShimmer(const CrsData::SObject &obj) : GeographyObj(obj) {
    if (sSupervisor == nullptr) {
        sSupervisor = new GeoShimmerSupervisor();
    }

#line 161
    JUT_ASSERT_MSG(RaceMgr::getManager()->getConsoleNumber() == 1, "ONLY ONE CONSOLE");

    ResTIMG *timg =  nullptr;

    if (isShimmer()) {
        timg = (ResTIMG *)ObjUtility::getPtrCourseArc("/Objects/Shimmer_Ind.bti");
    }

    mScreen = new ShimmerScreen(timg);

    if (mObjData->mParam2 != 0) {
        mScreen->setOffsetZ(0, mObjData->mParam2);
    }

    if (mObjData->mParam3 != 0) {
        mScreen->setOffsetZ(1, mObjData->mParam3);
    }

    if (mObjData->mParam4 != 0) {
        mScreen->setBlendRate(0, mObjData->mParam4);
    }

    if (mObjData->mParam5 != 0) {
        mScreen->setBlendRate(1, mObjData->mParam5);
    }

    reset();
    sSupervisor->entry(this);
}

void GeoShimmer::setCrsOfsZ0() {
    mScreen->setOffsetZ(0, mObjData->mParam2);
}

void GeoShimmer::setCrsOfsZ1() {
    mScreen->setOffsetZ(1, mObjData->mParam3);
}

void GeoShimmer::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
}

void GeoShimmer::createColModel(J3DModelData *) {}

void GeoShimmer::calc() {
    int cameraId;
    if (RaceMgr::getManager()->isCrsDemoMode()) {
        cameraId = RCMGetCamera(0)->GetDemoCam()->mStartId;
    }
    else {
        cameraId = (s16)RCMGetCamera(0)->mCurCamIdx;
    }

    CrsData *crsData = RCMGetCourse()->getCrsData();
    CrsData::Camera *camera = nullptr;

    if (cameraId >= 0) {
        camera = crsData->getCameraData(cameraId);
    }

    setCrsOfsZ0();
    setCrsOfsZ1();

    if (camera) {
        if (camera->getShimmerZ0() != 0) {
            mScreen->setOffsetZ(0, camera->getShimmerZ0());
        }
        if (camera->getShimmerZ1() != 0) {
            mScreen->setOffsetZ(1, camera->getShimmerZ1());
        }
    }

    mScreen->calc();
}

void GeoShimmer::drawScreen() {
    mScreen->draw();
}
