#include "Yamamoto/KartRescue.h"
#include "Kameda/WipeManager.h"
#include "Kaneshige/Course/Course.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

// comments inside functions are inline functions being called in that function

void KartRescue::Init(int) {}

void KartRescue::DoKeep() {}

void KartRescue::GetJugemuStatus() {
    // void CrsGround::operator= (const CrsGround &) {} implicit most likely
    // void GeographyObj::getObjData() const {}
}

void KartRescue::SetPose(unsigned char) {}

void KartRescue::SetJugemuPose(int, KartBody *) {}

void KartRescue::DoMove(KartBody *) {}

void KartRescue::SetHookPos(KartBody *) {}

void KartRescue::DoCatch() {
    // void KartChecker::beginWarp(JugemPoint *) {}
    // void KartCam::SetFlag(unsigned short) {}
}

void KartRescue::DoTop() {}

void KartRescue::DoDrop() {}

void KartRescue::DoCheckEnd() {
    // void KartCam::ClrFlag(unsigned short) {}
}

void KartRescue::MakeRescue(GeographyObj *, unsigned char) {}

void KartRescue::DoRescueCrl() {
    // void KartChecker::endWarp() {}
    // void RivalKart::getBodyCtrl() {}
    // void JugemPoint::getCLPoint() {}
    // void Course::isFreeze() {}
    // void CrsData::isSnow() const {}
    // void TMapObjAntLion::eat() {}
    // void GETWipeManager() {}
    // void WipeManager::getManager() {}
}

void KartRescue::DoAfterRescueCrl() {}
