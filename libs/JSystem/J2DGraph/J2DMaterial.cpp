#include "dolphin/gx.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DMatBlock.h"
#include "JSystem/J2D/J2DMaterial.h"
#include "JSystem/J2D/J2DTypes.h"
#include "JSystem/J3D/J3DTypes.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J3D/J3DAnmTexPattern.h"
#include "JSystem/J3D/J3DAnmTevRegKey.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/gx/GXEnum.h"
#include "types.h"

J2DMaterial::J2DMaterial()
    : mColorBlock()
    , mTexGenBlock()
    , mPeBlock()
{
    mPane         = nullptr;
    mIndex        = -1;
    mTevBlock     = nullptr;
    mIndBlock     = nullptr;
    mAnmPtr       = nullptr;
    mMaterialMode = 1;
    mIsVisible    = 1;
}

J2DMaterial::J2DMaterial(u32 flag)
{
    mPane              = nullptr;
    mIndex             = -1;
    mTevBlock          = createTevBlock((flag >> 16) & 0x1F, true);
    mIndBlock          = createIndBlock((flag >> 24) & 0x1, true);
    mAnmPtr            = nullptr;
    mMaterialMode      = 1;
    mIsVisible         = 1;
    mMaterialAlphaCalc = 1;
}

J2DMaterial::~J2DMaterial()
{
    delete mTevBlock;
    delete mIndBlock;
    delete mAnmPtr;
}

void J2DMaterial::setGX()
{
    if (!mIsVisible) {
        return;
    }

    mColorBlock.setGX();
    mTexGenBlock.setGX();

    if (mTevBlock) {
        mTevBlock->setGX();
    }

    if (mIndBlock) {
        mIndBlock->setGX();
    }

    mPeBlock.setGX();
}

J2DTevBlock* J2DMaterial::createTevBlock(int stageCount, bool p2)
{
    if (p2) {
        if (stageCount <= 1) {
            return new J2DTevBlock1();
        }
        if (stageCount == 2) {
            return new J2DTevBlock2();
        }
        if (stageCount <= 4) {
            return new J2DTevBlock4();
        }
        if (stageCount <= 8) {
            return new J2DTevBlock8();
        }
        return new J2DTevBlock16();
    }
    if (stageCount <= 1) {
        return new (-4) J2DTevBlock1();
    }
    if (stageCount == 2) {
        return new (-4) J2DTevBlock2();
    }
    if (stageCount <= 4) {
        return new (-4) J2DTevBlock4();
    }
    if (stageCount <= 8) {
        return new (-4) J2DTevBlock8();
    }
    return new (-4) J2DTevBlock16();
}

J2DIndBlock* J2DMaterial::createIndBlock(int stageCount, bool p2)
{
    if (p2) {
        if (stageCount != 0) {
            return new J2DIndBlockFull();
        }
        return new J2DIndBlockNull();
    }
    if (stageCount != 0) {
        return new (-4) J2DIndBlockFull();
    }
    return new (-4) J2DIndBlockNull();
}

void J2DMaterial::makeAnmPointer()
{
    if (!mAnmPtr) {
        mAnmPtr = new J2DMaterialAnmPointer;
    }
}

void J2DMaterial::setAnimation(J2DAnmColor* animation)
{
    if (animation || mAnmPtr) {
        makeAnmPointer();
        mAnmPtr->mColorAnm    = animation;
        mAnmPtr->mColorIds[0] = -1;

        if (animation) {
            u32 count = animation->getUpdateMaterialNum();
            u16 index = mIndex;
            for (u16 i = 0; i < count; i++) {
                if (index == animation->getUpdateMaterialID(i)) {
                    mAnmPtr->mColorIds[0] = i;
                    return;
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTextureSRTKey* animation)
{
    if (animation || mAnmPtr) {
        makeAnmPointer();
        mAnmPtr->mSRTAnm = animation;
        for (int i = 0; i < 8; i++) {
            mAnmPtr->mSRTIds[i] = -1;
        }

        if (animation) {
            u32 count = animation->getUpdateMaterialNum();
            u32 index = getIndex();
            for (u16 i = 0; i < (u16)count; i++) {
                if ((u16)index == animation->getUpdateMaterialID(i)) {
                    mAnmPtr->mSRTIds[animation->getUpdateTexMtxID(i)] = i;
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTexPattern* animation)
{
    if (animation || mAnmPtr) {
        makeAnmPointer();
        mAnmPtr->mPatternAnm = animation;
        for (int i = 0; i < 8; i++) {
            mAnmPtr->mPatternIds[i] = -1;
        }

        if (animation) {
            u32 count                        = animation->getUpdateMaterialNum();
            u32 index                        = getIndex();
            J3DAnmTexPatternFullTable* table = animation->getAnmTable();
            for (u16 i = 0; i < count; i++) {
                if (index == animation->getUpdateMaterialID(i)) {
                    mAnmPtr->mPatternIds[((u8*)(table->mData[(u32)i * 2]))[4]] = i; // NB: need to fix up this struct eventually
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTevRegKey* animation)
{
    if (animation || mAnmPtr) {
        makeAnmPointer();
        mAnmPtr->mTevAnm = animation;
        for (int i = 0; i < 4; i++) {
            mAnmPtr->mTevCRegIds[i] = -1;
        }

        if (animation) {
            u32 cCount = animation->getCRegUpdateMaterialNum();
            u32 cIndex = getIndex();
            for (u16 i = 0; i < cCount; i++) {
                if (cIndex == animation->getCRegUpdateMaterialID(i)) {
                    mAnmPtr->mTevCRegIds[((u8*)&(animation->getAnmCRegKeyTable()[i].mTables[4]))[0]] = i;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            mAnmPtr->mTevKRegIds[i] = -1;
        }

        if (animation) {

            u32 kCount = animation->getKRegUpdateMaterialNum();
            u32 kIndex = getIndex();
            for (u16 i = 0; i < kCount; i++) {
                if (kIndex == animation->getKRegUpdateMaterialID(i)) {
                    mAnmPtr->mTevKRegIds[((u8*)&(animation->getAnmKRegKeyTable()[i].mTables[4]))[0]] = i;
                }
            }
        }
    }
}

void J2DMaterial::animation()
{
    if (!mAnmPtr) {
        return;
    }

    if (mAnmPtr->mColorAnm && mAnmPtr->mColorIds[0] != 0xFFFF) {
        GXColor color;
        mAnmPtr->mColorAnm->getColor(mAnmPtr->mColorIds[0], &color);
        getColorBlock()->setMatColor(0, color);
    }

    if (mAnmPtr->mSRTAnm) {
        for (u8 i = 0; i < 8; i++) {
            if (mAnmPtr->mSRTIds[i] != 0xFFFF) {
                J3DTextureSRTInfo info3D;
                mAnmPtr->mSRTAnm->getTransform(mAnmPtr->mSRTIds[i], &info3D);
                J2DTextureSRTInfo info2D;
                info2D.mScaleX       = info3D.mScaleX;
                info2D.mScaleY       = info3D.mScaleY;
                info2D.mRotationDeg  = (360.0f * f32((u16)info3D.mRotation)) / 65535.0f;
                info2D.mTranslationX = info3D.mTranslationX;
                info2D.mTranslationY = info3D.mTranslationY;

                J2DTexMtx texMtx;
                getTexGenBlock()->getTexMtx(i, texMtx);

                texMtx.getTexMtxInfo().mTextureSRTInfo = info2D;
                getTexGenBlock()->setTexMtx(i, texMtx);
                getTexGenBlock()->getTexCoord(i).setTexGenMtx(i * 3 + 30);
            }
        }
    }

    if (getTevBlock() && mAnmPtr->mPatternAnm) {
        for (u8 i = 0; i < 8; i++) {
            if (mAnmPtr->mPatternIds[i] != 0xFFFF) {
                JUTTexture* texture = getTevBlock()->getTexture(i);
                if (!texture) {
                    continue;
                }

                u16 idx = mAnmPtr->mPatternIds[i];
                u16 texNo;
                mAnmPtr->mPatternAnm->getTexNo(idx, &texNo);
                getTevBlock()->setTexNo(i, texNo);

                ResTIMG* img = mAnmPtr->mPatternAnm->getResTIMG(idx);
                if (texture->getTexInfo() != img) {
                    JUTPalette* palette = nullptr;
                    GXTlut tlut            = GX_TLUT0;
                    if (img->mPaletteFormat != 0) {
                        palette = mAnmPtr->mPatternAnm->getPalette(idx);
                        if (palette->getNumColors() > 256) {
                            tlut = (GXTlut)(i % 4 + 16);
                        } else {
                            tlut = (GXTlut)i;
                        }
                    }

                    texture->storeTIMG(img, palette, tlut);
                }
            }
        }
    }

    if (getTevBlock() && mAnmPtr->mTevAnm) {
        for (u8 i = 0; i < 4; i++) {
            if (mAnmPtr->mTevCRegIds[i] != 0xFFFF) {
                J2DGXColorS10 color;
                mAnmPtr->mTevAnm->getTevColorReg(mAnmPtr->mTevCRegIds[i], &color);
                getTevBlock()->setTevColor(i, color);
            }
        }

        for (u8 i = 0; i < 4; i++) {
            u16 idx = mAnmPtr->mTevKRegIds[i];
            if (idx != 0xFFFF) {
                JUTColor konstColor;
                mAnmPtr->mTevAnm->getTevKonstReg(idx, &konstColor);
                getTevBlock()->setTevKColor(i, konstColor);
            }
        }
    }
}
