#ifndef _JSYSTEM_J2D_J2DTEXMTX_H
#define _JSYSTEM_J2D_J2DTEXMTX_H

#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include "types.h"

struct J2DTextureSRTInfo {
    f32 mScaleX;       // _00
    f32 mScaleY;       // _04
    f32 mRotationDeg;  // _08, rotation in degrees
    f32 mTranslationX; // _0C
    f32 mTranslationY; // _10
};

struct J2DTexMtxInfo
{
    enum DCCType
    { // from TP
        DCC_None = 0,
        DCC_Maya = 1,
    };

    void operator=(const J2DTexMtxInfo& other)
    {
        mCenter         = other.mCenter;
        mTexMtxType     = other.mTexMtxType;
        mTexMtxDCC      = other.mTexMtxDCC;
        mTextureSRTInfo = other.mTextureSRTInfo;
    }

    GXTexMtxType getTexMtxType() const { return (GXTexMtxType)mTexMtxType; }

    u8 mTexMtxType;                    // _00
    u8 mTexMtxDCC;                     // _01
    u16 _02;                           // _02. padding
    Vec mCenter;                       // _04
    J2DTextureSRTInfo mTextureSRTInfo; // _10
};

extern J2DTexMtxInfo const j2dDefaultTexMtxInfo;

/**
 * @size{0x54}
 */
struct J2DTexMtx
{
    J2DTexMtx() { mInfo = j2dDefaultTexMtxInfo; }
    /* @fabricated */
    inline J2DTexMtx(J2DTexMtxInfo *info)
        : mInfo(*info)
    {
    }

    J2DTexMtx(const J2DTexMtxInfo &otherInfo) {
        mInfo = otherInfo;
    }

    ~J2DTexMtx() {}
    

    void load(u32);
    void calc();
    void getTextureMtx(const J2DTextureSRTInfo &, Vec, Mtx);
    void getTextureMtxMaya(const J2DTextureSRTInfo &, Mtx);

    J2DTexMtxInfo &getTexMtxInfo() { return mInfo; }
    void setTexMtxInfo(J2DTexMtxInfo info) { mInfo = info; }

    J2DTexMtxInfo mInfo; // _00
    Mtx mMtx;            // _24
};

#endif
