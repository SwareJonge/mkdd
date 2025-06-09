#include "Kaneshige/Objects/GeoSoundGen.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoSoundGenSupervisor *GeoSoundGen::sSupervisor;

GeoSoundGenSupervisor::GeoSoundGenSupervisor() {
    mList.initiate();
}

GeoSoundGenSupervisor::~GeoSoundGenSupervisor() {
    GeoSoundGen::sSupervisor = nullptr;
}

void GeoSoundGenSupervisor::reset() {}

void GeoSoundGenSupervisor::calc() {}

void GeoSoundGenSupervisor::entry(GeoSoundGen *soundGen) {
    mList.append(&soundGen->mLink);
}

GeoSoundGen::GeoSoundGen(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {
    if (sSupervisor == nullptr) {
        sSupervisor = new GeoSoundGenSupervisor();
    }

    reset();

    s16 param = mObjData->mParam1;
    GameAudio::MapSoundObjectMgr::Type type = GameAudio::MapSoundObjectMgr::MapSound_Type0;
    switch (param) {
    case 0:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type0;
        break;
    case 1:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type1;
        break;
    case 2:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type2;
        break;
    case 3:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type3;
        break;
    case 4:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type4;
        break;
    case 5:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type5;
        break;
    case 6:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type6;
        break;
    case 7:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type7;
        break;
    case 8:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type8;
        break;
    case 9:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type9;
        break;
    case 10:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type10;
        break;
    case 11:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type11;
        break;
    case 12:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type12;
        break;
    case 13:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type13;
        break;
    case 14:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type14;
        break;
    case 15:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type15;
        break;
    case 16:
        type = GameAudio::MapSoundObjectMgr::MapSound_Type16;
        break;
    default:
#line 170
        JUT_ASSERT_F(false, "UNKNOWN TYPE:%d", param);
        break;
    }

    bool isNormalRace = true;

    switch (type) {
    case GameAudio::MapSoundObjectMgr::MapSound_Type5:
    case GameAudio::MapSoundObjectMgr::MapSound_Type6:
    case GameAudio::MapSoundObjectMgr::MapSound_Type7:
    case GameAudio::MapSoundObjectMgr::MapSound_Type8:
    case GameAudio::MapSoundObjectMgr::MapSound_Type9:
    case GameAudio::MapSoundObjectMgr::MapSound_Type10:
    case GameAudio::MapSoundObjectMgr::MapSound_Type11:
    case GameAudio::MapSoundObjectMgr::MapSound_Type12:
    case GameAudio::MapSoundObjectMgr::MapSound_Type13:
    case GameAudio::MapSoundObjectMgr::MapSound_Type14: {
        if (RaceMgr::getManager()->getAwardKartNo() < 0) {
            isNormalRace = false;
        }
        break;
    }
        
    case GameAudio::MapSoundObjectMgr::MapSound_Type15:
    case GameAudio::MapSoundObjectMgr::MapSound_Type16: {
        if (RaceMgr::getManager()->getAwardKartNo() >= 0) {
            isNormalRace = false;
        }
        break;
    }
    }

    if (isNormalRace) {
        mMapSoundMgr = new GameAudio::MapSoundObjectMgr(&mPos, type, nullptr);
    }
    else {
        mMapSoundMgr = nullptr;
    }
    sSupervisor->entry(this);
}

void GeoSoundGen::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
}

void GeoSoundGen::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsSphere(150.0f, 1.0f);
}

void GeoSoundGen::calc() {
    if (mMapSoundMgr) {
        mMapSoundMgr->frameWork();
    }
}
