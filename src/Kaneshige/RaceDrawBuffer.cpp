#include "Kaneshige/DrawBuffer.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Kaneshige/KartLoader.h"
#include "Kaneshige/RaceMgr.h"
#include "Kawano/feeling.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/J3DEffectMgr.h"
#include "Sato/stEffectMgr.h"

void ShadowGeoDrawBuffer::drawIn(u32 viewNo) {
    GetGeoObjMgr()->setCurrentViewNo_groundObj(viewNo);
}

void ShadowGeoDrawBuffer::update() {
    DrawBuffer::update();
    GetGeoObjMgr()->update_groundObj();
    DrawBuffer::lock();
}

void ShadowGeoDrawBuffer::viewCalc(u32 viewNo) {
    GetGeoObjMgr()->viewCalc_groundObj(viewNo);
}

void GeoDrawBuffer::drawIn(u32 viewNo) {
    GetGeoObjMgr()->setCurrentViewNo(viewNo);
}

void GeoDrawBuffer::update() {
    DrawBuffer::update();
    GetGeoObjMgr()->update();
    DrawBuffer::lock();
}

void GeoDrawBuffer::viewCalc(u32 viewNo) {
    GetGeoObjMgr()->viewCalc(viewNo);
}

void JugemDrawBuffer::drawIn(u32 viewNo) {
    GetGeoObjMgr()->setCurrentViewNo_foreground(viewNo);
    int kartNum = GetGeoObjMgr()->getJugem(viewNo)->getKartNum();
    RaceMgr::getManager()->getKartDrawer(kartNum)->getLight(viewNo)->draw();

    if (GetGeoObjMgr()->getJugem(viewNo)->isAllShapePacketHidding(viewNo)) {
        DrawBuffer::clrDraw();
    }
}

void JugemDrawBuffer::update() {
    DrawBuffer::update();
    GetGeoObjMgr()->update_foreground();
    DrawBuffer::lock();
}

void JugemDrawBuffer::viewCalc(u32 viewNo) {
    GetGeoObjMgr()->viewCalc_foreground(viewNo);
}

void ItemDrawBuffer::drawIn(u32 viewNo) {
    if (mTargetKart == -1) {
        GetItemObjMgr()->setCurrentViewNo(viewNo);
    }
}

void ItemDrawBuffer::update() {
    DrawBuffer::update();
    ItemObjMgr::eDrawSimplModelItemType type = ItemObjMgr::ItemType_2;
    if (RaceMgr::getManager()->isActiveAreaLight()) {
        if (mTargetKart >= 0) {
            type = ItemObjMgr::ItemType_1;
        }
    }
    else {
        type = ItemObjMgr::ItemType_0;
    }

    GetItemObjMgr()->update(type, mTargetKart);
}

void ItemDrawBuffer::viewCalc(u32 viewNo) {
    if (mTargetKart == -1) {
        GetItemObjMgr()->viewCalc(viewNo);
    }
}

void EffectDrawBuffer::drawIn(u32 viewNo) {
    GetStEfctMgr()->setCurrentViewNo(viewNo);
    GetJ3DEfctMgr()->setCurrentViewNo(viewNo);
}

void EffectDrawBuffer::update() {
    DrawBuffer::update();
    GetStEfctMgr()->update();
    GetJ3DEfctMgr()->update();
}

void EffectDrawBuffer::viewCalc(u32 viewNo) {
    GetStEfctMgr()->viewCalc(viewNo);
    GetJ3DEfctMgr()->viewCalc(viewNo);
}

void FeelDrawBuffer::drawIn(u32 viewNo) {
    FeelingMgr::ptr()->setCurrentViewNo(viewNo);
}

void FeelDrawBuffer::update() {
    DrawBuffer::update();
    FeelingMgr::ptr()->update();
    DrawBuffer::lock();
}

void FeelDrawBuffer::viewCalc(u32 viewNo) {}

void OsageDrawBuffer::drawIn(u32 viewNo) {
    KartLoader *kartLoader = RCMGetKartLoader(mTargetKart);
    for (int i = 0; i < 2; i++) {
        ExModel *osageMdl = kartLoader->getExModelOsage(i);
        if (osageMdl) {
            osageMdl->setCurrentViewNo(viewNo);
        }
    }
}

void OsageDrawBuffer::update() {
    DrawBuffer::update();
    KartLoader *kartLoader = RCMGetKartLoader(mTargetKart);
    for (int i = 0; i < 2; i++) {
        ExModel *osageMdl = kartLoader->getExModelOsage(i);
        if (osageMdl) {
            osageMdl->update(0);
        }
    }
    DrawBuffer::lock();
}

void OsageDrawBuffer::viewCalc(u32 viewNo) {}
