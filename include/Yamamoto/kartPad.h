#ifndef YAMAMOTO_KARTPAD_H
#define YAMAMOTO_KARTPAD_H

#include "types.h"

// useless, don't take this serious
class KartPad
{
public:
    KartPad();
    void Init() {
        _4C = 0;
        _50 = 0;
        _51 = 0;
        _52 = 0;
        mItemStickY = 0;
        _40 = 0;
        _44 = 0;
        _48 = 0;
        mAccelBtn = 0x100;
        mTrigZ = 0x10;
        mBtnB = 0x200;
        mItemMask = 0x400 | 0x800;
        mBtnB2 = 0x200;
        mFaceBtnsMask = mItemMask | 0x300;
        mBtnY = 0x800;
        mTrigL = 0x40;
        mTrigR = 0x20;
        _54 = 0;
    }

    u32 mAccelBtn;
    u32 mBtnB;
    u32 mTrigZ;
    u32 mItemMask;
    u32 mFaceBtnsMask;
    u32 mBtnY;
    u32 mBtnB2;
    u32 mTrigL;
    u32 mTrigR;
    u8 _24[0x3c - 0x24];
    f32 mItemStickY;
    f32 _40;
    f32 _44;
    f32 _48;
    u32 _4C;
    u8 _50;
    u8 _51;
    u8 _52;
    u32 _54;
};

// TODO: this is incorrect, this is a global symbol, yet it has unreferenced duplicates... weird
DECL_WEAK KartPad::KartPad()
{
    mAccelBtn = 0x100;
    mTrigZ = 0x10;
    mBtnB = 0x200;
    mItemMask = 0xc00;
    mBtnB2 = 0x200;
    mFaceBtnsMask = mItemMask | 0x300;
    mBtnY = 0x800;
    mTrigL = 0x40;
    mTrigR = 0x20;
    _4C = 0;
    _50 = 0;
    _51 = 0;
    _52 = 0;
}

#endif