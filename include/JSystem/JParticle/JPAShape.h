#ifndef _JSYSTEM_JPA_JPASHAPE_H
#define _JSYSTEM_JPA_JPASHAPE_H

#include <dolphin/gx.h>

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

struct JPAEmitterWorkData;
struct JPABaseParticle;

struct JPAClrAnmKeyData
{
    s16 index;     // _00
    GXColor color; // _02
};

/**
 * @fabricated
 */
struct JPABaseShapeData
{
    // Representation of contents of .jpc file
    u8 mMagic[4];         // _00
    u32 mSize;            // _04
    u32 mFlags;           // _08
    s16 mClrPrmAnmOffset; // _0C
    s16 mClrEnvAnmOffset; // _0E
    f32 mBaseSizeX;       // _10
    f32 mBaseSizeY;       // _14
    u16 mBlendModeCfg;    // _18
    u8 mAlphaCompareCfg;  // _1A
    u8 mAlphaRef0;        // _1B
    u8 mAlphaRef1;        // _1C
    u8 mZModeCfg;         // _1E
    u8 mTexFlg;           // _1E
    u8 texAnmKeyNum;      // _1F
    u8 mTexIdx;           // _20
    u8 mClrFlg;           // _21
    u8 prmAnmKeyNum;      // _22
    u8 envAnmKeyNum;      // _23
    s16 mClrAnmFrmMax;    // _24
    GXColor mClrPrm;      // _26
    GXColor mClrEnv;      // _2A
    u8 mAnmRndm;          // _2E
    u8 mClrAnmRndmMask;   // _2F
    u8 mTexAnmRndmMask;   // _30
};

/**
 * @size{0x14}
 */
struct JPABaseShape
{
    JPABaseShape(const u8 *, JKRHeap *);

    void setGX(JPAEmitterWorkData *) const;

    GXBlendMode getBlendMode() const { return st_bm[pBsd->mBlendModeCfg & 0x03]; }
    GXBlendFactor getBlendSrc() const { return st_bf[(pBsd->mBlendModeCfg >> 2) & 0x0F]; }
    GXBlendFactor getBlendDst() const { return st_bf[(pBsd->mBlendModeCfg >> 6) & 0x0F]; }
    GXLogicOp getLogicOp() const { return st_lo[(pBsd->mBlendModeCfg >> 10) & 0x0F]; }
    GXBool getZCompLoc() const { return (GXBool)((pBsd->mZModeCfg >> 5) & 0x01); }

    GXBool getZEnable() const { return (GXBool)(pBsd->mZModeCfg & 0x01); }
    GXCompare getZCmp() const { return st_c[(pBsd->mZModeCfg >> 1) & 0x07]; }
    GXBool getZUpd() const { return (GXBool)((pBsd->mZModeCfg >> 4) & 0x01); }

    GXCompare getAlphaCmp0() const { return st_c[pBsd->mAlphaCompareCfg & 0x07]; }
    u8 getAlphaRef0() const { return pBsd->mAlphaRef0; }
    GXAlphaOp getAlphaOp() const { return st_ao[(pBsd->mAlphaCompareCfg >> 3) & 0x03]; }
    GXCompare getAlphaCmp1() const { return st_c[(pBsd->mAlphaCompareCfg >> 5) & 0x07]; }
    u8 getAlphaRef1() const { return pBsd->mAlphaRef1; }

    const GXTevColorArg *getTevColorArg() const { return st_ca[(pBsd->mFlags >> 0x0F) & 0x07]; }
    const GXTevAlphaArg *getTevAlphaArg() const { return st_aa[(pBsd->mFlags >> 0x12) & 0x01]; }

    u32 getType() const { return (pBsd->mFlags >> 0) & 0x0F; }
    u32 getDirType() const { return (pBsd->mFlags >> 4) & 0x07; }
    u32 getRotType() const { return (pBsd->mFlags >> 7) & 0x07; }
    u32 getBasePlaneType() const { return (pBsd->mFlags >> 10) & 0x01; }
    u32 getProjType() const { return ((pBsd->mFlags >> 24) & 0x01); }
    u32 getTilingS() const { return (pBsd->mFlags >> 25) & 0x01; }
    u32 getTilingT() const { return (pBsd->mFlags >> 26) & 0x01; }
    bool isGlblClrAnm() const { return !!(pBsd->mFlags & 0x00001000); }
    bool isGlblTexAnm() const { return !!(pBsd->mFlags & 0x00004000); }
    bool isPrjTex() const { return !!(pBsd->mFlags & 0x00100000); }
    bool isDrawFwdAhead() const { return !!(pBsd->mFlags & 0x00200000); }
    bool isDrawPrntAhead() const { return !!(pBsd->mFlags & 0x00400000); }
    bool isClipOn() const { return !!(pBsd->mFlags & 0x00800000); }
    bool isTexCrdAnm() const { return !!(pBsd->mFlags & 0x01000000); }
    bool isNoDrawParent() const { return !!(pBsd->mFlags & 0x08000000); }
    bool isNoDrawChild() const { return !!(pBsd->mFlags & 0x10000000); }

    bool isPrmAnm() const { return !!(pBsd->mClrFlg & 0x02); }
    bool isEnvAnm() const { return !!(pBsd->mClrFlg & 0x08); }
    u8 getClrAnmType() const { return (pBsd->mClrFlg >> 4) & 0x07; }
    s32 getClrAnmMaxFrm() const { return pBsd->mClrAnmFrmMax; }
    void getPrmClr(GXColor *dst) { *dst = pBsd->mClrPrm; }
    void getPrmClr(s16 idx, GXColor *dst) const { *dst = mPrmClrAnmTbl[idx]; }
    void getEnvClr(GXColor *dst) { *dst = pBsd->mClrEnv; }
    void getEnvClr(s16 idx, GXColor *dst) const { *dst = mEnvClrAnmTbl[idx]; }

    bool isTexAnm() const { return !!(pBsd->mTexFlg & 0x01); }
    u8 getTexAnmType() const { return (pBsd->mTexFlg >> 2) & 0x07; }
    s64 getTexIdx() const { return (u8)pBsd->mTexIdx; } // wtf
    u8 getTexIdx(u8 idx) const { return mTexIdxAnimTbl[idx]; }

    f32 getBaseSizeX() const { return pBsd->mBaseSizeX; }
    f32 getBaseSizeY() const { return pBsd->mBaseSizeY; }
    u8 getClrLoopOfstMask() const { return pBsd->mClrAnmRndmMask; }
    int getClrLoopOfst(u8 param_1) const { return getClrLoopOfstMask() & param_1; }
    u8 getTexLoopOfstMask() const { return pBsd->mTexAnmRndmMask; }
    u32 getTexLoopOfst(u8 param_1) const { return getTexLoopOfstMask() & param_1; }
    u8 getLoopOfstValue() const { return pBsd->mAnmRndm; }

    f32 getIncTransX() const { return ((f32 *)mTexCrdMtxAnmTbl)[5]; }
    f32 getInitTransX() const { return ((f32 *)mTexCrdMtxAnmTbl)[0]; }
    f32 getIncTransY() const { return ((f32 *)mTexCrdMtxAnmTbl)[6]; }
    f32 getInitTransY() const { return ((f32 *)mTexCrdMtxAnmTbl)[1]; }
    f32 getIncScaleX() const { return ((f32 *)mTexCrdMtxAnmTbl)[7]; }
    f32 getInitScaleX() const { return ((f32 *)mTexCrdMtxAnmTbl)[2]; }
    f32 getIncScaleY() const { return ((f32 *)mTexCrdMtxAnmTbl)[8]; }
    f32 getInitScaleY() const { return ((f32 *)mTexCrdMtxAnmTbl)[3]; }
    f32 getIncRot() const { return ((f32 *)mTexCrdMtxAnmTbl)[9]; }
    f32 getInitRot() const { return ((f32 *)mTexCrdMtxAnmTbl)[4]; }
    u8 getTexAnmKeyNum() const { return (u8)pBsd->texAnmKeyNum; }

    // Unused/inlined:
    void init_jpa(const u8 *, JKRHeap *);

    static GXBlendMode st_bm[3];
    static GXBlendFactor st_bf[10];
    static GXLogicOp st_lo[16];
    static GXCompare st_c[8];
    static GXAlphaOp st_ao[4];
    static GXTevColorArg st_ca[6][4];
    static GXTevAlphaArg st_aa[2][4];

    const JPABaseShapeData *pBsd; // _00
    const void *mTexCrdMtxAnmTbl; // _04
    const u8 *mTexIdxAnimTbl;     // _08
    GXColor *mPrmClrAnmTbl;       // _0C
    GXColor *mEnvClrAnmTbl;       // _10
};

struct JPAChildShapeData
{
    u8 mMagic[4];      // _00
    u32 mSize;         // _04
    u32 mFlags;        // _08
    f32 mPosRndm;      // _0C
    f32 mBaseVel;      // _10
    f32 mBaseVelRndm;  // _14
    f32 mVelInfRate;   // _18
    f32 mGravity;      // _1C
    f32 mScaleX;       // _20
    f32 mScaleY;       // _24
    f32 mInheritScale; // _28
    f32 mInheritAlpha; // _2C
    f32 mInheritRGB;   // _30
    GXColor mPrmClr;   // _34
    GXColor mEnvClr;   // _38
    f32 mTiming;       // _3C
    s16 mLife;         // _40
    s16 mRate;         // _44
    u8 mStep;          // _48
    u8 mTexIdx;        // _4C
    s16 mRotSpeed;     // _50
};

/**
 * @size{0x4}
 */
struct JPAChildShape
{
    JPAChildShape(const u8 *);

    // Unused/inlined:
    void init_jpa(const u8 *, JKRHeap *);

    void getPrmClr(GXColor *dst) { *dst = pBsd->mPrmClr; }
    void getEnvClr(GXColor *dst) { *dst = pBsd->mEnvClr; }
    s16 getLife() const { return pBsd->mLife; }
    s16 getRotInitSpeed() const { return pBsd->mRotSpeed; }
    s16 getRate() const { return pBsd->mRate; }
    u8 getPrmAlpha() const { return pBsd->mPrmClr.a; }

    f32 getPosRndm() const { return pBsd->mPosRndm; }
    f32 getBaseVel() const { return pBsd->mBaseVel; }
    f32 getBaseVelRndm() const { return pBsd->mBaseVelRndm; }
    f32 getVelInhRate() const { return pBsd->mVelInfRate; }
    f32 getScaleInhRate() const { return pBsd->mInheritScale; }
    f32 getColorInhRate() const { return pBsd->mInheritRGB; }
    f32 getAlphaInhRate() const { return pBsd->mInheritAlpha; }
    f32 getGravity() const { return pBsd->mGravity; }

    bool isFieldAffected() const { return pBsd->mFlags & 0x200000; }
    bool isScaleInherited() const { return pBsd->mFlags & 0x10000; }
    bool isColorInherited() const { return pBsd->mFlags & 0x40000; }
    bool isAlphaInherited() const { return pBsd->mFlags & 0x20000; }
    bool isRotateOn() const { return pBsd->mFlags & 0x1000000; }

    const JPAChildShapeData *pBsd; // _00
};

struct JPAExTexShapeData
{
    u8 mMagic[4]; // _00
    u32 mSize;    // _04

    u32 mFlags;           // _08
    f32 mIndTexMtx[2][3]; // _0C
    s8 mExpScale;         // _24
    s8 mIndTexIdx;        // _25
    s8 mSecTexIdx;        // _26
};

/**
 * @size{0x4}
 */
struct JPAExTexShape
{
    JPAExTexShape(const u8 *);

    // Unused/inlined:
    void init_jpa(const u8 *, JKRHeap *);

    const f32 *getIndTexMtx() const { return &pBsd->mIndTexMtx[0][0]; }
    s32 getExpScale() const { return pBsd->mExpScale; }
    u8 getIndTexIdx() const { return pBsd->mIndTexIdx; }
    u8 getSecTexIdx() const { return pBsd->mSecTexIdx; }
    bool isUseIndirect() const { return !!(pBsd->mFlags & 0x01); }
    bool isUseSecTex() const { return !!(pBsd->mFlags & 0x0100); }

    const JPAExTexShapeData *pBsd; // _00
};

struct JPAExtraShapeData
{
    u8 mMagic[4]; // _00
    u32 mSize;    // _04

    u32 mFlags;              // _08
    f32 mScaleInTiming;      // _0C
    f32 mScaleOutTiming;     // _10
    f32 mScaleInValueX;      // _14
    f32 mScaleOutValueX;     // _18
    f32 mScaleInValueY;      // _1C
    f32 mScaleOutValueY;     // _20
    f32 mScaleOutRandom;     // _24
    s16 mScaleAnmCycleX;     // _28
    s16 mScaleAnmCycleY;     // _2A
    f32 mAlphaInTiming;      // _2C
    f32 mAlphaOutTiming;     // _30
    f32 mAlphaInValue;       // _34
    f32 mAlphaBaseValue;     // _38
    f32 mAlphaOutValue;      // _3C
    f32 mAlphaWaveFrequency; // _40
    f32 mAlphaWaveRandom;    // _44
    f32 mAlphaWaveAmplitude; // _48
    f32 mRotateAngle;        // _4C
    f32 mRotateAngleRandom;  // _50
    f32 mRotateSpeed;        // _54
    f32 mRotateSpeedRandom;  // _58
    f32 mRotateDirection;    // _5C
};

/**
 * @size{0x1C}
 */
struct JPAExtraShape
{
    JPAExtraShape(const u8 *);

    void init();

    // Unused/inlined:
    void init_jpa(const u8 *, JKRHeap *);

    f32 getScaleInTiming() const { return pBsd->mScaleInTiming; }
    f32 getScaleOutTiming() const { return pBsd->mScaleOutTiming; }
    f32 getScaleInValueX() const { return pBsd->mScaleInValueX; }
    f32 getScaleInValueY() const { return pBsd->mScaleInValueY; }
    f32 getScaleOutValueX() const { return pBsd->mScaleOutValueX; }
    f32 getScaleOutValueY() const { return pBsd->mScaleOutValueY; }
    s16 getScaleAnmCycleX() const { return pBsd->mScaleAnmCycleX; }
    s16 getScaleAnmCycleY() const { return pBsd->mScaleAnmCycleY; }
    f32 getScaleRndm() const { return pBsd->mScaleOutRandom; }
    f32 getAlphaInTiming() const { return pBsd->mAlphaInTiming; }
    f32 getAlphaOutTiming() const { return pBsd->mAlphaOutTiming; }
    f32 getAlphaInValue() const { return pBsd->mAlphaInValue; }
    f32 getAlphaOutValue() const { return pBsd->mAlphaOutValue; }
    f32 getAlphaBaseValue() const { return pBsd->mAlphaBaseValue; }
    f32 getAlphaFreq() const { return pBsd->mAlphaWaveFrequency; }
    f32 getAlphaFreqRndm() const { return pBsd->mAlphaWaveRandom; }
    f32 getAlphaAmp() const { return pBsd->mAlphaWaveAmplitude; }
    f32 getRotateInitAngle() const { return pBsd->mRotateAngle; }
    f32 getRotateRndmAngle() const { return pBsd->mRotateAngleRandom; }
    f32 getRotateInitSpeed() const { return pBsd->mRotateSpeed; }
    f32 getRotateRndmSpeed() const { return pBsd->mRotateSpeedRandom; }
    f32 getRotateDirection() const { return pBsd->mRotateDirection; }
    f32 getScaleIncRateX() const { return mScaleIncRateX; }
    f32 getScaleDecRateX() const { return mScaleDecRateX; }
    f32 getScaleIncRateY() const { return mScaleIncRateY; }
    f32 getScaleDecRateY() const { return mScaleDecRateY; }
    f32 getAlphaIncRate() const { return mAlphaIncRate; }
    f32 getAlphaDecRate() const { return mAlphaDecRate; }

    bool isEnableScaleAnm() const { return pBsd->mFlags & 0x1; }
    bool isEnableAlphaFlick() const { return pBsd->mFlags & 0x20000; }
    bool isEnableRotateAnm() const { return pBsd->mFlags & 0x1000000; }

    const JPAExtraShapeData *pBsd; // _00
    f32 mAlphaIncRate;             // _04
    f32 mAlphaDecRate;             // _08
    f32 mScaleIncRateX;            // _0C
    f32 mScaleIncRateY;            // _10
    f32 mScaleDecRateX;            // _14
    f32 mScaleDecRateY;            // _18
};

// In JPABaseShape.cpp:
void JPASetPointSize(JPAEmitterWorkData *);
void JPASetLineWidth(JPAEmitterWorkData *);
void JPASetPointSize(JPAEmitterWorkData *, JPABaseParticle *);
void JPASetLineWidth(JPAEmitterWorkData *, JPABaseParticle *);
void JPARegistPrm(JPAEmitterWorkData *);
void JPARegistEnv(JPAEmitterWorkData *);
void JPARegistPrmEnv(JPAEmitterWorkData *);
void JPARegistAlpha(JPAEmitterWorkData *, JPABaseParticle *);
void JPARegistPrmAlpha(JPAEmitterWorkData *, JPABaseParticle *);
void JPARegistPrmAlphaEnv(JPAEmitterWorkData *, JPABaseParticle *);
void JPARegistAlphaEnv(JPAEmitterWorkData *, JPABaseParticle *);
void JPARegistEnv(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcClrIdxNormal(JPAEmitterWorkData *);
void JPACalcClrIdxNormal(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcClrIdxRepeat(JPAEmitterWorkData *);
void JPACalcClrIdxRepeat(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcClrIdxReverse(JPAEmitterWorkData *);
void JPACalcClrIdxReverse(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcClrIdxMerge(JPAEmitterWorkData *);
void JPACalcClrIdxMerge(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcClrIdxRandom(JPAEmitterWorkData *);
void JPACalcClrIdxRandom(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcPrm(JPAEmitterWorkData *);
void JPACalcPrm(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcEnv(JPAEmitterWorkData *);
void JPACalcEnv(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcColorCopy(JPAEmitterWorkData *, JPABaseParticle *);
void JPAGenTexCrdMtxIdt(JPAEmitterWorkData *);
void JPAGenTexCrdMtxAnm(JPAEmitterWorkData *);
void JPAGenTexCrdMtxPrj(JPAEmitterWorkData *);
void JPAGenCalcTexCrdMtxAnm(JPAEmitterWorkData *);
void JPALoadCalcTexCrdMtxAnm(JPAEmitterWorkData *, JPABaseParticle *);
void JPALoadTex(JPAEmitterWorkData *);
void JPALoadTexAnm(JPAEmitterWorkData *);
void JPALoadTexAnm(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcTexIdxNormal(JPAEmitterWorkData *);
void JPACalcTexIdxNormal(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcTexIdxRepeat(JPAEmitterWorkData *);
void JPACalcTexIdxRepeat(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcTexIdxReverse(JPAEmitterWorkData *);
void JPACalcTexIdxReverse(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcTexIdxMerge(JPAEmitterWorkData *);
void JPACalcTexIdxMerge(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcTexIdxRandom(JPAEmitterWorkData *);
void JPACalcTexIdxRandom(JPAEmitterWorkData *, JPABaseParticle *);
void JPALoadPosMtxCam(JPAEmitterWorkData *);
void JPADrawBillboard(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawRotBillboard(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawYBillboard(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawRotYBillboard(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawDirection(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawRotDirection(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawDBillboard(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawRotation(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawPoint(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawLine(JPAEmitterWorkData *, JPABaseParticle *);
void JPADrawStripe(JPAEmitterWorkData *);
void JPADrawStripeX(JPAEmitterWorkData *);
void JPADrawEmitterCallBackB(JPAEmitterWorkData *);
void JPADrawParticleCallBack(JPAEmitterWorkData *, JPABaseParticle *);

// In JPAChildShape.cpp:
void JPARegistChildPrmEnv(JPAEmitterWorkData *);
void JPACalcChildAlphaOut(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcChildScaleOut(JPAEmitterWorkData *, JPABaseParticle *);

// In JPAExtraShape.cpp:
void JPACalcScaleX(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleY(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleCopy(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleAnmNormal(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleAnmRepeatX(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleAnmRepeatY(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleAnmReverseX(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcScaleAnmReverseY(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcAlphaAnm(JPAEmitterWorkData *, JPABaseParticle *);
void JPACalcAlphaFlickAnm(JPAEmitterWorkData *, JPABaseParticle *);

// In JPAExTexShape.cpp:
void JPALoadExTex(JPAEmitterWorkData *);

#endif
