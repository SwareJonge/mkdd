#ifndef _JSYSTEM_J2D_J2DMATERIAL_H
#define _JSYSTEM_J2D_J2DMATERIAL_H

#include "types.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DMatBlock.h"

struct J2DPane;

/**
 * @size{0x88}
 */
struct J2DMaterial {
    struct J2DMaterialAnmPointer {
        J2DMaterialAnmPointer()
        {
            mColorAnm    = nullptr;
            mColorIds[0] = 0xFFFF;

            mSRTAnm    = nullptr;
            for (int i = 0; i < 8; i++) {
                mSRTIds[i] = 0xFFFF;
            }

            mPatternAnm    = nullptr;
            for (int i = 0; i < 8; i++) {
                mPatternIds[i] = 0xFFFF;
            }

            mTevAnm        = nullptr;
            for (int i = 0; i < 4; i++) {
                mTevCRegIds[i] = 0xFFFF;
            }

            for (int i = 0; i < 4; i++) {
                mTevKRegIds[i] = 0xFFFF;
            }
        }

        J2DAnmColor* mColorAnm;        // _00
        J2DAnmTextureSRTKey* mSRTAnm;  // _04
        J2DAnmTexPattern* mPatternAnm; // _08
        J2DAnmTevRegKey* mTevAnm;      // _0C
        u16 mColorIds[1];              // _10
        u16 mSRTIds[8];                // _12
        u16 mPatternIds[8];            // _22
        u16 mTevCRegIds[4];            // _32
        u16 mTevKRegIds[4];            // _3A
    };

    J2DMaterial();
    J2DMaterial(u32);

    virtual ~J2DMaterial(); // _08

    void animation();
    void makeAnmPointer();
    void setAnimation(J2DAnmColor*);
    void setAnimation(J2DAnmTevRegKey*);
    void setAnimation(J2DAnmTextureSRTKey*);
    void setAnimation(J2DAnmTexPattern*);
    void setGX();

    static J2DTevBlock* createTevBlock(int, bool);
    static J2DIndBlock* createIndBlock(int, bool);

    u32 getIndex() const { return mIndex; }
    J2DColorBlock* getColorBlock() { return &mColorBlock; }
    J2DTexGenBlock* getTexGenBlock() { return &mTexGenBlock; }
    J2DPEBlock* getPEBlock() { return &mPeBlock; }
    J2DTevBlock* getTevBlock() { return mTevBlock; }
    J2DIndBlock* getIndBlock() { return mIndBlock; }
    bool isVisible() const { return mIsVisible; }
    int getMaterialAlphaCalc() const { return mMaterialAlphaCalc; }

    // _00 = VTBL
    J2DPane* mPane;                 // _04
    int mMaterialMode;              // _08
    u16 mIndex;                     // _0C
    u8 mIsVisible;                  // _0E
    u8 mMaterialAlphaCalc;          // _0F
    J2DColorBlock mColorBlock;      // _10
    J2DTexGenBlock mTexGenBlock;    // _28
    J2DTevBlock* mTevBlock;         // _70
    J2DIndBlock* mIndBlock;         // _74
    J2DPEBlock mPeBlock;            // _78
    J2DMaterialAnmPointer* mAnmPtr; // _84
};

#endif
