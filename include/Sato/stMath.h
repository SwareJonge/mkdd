#ifndef STMATH_H
#define STMATH_H

#include "types.h"

#include "JSystem/JGeometry.h"

using namespace JGeometry;

inline float stAbs(float x) {
    if(x >= 0.0f) {
        return x;
    }
    return -x;
}

struct stPlaneParam
{
    float x, y, z; // might be TVec3f
    float angle; // dot productor
};

int stVecNormalize(TVec3f &);
int stVecNormalize(TVec3f *);
void stClampVecMax(TVec3f &, float);
void stClampVecMin(TVec3f &, float);
float stspeedy_sqrtf(register float x);
float stLength2(float, float);
void stMakeDirectionMtx(TMtx34f *, const TVec3f &, char);
void stMakeRMtx(Mtx, const TVec3f &, const TVec3f &, const TVec3f &);
void stQt2Mtx(Mtx, const Quaternion *);
void stVec2QtUpdate(Quaternion &, Quaternion &, const TVec3f &, const TVec3f &);
void stVec2QtUpdate(Quaternion &, const Vec &, const Vec &);
bool stVec2Qt(Quaternion &, const Vec &, const Vec &);
void stMtx2Qt(Quaternion *, const Mtx);
void stQtLerp(Quaternion *, const Quaternion *, const Quaternion *, float);
f32 stQtNormalize(Quaternion *, const Quaternion *);
int stMakePlaneParam(stPlaneParam &, TVec3f &, const TVec3f &);
int stMakePlaneParam(stPlaneParam &, const TVec3f &, const TVec3f &, const TVec3f &);
void stSearchInSurface(const TVec3f &, const TVec3f &, const TVec3f &);
void stSearchInSurface(const TVec3f &, const stPlaneParam &);
void stCollideSurfaceAndSphere(const TVec3f &, float, const stPlaneParam &, float &);
float stCollideLineToPlaneIn(const TVec3f &, const TVec3f &, const stPlaneParam &);
TVec3f stGetCollidePosFromT(const TVec3f &, const TVec3f &, float);
float stGetCollideDepthFromT(const TVec3f &, const TVec3f &, float);
void stMTXRotDeg(Mtx, char, float);
void stMTXRotRad(Mtx, char, float);

struct stRandom : public JMath::TRandom_fast_
{
    stRandom() : JMath::TRandom_fast_(0) {
        permission = true;
        sMgr = this;
    };

    ~stRandom() { }

    static void createAllRandom();
    static void deleteAllRandom();

    u32 getRandomMax(u32 max);
    void getArbitUnitVec(TVec3<float> &, float, float);
    void getArbitUnitVecSimple(TVec3<float> &, float);
    void getArbitUnitVecXZ(TVec3<float>&, float);
    void setSeed(u32 seed) {
        randomSeed = seed;
        JMath::TRandom_fast_::setSeed(randomSeed);
    }

    static stRandom *sRndMgr[6];
    static stRandom *sMgr; // not sure if this get's accesssed by something else, more research needed

    s16 _0x4; // sin
    s16 _0x6; // cos
    float _0x8; // angle?
    u8 _0xC[4]; // unknown
    u32 randomSeed;
    bool permission;
    // padding bytes
};

stRandom * stGetRnd(u32 idx);
void stSetRndPermission(u32 idx, bool perm);

#endif // !STMATH_H
