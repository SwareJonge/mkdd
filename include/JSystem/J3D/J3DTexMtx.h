#ifndef _JSYSTEM_J3D_J3DTEXMTX_H
#define _JSYSTEM_J3D_J3DTEXMTX_H

#include <dolphin/mtx.h>
#include "JSystem/J3D/J3DPacket.h"
#include "JSystem/J3D/J3DTexGenBlock.h"
#include "JSystem/J3D/J3DTypes.h"
#include "types.h"

struct J3DTexMtxInfo
{
    void operator=(J3DTexMtxInfo const &other)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mEffectMtx[i][j] = other.mEffectMtx[i][j];
            }
        }
        mCenter = other.mCenter;
        mProjection = other.mProjection;
        mInfo = other.mInfo;
        mSRT = other.mSRT;
    }
    void setEffectMtx(Mtx m)
    {
        mEffectMtx[0][0] = m[0][0];
        mEffectMtx[0][1] = m[0][1];
        mEffectMtx[0][2] = m[0][2];
        mEffectMtx[0][3] = m[0][3];
        mEffectMtx[1][0] = m[1][0];
        mEffectMtx[1][1] = m[1][1];
        mEffectMtx[1][2] = m[1][2];
        mEffectMtx[1][3] = m[1][3];
        mEffectMtx[2][0] = m[2][0];
        mEffectMtx[2][1] = m[2][1];
        mEffectMtx[2][2] = m[2][2];
        mEffectMtx[2][3] = m[2][3];
        mEffectMtx[3][2] = 0.0f;
        mEffectMtx[3][1] = 0.0f;
        mEffectMtx[3][0] = 0.0f;
        mEffectMtx[3][3] = 1.0f;
    }

    /* 0x00 */ u8 mProjection;
    /* 0x01 */ u8 mInfo;
    /* 0x02 */ u8 padding[2];
    /* 0x04 */ Vec mCenter;
    /* 0x10 */ J3DTextureSRTInfo mSRT;
    /* 0x24 */ Mtx44 mEffectMtx;
}; // Size: 0x64

extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;

class J3DTexMtx
{
public:
    J3DTexMtx() { mTexMtxInfo = j3dDefaultTexMtxInfo; }
    J3DTexMtx(const J3DTexMtxInfo &info)
    {
        mTexMtxInfo = info;
    }
    ~J3DTexMtx() {}
    /*void load(u32 i) const
    {
        GDOverflowCheck(53);
        J3DGDLoadTexMtxImm((Mtx &)mMtx, i * 3 + 30, (GXTexMtxType)mTexMtxInfo.mProjection);
    }*/
    void calc();
    void calc(const Mtx m);
    void calcTexMtx(f32 const (*)[4]);
    void calcPostTexMtx(f32 const (*)[4]);
    void loadTexMtx(u32) const;
    void loadPostTexMtx(u32) const;

    J3DTexMtxInfo &getTexMtxInfo() { return mTexMtxInfo; }
    Mtx &getMtx() { return mMtx; }
    void setEffectMtx(Mtx effectMtx) { mTexMtxInfo.setEffectMtx(effectMtx); }
    Mtx &getViewMtx() { return mViewMtx; }
    //void setViewMtx(const Mtx viewMtx) { MTXCopy(viewMtx, mViewMtx); }

private:
    /* 0x00 */ J3DTexMtxInfo mTexMtxInfo;
    /* 0x64 */ Mtx mMtx;
    /* 0x94 */ Mtx mViewMtx;
}; // Size: 0xc4

namespace J3DDifferedTexMtx {
void loadExecute(const f32 (*)[4]);
extern J3DTexGenBlock* sTexGenBlock;
extern J3DShapePacket_0x24* sTexMtxObj;
} // namespace J3DDifferedTexMtx

#endif
