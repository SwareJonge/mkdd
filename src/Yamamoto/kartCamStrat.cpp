#include "Yamamoto/kartCamera.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

// Unused function calls set from TVec3f
// comments inside functions are inline functions being called in that function

void KartCam::SetTargetNum(unsigned char) {}

void KartCam::SetClipper() {
    // void J3DUClipper::setFovy(float) {}
    // void J3DUClipper::setFar(float) {}
    // void J3DUClipper::setNear(float) {}
    // void J3DUClipper::setAspect(float) {}
}

void KartCam::CheckClipper() {}

GrafPort::GrafPort(int, int, int, int, float, float, float) {}

void GrafPort::setPort() {}

void GrafPort::SetParam(float, float, float) {}

void KartCam::MakeShaker(float) {}

void KartCam::DoShaker(JGeometry::TVec3<float> *, JGeometry::TVec3<float>) {}

void KartCam::SetPtr() {}

JGeometry::TVec3f *KartCam::GetCameraPos() {}

JGeometry::TVec3f *KartCam::GetCameraLookPos() {}

void KartCam::SetFovyData() {}

void KartCam::First4ScreenPort(unsigned char) {}

void KartCam::Second4ScreenPort(unsigned char) {}

void KartCam::Third4ScreenPort(unsigned char) {}

void KartCam::Fourth4ScreenPort(unsigned char) {}

void KartCam::First2ScreenPort(unsigned char) {}

void KartCam::Second2ScreenPort(unsigned char) {}

void KartCam::SetVictoryScreenPort(unsigned char) {
    // void RaceMgr::getZoomWinConsoleNo() const {}
    // void RaceDirector::getZoomWinConsoleNo() const {}
    // void RaceMgr::isZoomWinConsole() const {}
}

void KartCam::DoMoveCamera(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartCam::DoRoof(JGeometry::TVec3<float> *, CrsArea *) {}

void KartCam::DoColCamera() {}

void KartCam::DoSea(JGeometry::TVec3<float> *, CrsGround *) {}

void KartCam::DoGround(JGeometry::TVec3<float> *, CrsGround *) {}

void KartCam::GroundCheck(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, CrsArea *) {}

void KartCam::OutViewCalc() {}

void KartCam::InitOutView() {}

void KartCam::DoChangFovy() {}

void KartCam::DoLookChase() {}

void KartCam::DoYRotation() {}

void KartCam::DoXRotation() {}

void KartCam::DoDist() {}

void KartCam::DoCamPos(float, JGeometry::TVec3<float> *) {}

void KartCam::OutView() {}

void KartCam::InitRaceBackView() {}

void KartCam::ParallelView() {}

void KartCam::InitBackView() {}

void KartCam::BackView() {}

void KartCam::HangRescueView() {}

void KartCam::InitDropRescueView() {}

void KartCam::DropRescueView() {}

void KartCam::LaunchView() {}

void KartCam::LandView() {}

void KartCam::InitPipeView() {}

void KartCam::PipeView() {}
