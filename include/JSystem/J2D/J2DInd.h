#ifndef _JSYSTEM_J2D_J2DINDBLOCK_H
#define _JSYSTEM_J2D_J2DINDBLOCK_H

#include "dolphin/mtx.h"
#include "dolphin/gx.h"

#include "JSystem/JBlockType.h"
#include "types.h"

struct J2DIndTexCoordScaleInfo {

    GXIndTexScale getScaleS() const { return (GXIndTexScale)mScaleS; }
    GXIndTexScale getScaleT() const { return (GXIndTexScale)mScaleT; }

    J2DIndTexCoordScaleInfo& operator=(const J2DIndTexCoordScaleInfo& other)
    {
        mScaleS = other.mScaleS;
        mScaleT = other.mScaleT;
        return *this;
    }

    u8 mScaleS; // _00
    u8 mScaleT; // _01
};

extern const J2DIndTexCoordScaleInfo j2dDefaultIndTexCoordScaleInfo;

/**
 * @size{0x2}
 */
struct J2DIndTexCoordScale {
    J2DIndTexCoordScale() { mScaleInfo = j2dDefaultIndTexCoordScaleInfo; }

    J2DIndTexCoordScale(const J2DIndTexCoordScaleInfo& info) { mScaleInfo = info; }

    ~J2DIndTexCoordScale() { }

    void load(u8);

    J2DIndTexCoordScaleInfo mScaleInfo; // _00
};

struct J2DIndTexMtxInfo {
    Mtx23 mMtx; // _00
    s8 mScale;  // _18

    J2DIndTexMtxInfo& operator=(const J2DIndTexMtxInfo& other)
    {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                mMtx[i][j] = other.mMtx[i][j];
            }
        }
        mScale = other.mScale;
        return *this;
    }
};

extern const J2DIndTexMtxInfo j2dDefaultIndTexMtxInfo;

/**
 * @size{0x1C}
 */
struct J2DIndTexMtx {
    J2DIndTexMtx() { mMtxInfo = j2dDefaultIndTexMtxInfo; }

    J2DIndTexMtx(const J2DIndTexMtxInfo& info) { mMtxInfo = info; }

    ~J2DIndTexMtx() { }

    void load(u8);

    J2DIndTexMtxInfo mMtxInfo; // _00
};

struct J2DIndTexOrderInfo {

    J2DIndTexOrderInfo& operator=(const J2DIndTexOrderInfo& other)
    {
        mTexCoordID = other.mTexCoordID;
        mTexMapID   = other.mTexMapID;
        return *this;
    }

    GXTexCoordID getTexCoordID() const { return (GXTexCoordID)mTexCoordID; }
    GXTexMapID getTexMapID() const { return (GXTexMapID)mTexMapID; }

    u8 mTexCoordID; // _00
    u8 mTexMapID;   // _01
};

extern const J2DIndTexOrderInfo j2dDefaultIndTexOrderNull;

/**
 * @size{0x2}
 */
struct J2DIndTexOrder {
    J2DIndTexOrder() { mOrder = j2dDefaultIndTexOrderNull; }
    J2DIndTexOrder(const J2DIndTexOrderInfo& info) { mOrder = info; }
    void load(u8);

    J2DIndTexOrderInfo mOrder; // _00
};


#endif
