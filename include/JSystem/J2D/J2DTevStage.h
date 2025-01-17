#ifndef  JSYSTEM_J2D_TEVSTAGE_H
#define JSYSTEM_J2D_TEVSTAGE_H

#include "dolphin/gx.h"
#include "JSystem/J2D/J2DTypes.h"
#include "macros.h"

extern const J2DIndTevStageInfo j2dDefaultIndTevStageInfo;

struct J2DTevStage {
    J2DTevStage() { 
        setTevStageInfo(j2dDefaultTevStageInfo);
        setTevSwapModeInfo(j2dDefaultTevSwapMode);
    }
    J2DTevStage(const J2DTevStageInfo& info) {
        setTevStageInfo(info);
        setTevSwapModeInfo(j2dDefaultTevSwapMode);
    }

    void setTevStageInfo(const J2DTevStageInfo &info) {
        setColorABCD(info.mColorA, info.mColorB, info.mColorC, info.mColorD);
        setTevColorOp(info.mCOp, info.mCBias, info.mCScale, info.mCClamp, info.mCReg);
        setAlphaABCD(info.mAlphaA, info.mAlphaB, info.mAlphaC, info.mAlphaD);
        setTevAlphaOp(info.mAOp, info.mABias, info.mAScale, info.mAClamp, info.mAReg);
    }

    void setStageNo(u32 param_0)
    {
        _00 = (param_0 << 1) + 0xc0;
        _04 = (param_0 << 1) + 0xc1;
    }

    void setTevSwapModeInfo(const J2DTevSwapModeInfo& swapInfo)
    {
        setTexSel(swapInfo.mTexSel);
        setRasSel(swapInfo.mRasSel);
    }

    void setTexSel(const u8 texSel) { _07 = (_07 & ~0xC) | ((texSel) << 2); }

    void setRasSel(const u8 rasSel) { _07 = (_07 & ~0x3) | rasSel; }

    void setColorABCD(const u8 a, const u8 b, const u8 c, const u8 d)
    {
        setTevColorAB(a, b);
        setTevColorCD(c, d);
    }

    void setTevColorAB(const u8 a, const u8 b) { _02 = a << 4 | b; }
    void setTevColorCD(const u8 c, const u8 d) { _03 = c << 4 | d; }

    void setTevColorOp(const u8 op, const u8 bias, const u8 scale, const u8 clamp, const u8 reg)
    {
        _01 = _01 & ~0x04 | op << 2;
        if (op <= 1) {
            _01 = _01 & ~0x30 | scale << 4;
            _01 = _01 & ~0x03 | bias;
        } else {
            _01 = _01 & ~0x30 | (op >> 1 & 3) << 4;
            _01 = _01 & ~0x03 | 3;
        }
        _01 = _01 & ~0x08 | clamp << 3;
        _01 = _01 & ~0xc0 | reg << 6;
    }

    void setAlphaABCD(const u8 a, const u8 b, const u8 c, const u8 d)
    {
        setAlphaA(a);
        setAlphaB(b);
        setAlphaC(c);
        setAlphaD(d);
    }

    void setAlphaA(const u8 a) { _06 = _06 & ~0xe0 | a << 5; }
    void setAlphaB(const u8 b) { _06 = _06 & ~0x1c | b << 2; }
    void setAlphaC(const u8 c)
    {
        _06 = _06 & ~0x03 | c >> 1;
        _07 = _07 & ~0x80 | c << 7;
    }
    void setAlphaD(const u8 d) { _07 = _07 & ~0x70 | d << 4; }
    void setTevAlphaOp(const u8 op, const u8 bias, const u8 scale, const u8 clamp, const u8 reg)
    {
        _05 = _05 & ~0x04 | op << 2;
        if (op <= 1) {
            _05 = _05 & ~0x03 | bias;
            _05 = _05 & ~0x30 | scale << 4;
        } else {
            _05 = _05 & ~0x30 | (op >> 1 & 3) << 4;
            _05 = _05 & ~0x03 | 3;
        }
        _05 = _05 & ~0x08 | clamp << 3;
        _05 = _05 & ~0xc0 | reg << 6;
    }

    u8 getColorA() { return (_02 & 0xf0) >> 4; }
    u8 getColorB() { return _02 & 0x0f; }
    u8 getColorC() { return (_03 & 0xf0) >> 4; }
    u8 getColorD() { return _03 & 0x0f; }
    u8 getAlphaA() { return (_06 & 0xe0) >> 5; }
    u8 getAlphaB() { return (_06 & 0x1c) >> 2; }
    u8 getAlphaC() { return (_06 & 0x03) << 1 | (_07 & 0x80) >> 7; }
    u8 getAlphaD() { return (_07 & 0x70) >> 4; }
    u8 getCOp()
    {
        if (getCBias() != 3) {
            return (_01 & 4) >> 2;
        }
        return ((_01 & 4) >> 2) + 8 + ((_01 & 0x30) >> 3);
    }
    u8 getCBias() { return _01 & 0x03; }
    u8 getCScale() { return (_01 & 0x30) >> 4; }
    u8 getCClamp() { return (_01 & 0x08) >> 3; }
    u8 getCReg() { return (_01 & 0xc0) >> 6; }
    u8 getAOp()
    {
        if (getABias() != 3) {
            return (_05 & 4) >> 2;
        }
        return ((_05 & 4) >> 2) + 8 + ((_05 & 0x30) >> 3);
    }
    u8 getABias() { return _05 & 0x03; }
    u8 getAScale() { return (_05 & 0x30) >> 4; }
    u8 getAClamp() { return (_05 & 0x08) >> 3; }
    u8 getAReg() { return (_05 & 0xc0) >> 6; }
    u8 getRasSel() { return _07 & 3; }
    u8 getTexSel() { return (_07 & 0x0c) >> 2; }

    void operator=(J2DTevStage const& other)
    {
        _01 = other._01;
        _02 = other._02;
        _03 = other._03;
        _05 = other._05;
        _06 = other._06;
        _07 = other._07;
    }

    u8 _00; // _00
    u8 _01; // _01
    u8 _02; // _02
    u8 _03; // _03
    u8 _04; // _04
    u8 _05; // _05
    u8 _06; // _06
    u8 _07; // _07
};

inline u32 J2DCalcIndTevStage(J2DIndTevStageInfo info)
{
    return (info.mAlphaSel << 22) | (info.mLod << 21) | (info.mPrev << 20) | (info.mMtxSel << 16) | (info.mWrapT << 11) | (info.mWrapS << 8)
         | (info.mBiasSel << 4) | (info.mIndFormat << 2) | (info.mIndStage);
}

struct J2DIndTevStage {
    J2DIndTevStage() { mFlags = J2DCalcIndTevStage(j2dDefaultIndTevStageInfo); }

    J2DIndTevStage(const J2DIndTevStageInfo& info) { mFlags = J2DCalcIndTevStage(info); }

    void load(u8);

    GXIndTexStageID getIndStage() const { return (GXIndTexStageID)u8(mFlags & 0x03); }
    GXIndTexFormat getIndFormat() const { return (GXIndTexFormat)u8((mFlags >> 2) & 0x03); }
    GXIndTexBiasSel getBiasSel() const { return (GXIndTexBiasSel)u8((mFlags >> 4) & 0x07); }
    GXIndTexWrap getWrapS() const { return (GXIndTexWrap)u8((mFlags >> 8) & 0x07); }
    GXIndTexWrap getWrapT() const { return (GXIndTexWrap)u8((mFlags >> 11) & 0x07); }
    GXIndTexMtxID getMtxSel() const { return (GXIndTexMtxID)u8((mFlags >> 16) & 0x0F); }
    GXBool getPrev() const { return (GXBool)u8((mFlags >> 20) & 0x01); }
    GXBool getLod() const { return (GXBool)u8((mFlags >> 21) & 0x01); }
    GXIndTexAlphaSel getAlphaSel() const { return (GXIndTexAlphaSel)u8((mFlags >> 22) & 0x03); }

    void setIndTevStageInfo(const J2DIndTevStageInfo& info) { mFlags = J2DCalcIndTevStage(info); }

    u32 mFlags; // _00
};



#endif
