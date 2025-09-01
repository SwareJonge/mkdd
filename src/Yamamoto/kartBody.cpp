#include "Yamamoto/kartBody.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/GeographyObjMgr.h"

// settingPtr2
// settingMiniPtr2

// body settings

// comments inside functions are inline functions being called in that function

void KartBody::DegubBody(unsigned long) {
    // void SysDebug::checkNaNVector(Vec *, unsigned long) {}

    //void SysDebug::checkNaNMatrix(float (*) [4], unsigned long) {}
}

void KartBody::MakeMassBody(float, float, float, float) {}

void KartBody::MakeRigidVertexBody(float, float, float, float) {}

void KartBody::SetUpRigidBodyPose(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {
    //void std::atan2f(float, float) {}
}

void KartBody::SetUpRigidBodyStartGridPose() {
    // void JGeometry::TVec3<float>::setNormal(const JGeometry::TVec3<float> &, const JGeometry::TVec3<float> &, const JGeometry::TVec3<float> &) {}
    // void KartBody::getSus(int) {}
}

void KartBody::DoForce(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::DoTq(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::DoForceOnly(KartBody *, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::DoForceOnly(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::DoGForce() {
    // int KartBody::getTouchNum() {}
}

void KartBody::CalcMovement(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::DoCalcWgVel() {
    //void KartBody::getGame() {}
    //void KartBody::getStrat() {}
    //void KartThunder::getScale() {}
    //void KartBody::getThunder() {}
    //void JGeometry::TVec3<float>::div(float, const JGeometry::TVec3<float> &) {}
    //void JGeometry::TUtil<float>::invert(float) {}
}

void KartBody::DoPose() {}

void KartBody::DoCalcMassMatrix(float (*) [4], float, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *) {}

void KartBody::WallFriction(JGeometry::TVec3<float> *, float, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, float) {}

void KartBody::GroundReflection(JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, float, float, float) {}

void KartBody::RigidReflectionAnime(KartBody *, KartBody *, float, float) {
    // void KartCtrl::getKartSound(int) {}
    // void KartCtrl::getKartAnime(int) {}
    // void KartBody::getChecker() {}
}

void KartBody::RigidReflectionPower(KartBody *, KartBody *, float *, float *) {
    // void KartBody::getCrash() {}
    // void KartBody::getTumble() {}
    // void KartBody::getDamage() {}
}

void KartBody::RigidReflection(KartBody *, KartBody *, JGeometry::TVec3<float> *, JGeometry::TVec3<float> *, float, float, float) {
    // void JGeometry::TVec3<float>::angle(const JGeometry::TVec3<float> &) const {}
    // void JGeometry::TUtil<float>::abs(float) {}
    // void JGeometry::TUtil<float>::atan2(float, float) {}
    // void JMAAtan2Radian(float, float) {}
    // void JMath::TAtanTable<1024, float>::atan2Radian(float, float) const {}
}

void KartBody::ObjectReflection(JGeometry::TVec3<float> *) {}

void KartBody::ObjectWallReflection(GeographyObj *) {
    // void GeographyObjManager::getKartHitRefVecNormalObj(int) const {}
    // void GeographyObjManager::getKartHitDepthNormalObj(int) const {}
    // void GeographyObj::getPosition(JGeometry::TVec3<float> *) const {}
}

void KartBody::StarReact(GeographyObj *) {}

void KartBody::CheckObjectReflection() {
    // void KartBody::getDossin() {}
    // void GeographyObjManager::getKartReactHitObjectList(int) {}
}

void KartBody::CheckRoofReflection() {}

void KartBody::WallReflection(JGeometry::TVec3<float> *, float, float, float) {}

void KartBody::GroundVertexReflection(int, CrsGround, JGeometry::TVec3<float> *, unsigned char *) {}

void KartBody::WallVertexReflection(CrsGround, JGeometry::TVec3<float> *, unsigned char *, unsigned char *) {}

void KartBody::CheckVertexReflection() {
    // void CrsGround::CrsGround(const CrsGround &) {}
    // void KartBody::CheckBodyHit(KartBody *, KartBody *) {}
}

void KartBody::InitBodySetting() {}

void KartBody::GetMiniGameCamData(int) {
    // void RaceMgr::getCurrentManager() {}
}

void KartBody::GetPakkunMiniGameCamData(int) {}

void KartBody::GetPakkunGameCamData(int) {}

void KartBody::InitCamSetting() {
    // void RaceMgr::isSubScrExist() const {}
}

void KartBody::InitBodyCamParam() {}

void KartBody::InitBodyParam() {
    //void KartInfo::getWeight() const {}
    //void KartInfo::getDriverWeight(int) const {}
    //void KartInfo::KartCharacter::getWeight() const {}
}

void KartBody::Init(int) {
    //void KartShadowModel::setScale(float, float) {}
    //void KartLoader::getShadowModel() {}
    //void CrsGround::getObject() const {}
}
