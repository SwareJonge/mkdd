#ifndef JPEFFECTPERFORMER_H
#define JPEFFECTPERFORMER_H

#include "Kaneshige/Course/CrsGround.h"

class JPABaseEmitter; // TODO JSystem

class JPEffectPerformer
{ // Autogenerated
public:
    enum EffectType
    {
        // TODO
        Effect_Star = 0x1c
    };

    void init();                                                       // 80266010
    void reset();                                                      // 80266178
    void loadCourseParam(u32);                                         // 802662fc
    void calc();                                                       // 802663d4
    void deleteKartEfctKoukasen(int, u8);                              // 80266458
    void deleteKartEfctTireSand(int);                                  // 802664ac
    void getNearestCamPtr(const JGeometry::TVec3f &);                  // 8026651c
    static void setEffect(EffectType, int, const JGeometry::TVec3f &, u8);    // 80266728
    void setEffectSplash(CrsGround *, int, const JGeometry::TVec3f &); // 80266858
    void setEffectEachCam(EffectType, int, u8, u8);                    // 80266a8c
    void setLandEffect(const JGeometry::TVec3f &, CrsGround *, int);   // 80266bf0
    void getLandingEfctName(CrsGround *);                              // 80266d20
    void getKartEfctEmts(int);                                         // 80266dcc
    void deleteKartEfctWet(int);                                       // 80266e64
    // void sKartEfctEmts;                                                              // 804119d0
    // void sPCounters;                                                                 // 8041299c
    // void sPCounterObserver;                                                          // 8041375c
    // void sKoukasenLength;                                                            // 80414d40
    // void sCrs_Params;                                                                // 80416c74
    // void sCrs_ParamNum;                                                              // 80416c78
    // void sCharSmokeParamSpecialFlg;                                                  // 80416c7c
    // void sStartFootSmokeIndex;                                                       // 80416c7d

    // Inline/Unused
    void TstEmitterClipNotAvailableFlg(int, JPABaseEmitter *);
    void setKartThunderScale(int, JPABaseEmitter *);
    void TstEmitterStopKartState(int, JPABaseEmitter *);
    void setEmitterKartOffsetData(int, JPABaseEmitter *);
    // void JPTireTraceEmitterCallBack::execute_common(JPABaseEmitter *);
    // void PermissionCounterObserver::init();
    void calcEfct_KartSpinBase(int, const JGeometry::TVec3f &, const char *);
    void calcEfct_KartSplashBase(int, const JGeometry::TVec3f &, char **);
    void calcEfct_KartKinokoDashBase(int, const JGeometry::TVec3f &, u8, const char *);
    void calcEfct_KartKinokoDashBase2(int, const JGeometry::TVec3f &, u8, u8, char *);
    void calcEfct_KartAccelExhaustBase(int, const JGeometry::TVec3f &, u8);
    void calcEfct_KartKinokoDash3Base(int, const JGeometry::TVec3f &, u8, char **);
    void calcEfct_KartTireSandBase(int, const JGeometry::TVec3f &, u8, const char *);
    void calcEfct_KartDriftFireChipBase(int, const JGeometry::TVec3f &, u8);
    JPEffectPerformer();
    ~JPEffectPerformer();
}; // class JPEffectPerformer
// Outside class members
// void TstEmitterModeNotAvailableFlg(int, JPABaseEmitter *);
// void TstEmitterModeAndClipNotAvailableFlg(int, JPABaseEmitter *);
// void JPTraceEmitterCallBack::execute(JPABaseEmitter *);
// void JPTraceEmitterCallBack::draw(JPABaseEmitter *);
// void JPRotateOffsetEmitterCallBack::execute(JPABaseEmitter *);
// void JPRotateOffsetEmitterCallBack::draw(JPABaseEmitter *);
// void JPRotateXZEmitterCallBack::execute(JPABaseEmitter *);
// void JPRotateXZEmitterCallBack::draw(JPABaseEmitter *);
// void JPRotateEmitterCallBack::execute(JPABaseEmitter *);
// void JPRotateEmitterCallBack::calc_common(JPABaseEmitter *);
// void JPRotateEmitterCallBack::draw(JPABaseEmitter *);
// void JPBonusKartEfctEmitterCallBack::execute(JPABaseEmitter *);
// void JPKinokoDashEmitterCallBack::execute(JPABaseEmitter *);
// void JPKinokoDashEmitterCallBack::draw(JPABaseEmitter *);
// void JPKinokoDash2EmitterCallBack::execute(JPABaseEmitter *);
// void JPKinokoDash2EmitterCallBack::draw(JPABaseEmitter *);
// void JPPtclTraceEmitterCallBack::execute(JPABaseEmitter *);
// void JPPtclTraceEmitterCallBack::execute_common(JPABaseEmitter *);
// void JPPtclTraceEmitterCallBack::draw(JPABaseEmitter *);
// void JPStarEmitterCallBack::execute(JPABaseEmitter *);
// void JPTireTraceEmitterCallBack::draw(JPABaseEmitter *);
// void JPWheelSpinEmitterCallBack::execute(JPABaseEmitter *);
// void JPWheelSpinEmitterCallBack::draw(JPABaseEmitter *);
// void JPWaterWaveEmitterCallBack::execute(JPABaseEmitter *);
// void JPWaterWaveEmitterCallBack::draw(JPABaseEmitter *);
// void JPWaterRippleEmitterCallBack::execute(JPABaseEmitter *);
// void JPWaterRippleEmitterCallBack::draw(JPABaseEmitter *);
// void JPKoukasenEmitterCallBack::execute(JPABaseEmitter *);
// void JPKoukasenEmitterCallBack::draw(JPABaseEmitter *);
// void JPCharFireEmitterCallBack::execute(JPABaseEmitter *);
// void JPCharFireEmitterCallBack::draw(JPABaseEmitter *);
// void JPTireSandEmitterCallBack::execute(JPABaseEmitter *);
// void JPTireSandEmitterCallBack::draw(JPABaseEmitter *);
// void JPKartWetEmitterCallBack::execute(JPABaseEmitter *);
// void JPKartWetEmitterCallBack::draw(JPABaseEmitter *);
// void JPTraceParticleCallBack::execute(JPABaseEmitter *, JPABaseParticle *);
// void JPTraceParticleCallBack::draw(JPABaseEmitter *, JPABaseParticle *);
// void JPStartFootSmokeEmitterCallBack::execute(JPABaseEmitter *);
// void JPStartFootSmokeEmitterCallBack::draw(JPABaseEmitter *);
// void PermissionCounterObserver::reset();
// void PermissionCounterObserver::calc();
#endif // JPEFFECTPERFORMER_H