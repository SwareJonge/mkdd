#ifndef STMATH_H
#define STMATH_H

#include "types.h"

#include "JSystem/JGeometry.h"

using namespace JGeometry;

struct stPlaneParam
{
};

int stVecNormalize(TVec3<float> &);
int stVecNormalize(TVec3<float> *);
void stClampVecMax(TVec3<float> &, float);
void stClampVecMin(TVec3<float> &, float);
f32 stspeedy_sqrtf(register f32 x);
f32 stLength2(float, float);
void stMakeDirectionMtx(TMtx34f *, const TVec3<float> &, char);
void stMakeRMtx(Mtx, const TVec3<float> &, const TVec3<float> &, const TVec3<float> &);
void stQt2Mtx(Mtx, const Quaternion *);
void stVec2QtUpdate(Quaternion &, Quaternion &, const TVec3<float> &, const TVec3<float> &);
void stVec2QtUpdate(Quaternion &, const Vec &, const Vec &);
void stVec2Qt(Quaternion &, const Vec &, const Vec &);
void stMtx2Qt(Quaternion *, const Mtx);
void stQtLerp(Quaternion *, const Quaternion *, const Quaternion *, float);
void stQtNormalize(Quaternion *, const Quaternion *);
void stMakePlaneParam(stPlaneParam &, TVec3<float> &, const TVec3<float> &);
void stMakePlaneParam(stPlaneParam &, const TVec3<float> &, const TVec3<float> &, const TVec3<float> &);
void stSearchInSurface(const TVec3<float> &, const TVec3<float> &, const TVec3<float> &);
void stSearchInSurface(const TVec3<float> &, const stPlaneParam &);
void stCollideSurfaceAndSphere(const TVec3<float> &, float, const stPlaneParam &, float &);
void stCollideLineToPlaneIn(const TVec3<float> &, const TVec3<float> &, const stPlaneParam &);
void stGetCollidePosFromT(const TVec3<float> &, const TVec3<float> &, float);
void stGetCollideDepthFromT(const TVec3<float> &, const TVec3<float> &, float);
void stMTXRotDeg(Mtx, char, float);
void stMTXRotRad(Mtx, char, float);

struct stRandom : public JMath::TRandom_fast_
{
    stRandom() : JMath::TRandom_fast_(0) {
        permission = true;
        sMgr = this;
    };

    static void createAllRandom();
    static void deleteAllRandom();

    u32 getRandomMax(u32 max);
    void getArbitUnitVec(TVec3<f32> &, f32, f32);
    void getArbitUnitVecSimple(TVec3<f32> &, f32);
    void getArbitUnitVecXZ(TVec3<f32>&, f32);
    void setSeed(u32 seed) {
        randomSeed = seed;
        JMath::TRandom_fast_::setSeed(randomSeed);
    }

    static stRandom *sRndMgr[6];
    static stRandom *sMgr; // not sure if this get's accesssed by something else, more research needed

    s16 _0x4; // sin
    s16 _0x6; // cos
    f32 _0x8; // angle?
    u8 _0xC[4]; // unknown
    u32 randomSeed;
    bool permission;
    // padding bytes
};

stRandom * stGetRnd(u32 idx);
void stSetRndPermission(u32 idx, bool perm);

#endif // !STMATH_H
