#include "Kaneshige/KartDrawer.h"

void KartDrawer::DriverDB::drawIn(u32) {}

//void KartCtrl::SetCoDriverCurrentViewNo(int, u32) {}

//void KartCtrl::SetDriverCurrentViewNo(int, u32) {}

void KartDrawer::DriverDB::update() {
    DrawBuffer::update();


    DrawBuffer::lock();
}

//void KartCtrl::DrwaCoDriver(int, u16) {}

//void KartCtrl::DrwaDriver(int, u16) {}

KartDrawer::KartDrawer() {}

void KartDrawer::reset() {}

void KartDrawer::create(int, int, u16, EGhostKind) {}

//void OsageDrawBuffer::setTargetKart(short) {}

//void OsageDrawBuffer::OsageDrawBuffer(u32) {}

void KartDrawer::DriverDB::setLevelNo(int) {}

void KartDrawer::DriverDB::setDriverNo(int) {}

void KartDrawer::DriverDB::setKartNo(int) {}

//void DrawBuffer::create(u32) {}

//void KartDrawer::DriverDB::~DriverDB() {}

//void KartDrawer::DriverDB::DriverDB() {}

void KartDrawer::setAnimation() {}

//void KartLoader::isOsageExist(int) {}

void KartDrawer::resetAnimation() {}

void KartDrawer::setTevAnm(int) {}

void KartDrawer::attach(int) {}

void KartDrawer::setAttribute(J3DAnmAttr) {}

void KartDrawer::isAvailableTevAnmShock() {}

//void J3DAnmObjMaterial::getAnmBase() {}

//void J3DFrameCtrl::setAttribute(unsigned char) {}

//void J3DAnmObjBase::getFrameCtrl() {}

void KartDrawer::resetFrame() {}

void KartDrawer::setRate(float) {}

//void J3DAnmObjBase::setRate(const float &) {}

//void J3DFrameCtrl::setRate(float) {}

void KartDrawer::kartAnmFrameProc() {}

//void J3DFrameCtrl::setFrame(float) {}

void KartDrawer::frameProc() {}

void KartDrawer::setShadowInfo(const CrsArea &) {}

void KartDrawer::isFlashHidding() {}

void KartDrawer::isFlashing() const {}

void KartDrawer::enableDrawing(u32, KartDrawer::EDrawStage) {}

void KartDrawer::isHide(u32) const {}

//void KartInfo::isAvailableKart() const {}

//void KartInfo::isAvailableDriver(int) const {}

void KartDrawer::drawKart(u32, KartDrawer::EDrawStage) {}

void KartDrawer::drawDriver(u32, KartDrawer::EDrawStage) {}

void KartDrawer::update() {}

//void KartLoader::getWheelNumber() {}

void KartDrawer::hide() {}

void KartDrawer::setLODLevel(u32, u16) {}

void KartDrawer::playTevAnm(int) {}

void KartDrawer::stopTevAnm() {}
