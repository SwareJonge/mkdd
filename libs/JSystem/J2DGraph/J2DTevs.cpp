#include "dolphin/gx.h"
#include "math.h"
#include "std/math.h"
#include "JSystem/J2D/J2DMatBlock.h"
#include "JSystem/J2D/J2DTexMtx.h"
#include "JSystem/J2D/J2DTevStage.h"
#include "types.h"

const J2DTexCoordInfo j2dDefaultTexCoordInfo[8] = {
    {
        GX_TG_MTX2x4,
        GX_TG_TEX0,
        GX_IDENTITY,
    }, // Tex 0
    {
        GX_TG_MTX2x4,
        GX_TG_TEX1,
        GX_IDENTITY,
    }, // Tex 1
    {
            GX_TG_MTX2x4,
        GX_TG_TEX2,
        GX_IDENTITY,
    }, // Tex 2
    {
            GX_TG_MTX2x4,
        GX_TG_TEX3,
        GX_IDENTITY,
    }, // Tex 3
    {
            GX_TG_MTX2x4,
        GX_TG_TEX4,
        GX_IDENTITY,
    }, // Tex 4
    {
            GX_TG_MTX2x4,
        GX_TG_TEX5,
        GX_IDENTITY,
    }, // Tex 5
    {
            GX_TG_MTX2x4,
        GX_TG_TEX6,
        GX_IDENTITY,
    }, // Tex 6
    {
            GX_TG_MTX2x4,
        GX_TG_TEX7,
        GX_IDENTITY,
    }, // Tex 7
};

const J2DTexMtxInfo j2dDefaultTexMtxInfo = {
        GX_TG_MTX2x4,            // Tex gen type
    J2DTexMtxInfo::DCC_Maya, // Tex gen DCC
    0xFFFF,                  // padding
    { 0.5f, 0.5f, 0.0f },    // Center
    {
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
    }, // SRT info
};

const J2DIndTexMtxInfo j2dDefaultIndTexMtxInfo = {
    {
        0.5f,
        0.0f,
        0.0f, // 2x3 mtx
        0.0f,
        0.5f,
        0.0f,
    },
    1, // scale exp
};

const J2DTevStageInfo j2dDefaultTevStageInfo = {
    4, 10, 15, 15, 0, 0, 0, 0, 1, 0, // Color
    5, 7,  7,  0,  0, 0, 0, 1, 0,    // Alpha
};

const J2DIndTevStageInfo j2dDefaultIndTevStageInfo = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void J2DTexMtx::load(u32 size) { GXLoadTexMtxImm(mMtx, size * 3 + 30, (GXTexMtxType)mInfo.getTexMtxType()); }

void J2DTexMtx::calc()
{
    if (mInfo.mTexMtxDCC == 0) {
        getTextureMtx(mInfo.mTextureSRTInfo, mInfo.mCenter, mMtx);
    } else if (mInfo.mTexMtxDCC == 1) {
        getTextureMtxMaya(mInfo.mTextureSRTInfo, mMtx);
    }
}

void J2DTexMtx::getTextureMtx(const J2DTextureSRTInfo& info, Vec pos, Mtx mtx)
{
    f32 theta = (info.mRotationDeg * F_PI) / 180.0f;
    mtx[0][0] = info.mScaleX * cosf(theta);
    mtx[0][1] = -info.mScaleX * sinf(theta);
    mtx[0][2] = 0.0f;
    mtx[0][3] = -info.mScaleX * cosf(theta) * pos.x + pos.y * (info.mScaleX * sinf(theta)) + pos.x + info.mTranslationX;

    mtx[1][0] = info.mScaleY * sinf(theta);
    mtx[1][1] = info.mScaleY * cosf(theta);
    mtx[1][2] = 0.0f;
    mtx[1][3] = -info.mScaleY * sinf(theta) * pos.x - pos.y * (info.mScaleY * cosf(theta)) + pos.y + info.mTranslationY;

    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[2][3] = 0.0f;
}

void J2DTexMtx::getTextureMtxMaya(const J2DTextureSRTInfo& info, Mtx mtx)
{
    f32 theta = (info.mRotationDeg * F_PI) / 180.0f;
    mtx[0][0] = info.mScaleX * cosf(theta);
    mtx[0][1] = info.mScaleY * sinf(theta);
    mtx[0][2] = 0.0f;
    mtx[0][3] = (info.mTranslationX - 0.5f) * cosf(theta) - (info.mTranslationY - 0.5f + info.mScaleY) * sinf(theta) + 0.5f;

    mtx[1][0] = -info.mScaleX * sinf(theta);
    mtx[1][1] = info.mScaleY * cosf(theta);
    mtx[1][2] = 0.0f;
    mtx[1][3] = -(info.mTranslationX - 0.5f) * sinf(theta) - (info.mTranslationY - 0.5f + info.mScaleY) * cosf(theta) + 0.5f;

    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[2][3] = 0.0f;
}

void J2DIndTevStage::load(u8 tevStage)
{
    GXSetTevIndirect((GXTevStageID)tevStage, getIndStage(), getIndFormat(), getBiasSel(), getMtxSel(), getWrapS(), getWrapT(), getPrev(),
                     getLod(), getAlphaSel());
}

void J2DIndTexMtx::load(u8 mtxID) { GXSetIndTexMtx((GXIndTexMtxID)(mtxID + 1), mMtxInfo.mMtx, mMtxInfo.mScale); }

void J2DIndTexCoordScale::load(u8 stage) { GXSetIndTexCoordScale((GXIndTexStageID)stage, mScaleInfo.getScaleS(), mScaleInfo.getScaleT()); }

void J2DIndTexOrder::load(u8 stage) { GXSetIndTexOrder((GXIndTexStageID)stage, mOrder.getTexCoordID(), mOrder.getTexMapID()); }

const GXColor j2dDefaultColInfo                              = { 0xFF, 0xFF, 0xFF, 0xFF };
const J2DTevOrderInfo j2dDefaultTevOrderInfoNull             = { 0xFF, 0xFF, 0xFF, 0 };
const J2DIndTexOrderInfo j2dDefaultIndTexOrderNull           = { 0xFF, 0xFF };
const GXColorS10 j2dDefaultTevColor                          = { 0xFF, 0xFF, 0xFF, 0xFF };
const J2DIndTexCoordScaleInfo j2dDefaultIndTexCoordScaleInfo = {
    0,
    0,
};
const GXColor j2dDefaultTevKColor                        = { 0xFF, 0xFF, 0xFF, 0xFF };
const J2DTevSwapModeInfo j2dDefaultTevSwapMode           = { 0, 0 };
const J2DTevSwapModeTableInfo j2dDefaultTevSwapModeTable = { 0, 1, 2, 3 };
const J2DBlendInfo j2dDefaultBlendInfo                   = { 1, 4, 5, 5 };
const u8 j2dDefaultDither                                = 0;
const J2DColorChanInfo j2dDefaultColorChanInfo           = { 0, 3 };
const u8 j2dDefaultTevSwapTable                          = 0x1B;
const u16 j2dDefaultAlphaCmp                             = 0xE7;
