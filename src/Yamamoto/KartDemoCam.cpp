#include "Yamamoto/KartDemoCam.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

// comments inside functions are inline functions being called in that function

void KartDemoCam::Init(KartCam *) {}

void KartDemoCam::ChaseFovy(const CrsData::Camera *) {
    // void KartCam::SetFovy(float) {}
    // void CrsData::Camera::getFovy() const {}
    // void CrsData::Camera::getFovy2() const {}
}

void KartDemoCam::DoInitCol() {}

void KartDemoCam::DoCol() {}

void KartDemoCam::InitFixCam() {
    // void CrsData::Camera::getPosition(JGeometry::TVec3<float> *) const {}
}

void KartDemoCam::DoFixCam() {}

void KartDemoCam::InitFixSearchCam() {}

void KartDemoCam::DoFixSearchCam() {}

void KartDemoCam::InitFixChaseCam() {}

void KartDemoCam::DoFixChaseCam() {}

void KartDemoCam::InitChaseCam() {}

void KartDemoCam::DoChaseCam() {}

void KartDemoCam::InitFixPathCam() {
    // void CrsData::Camera::getPathID() const {}
}

void KartDemoCam::DoFixPathCam() {
    // void CrsData::Camera::getPathSpeed() const {}
}

void KartDemoCam::InitChasePathCam() {}

void KartDemoCam::DoChasePathCam() {}

void KartDemoCam::InitFixSplCam() {}

void KartDemoCam::DoFixSplCam() {}

void KartDemoCam::InitChaseSplCam() {}

void KartDemoCam::DoChaseSplCam() {}

void KartDemoCam::InitFixPalaCam() {
    // void CrsData::Camera::getTargetPos2(JGeometry::TVec3<float> *) const {}
    // void CrsData::Camera::getTargetPos(JGeometry::TVec3<float> *) const {}
}

void KartDemoCam::DoFixPalaCam() {}

void KartDemoCam::InitDemoPathCam() {}

void KartDemoCam::DoDemoPathCam() {}

void KartDemoCam::InitDemoParallelView() {}

void KartDemoCam::DemoParallelView() {}

void KartDemoCam::InitDemoGoalView() {}

void KartDemoCam::DemoGoalView() {}

void KartDemoCam::MapView() {}

void KartDemoCam::ObjectView() {}

void KartDemoCam::DemoInitID() {
    // void CrsData::Camera::getKind() const {}
}

void KartDemoCam::InitID() {}

void KartDemoCam::DoDemoView() {}

void KartDemoCam::LiveViewChange() {}

void KartDemoCam::DemoView() {
    // void RaceMgr::requestDemoCameraID() const {}
}

void KartDemoCam::DoPathEnd(JGeometry::TVec3<float> *, float *) {}

void KartDemoCam::InitStartFixPathCam() {}

void KartDemoCam::DoStartFixPathCam() {}

void KartDemoCam::InitStartPathCam() {}

void KartDemoCam::DoStartPathCam() {}

void KartDemoCam::InitStartLookPathCam() {}

void KartDemoCam::DoStartLookPathCam() {}

void KartDemoCam::InitStartOnlyPathCam() {}

void KartDemoCam::DoStartOnlyPathCam() {}

void KartDemoCam::InitStartFixCam() {}

void KartDemoCam::DoStartFixCam() {}

void KartDemoCam::InitStartChasePathCam() {}

void KartDemoCam::DoStartChasePathCam() {}

void KartDemoCam::InitVsLastCam() {}

void KartDemoCam::VsLastCam() {}

void KartDemoCam::InitLastCam() {
    // void TJugem::getPosition() const {}
}
void KartDemoCam::LastCam() {}

void KartDemoCam::InitStartDemoCam() {}

void KartDemoCam::StartDemoCam() {}

void KartDemoCam::SetLastDemo() {}

void KartDemoCam::SetRaceCam() {}

void KartDemoCam::SetNextID() {}

void KartDemoCam::DoExecution() {}

u8 KartDemoCam::GetStartID() {}

void KartDemoCam::InitStartID(bool) {}

void KartDemoCam::StartDemoView() {}

void KartDemoCam::InitStartMapCam() {}

void KartDemoCam::StartMapCam() {}

void KartDemoCam::SetMapNextID() {}

void KartDemoCam::SetUpMapID() {}

void KartDemoCam::InitStaffView() {}

void KartDemoCam::StaffView() {}

void KartDemoCam::SetStaffNextID() {}

void KartDemoCam::SetUpStaffID() {}
