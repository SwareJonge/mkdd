#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DMatBlock.h"
#include "JSystem/J2D/J2DMaterial.h"
#include "JSystem/J2D/J2DTypes.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/TColor.h"
#include "types.h"


J2DWindowEx::J2DWindowEx(J2DPane* parent, JSURandomInputStream* input, u32 flags, J2DMaterial* materials)
{
    mAnmVisibility = nullptr;
    mAnmVtxColor   = nullptr;
    int position   = input->getPosition();
    int uStack_88[2];
    input->read(uStack_88, 8);
    mBloBlockType = uStack_88[0];
    int iVar2     = input->getPosition();
    int auStack_90[2];
    input->peek(auStack_90, 8);
    makePaneExStream(parent, input);
    input->seek(iVar2 + auStack_90[1], SEEK_SET);
    J2DWindowData windowData;
    input->read(&windowData, sizeof(J2DWindowData));

    JUTColor* colors[4] = { nullptr };
    colors[0]                   = &mContentsColorA;
    colors[1]                   = &mContentsColorB;
    colors[2]                   = &mContentsColorC;
    colors[3]                   = &mContentsColorD;

    for (int i = 0; i < 4; i++) {
        _158[i]            = windowData.mContentIds[i];
        mFrameMaterials[i] = nullptr;
        if (_158[i] != 0xffff) {
            mFrameMaterials[i]           = materials + _158[i];
            (materials + _158[i])->mPane = this;
        }
        _168[i]      = windowData._28[i];
        *(colors[i]) = JUTColor(windowData.mContentColors[i]);
    }

    mWrapFlags = windowData._18;
    mWindowArea.set(windowData.mMinX, windowData.mMinY, windowData.mMinX + windowData.mOffsetX, windowData.mMinY + windowData.mOffsetY);
    _166              = windowData._22;
    mMaterialID       = windowData.mParentId;
    mContentsMaterial = nullptr;

    if (mMaterialID != 0xffff) {
        mContentsMaterial            = &materials[mMaterialID];
        materials[mMaterialID].mPane = this;
    }

    input->seek(position + uStack_88[1], SEEK_SET);
    rewriteAlpha();
    mFrameTextureA   = nullptr;
    mFrameTextureB   = nullptr;
    mFrameTextureC   = nullptr;
    mFrameTextureD   = nullptr;
    mPalette         = nullptr;
    mContentsTexture = nullptr;
    mMaterialFlags   = 0;
    setMinSize();
}

void J2DWindowEx::initialize(u32, const ResTIMG**, const JGeometry::TBox2f* box)
{
    // this is just here to spawn weak functions and put shit in rodata
    mFrameMaterials[0]->getTevBlock()->setTevStageNum(0);
    mFrameMaterials[0]->getTevBlock()->setTevKColorSel(0, 0);
    mFrameMaterials[0]->getTevBlock()->setTevKAlphaSel(0, 0);
    mFrameMaterials[0]->getTevBlock()->insertTexture(0, (const ResTIMG*)nullptr);
    J2DTevOrder order;
    mFrameMaterials[0]->getTevBlock()->setTevOrder(0, order);
    mFrameMaterials[0]->getTevBlock()->getTevStage(0);
    mWindowArea.i.x = 0.0f; // something has to use 0.0f around here

    const u8 slack[] = { // this should be 8 arrays of 0x10
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
}

void J2DWindowEx::setMinSize()
{
    mMinWidth  = 1;
    mMinHeight = 1;
    if (mFrameMaterials[0] && mFrameMaterials[1] && mFrameMaterials[2] && mFrameMaterials[3] && mFrameMaterials[0]->mTevBlock
        && mFrameMaterials[1]->mTevBlock && mFrameMaterials[2]->mTevBlock && mFrameMaterials[3]->mTevBlock
        && mFrameMaterials[0]->mTevBlock->getTexture(0) && mFrameMaterials[1]->mTevBlock->getTexture(0)
        && mFrameMaterials[2]->mTevBlock->getTexture(0) && mFrameMaterials[3]->mTevBlock->getTexture(0)) {
        mMinWidth  = mFrameMaterials[0]->mTevBlock->getTexture(0)->getWidth() + mFrameMaterials[1]->mTevBlock->getTexture(0)->getWidth();
        mMinHeight = mFrameMaterials[0]->mTevBlock->getTexture(0)->getHeight() + mFrameMaterials[2]->mTevBlock->getTexture(0)->getHeight();
    }
}

J2DWindowEx::~J2DWindowEx()
{
    for (u8 i = 0; i < 4; i++) {
        if (mMaterialFlags & (1 << i)) {
            delete mFrameMaterials[i];
        }
    }

    if (mMaterialFlags & 0x10) {
        delete mContentsMaterial;
    }
}

void J2DWindowEx::drawSelf(f32 x, f32 y, Mtx* texMtx)
{
    JGeometry::TBox2f box = mBounds;
    box.addPos(x, y);
    Mtx v1;
    PSMTXConcat(*texMtx, mGlobalMtx, v1);
    GXLoadPosMtxImm(v1, 0);
    draw_private(box, mWindowArea);
    clip(mWindowArea);
}

void J2DWindowEx::draw_private(const JGeometry::TBox2<f32>& p1, const JGeometry::TBox2<f32>& p2)
{
    if (p1.getWidth() >= mMinWidth && p1.getHeight() >= mMinHeight) {
        JUTTexture* textures[4];
        bool anyFrameMaterialNull = false;
        for (int i = 0; i < 4; i++) {
            if (mFrameMaterials[i] == nullptr) {
                return;
            }
            J2DTevBlock* block = mFrameMaterials[i]->getTevBlock();
            if (block == nullptr) {
                return;
            }

            textures[i] = block->getTexture(0);
            if (textures[i] == nullptr) {
                anyFrameMaterialNull = true;
            }
        }

        JGeometry::TBox2<f32> aTStack_b8(p2);
        aTStack_b8.addPos(p1.i);
        drawContents(aTStack_b8);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        if (!anyFrameMaterialNull) {
            f32 dVar16 = p1.i.x;
            f32 dVar15 = p1.i.y;
            f32 dVar18 = p1.f.x - textures[3]->getWidth();
            f32 dVar17 = p1.f.y - textures[3]->getHeight();
            f32 dVar14 = dVar16 + textures[0]->getWidth();
            f32 dVar12 = dVar15 + textures[0]->getHeight();
            u16 local_c4;
            if (mWrapFlags & 0x80) {
                local_c4 = 0;
            } else {
                local_c4 = 0x8000;
            }
            u16 local_c6;
            if (mWrapFlags & 0x40) {
                local_c6 = 0;
            } else {
                local_c6 = 0x8000;
            }

            drawFrameTexture(dVar16, dVar15, textures[0]->getWidth(), textures[0]->getHeight(), local_c4, local_c6, 0x8000 - local_c4,
                             0x8000 - local_c6, mFrameMaterials[0], true);
            bool r9 = mFrameMaterials[1] != mFrameMaterials[0];
            u16 local_c8;
            if (mWrapFlags & 0x20) {
                local_c8 = 0;
            } else {
                local_c8 = 0x8000;
            }
            u16 local_ca;
            if (mWrapFlags & 0x10) {
                local_ca = 0;
            } else {
                local_ca = 0x8000;
            }
            drawFrameTexture(dVar18, dVar15, textures[3]->getWidth(), textures[0]->getHeight(), local_c8, local_ca, 0x8000 - local_c8,
                             0x8000 - local_ca, mFrameMaterials[1], r9);

            u16 local_cc;
            if (mWrapFlags & 0x20) {
                local_cc = 0x8000;
            } else {
                local_cc = 0;
            }

            u16 local_ce;
            if (mWrapFlags & 0x10) {
                local_ce = 0;
            } else {
                local_ce = 0x8000;
            }
            drawFrameTexture(dVar14, dVar15, dVar18 - dVar14, textures[0]->getHeight(), local_cc, local_ce, local_cc, local_ce ^ 0x8000,
                             mFrameMaterials[1], false);

            r9 = mFrameMaterials[3] != mFrameMaterials[1];
            u16 local_d0;
            if (mWrapFlags & 2) {
                local_d0 = 0;
            } else {
                local_d0 = 0x8000;
            }
            u16 local_d2;
            if (mWrapFlags & 1) {
                local_d2 = 0;
            } else {
                local_d2 = 0x8000;
            }

            drawFrameTexture(dVar18, dVar17, textures[3]->getWidth(), textures[3]->getHeight(), local_d0, local_d2, 0x8000 - local_d0,
                             0x8000 - local_d2, mFrameMaterials[3], r9);

            u16 local_d4;
            if (mWrapFlags & 2) {
                local_d4 = 0x8000;
            } else {
                local_d4 = 0;
            }

            u16 local_d6;
            if (mWrapFlags & 1) {
                local_d6 = 0;
            } else {
                local_d6 = 0x8000;
            }
            drawFrameTexture(dVar14, dVar17, dVar18 - dVar14, textures[3]->getHeight(), local_d4, local_d6, local_d4, local_d6 ^ 0x8000,
                             mFrameMaterials[3], false);

            u16 local_d8;
            if (mWrapFlags & 2) {
                local_d8 = 0;
            } else {
                local_d8 = 0x8000;
            }

            u16 local_da;
            if (mWrapFlags & 1) {
                local_da = 0x8000;
            } else {
                local_da = 0;
            }
            drawFrameTexture(dVar18, dVar12, textures[3]->getWidth(), dVar17 - dVar12, local_d8, local_da, local_d8 ^ 0x8000, local_da,
                             mFrameMaterials[3], false);

            r9 = mFrameMaterials[2] != mFrameMaterials[3];
            u16 local_dc;
            if (mWrapFlags & 8) {
                local_dc = 0;
            } else {
                local_dc = 0x8000;
            }

            u16 local_de;
            if (mWrapFlags & 4) {
                local_de = 0;
            } else {
                local_de = 0x8000;
            }
            drawFrameTexture(dVar16, dVar17, textures[0]->getWidth(), textures[3]->getHeight(), local_dc, local_de, 0x8000 - local_dc,
                             0x8000 - local_de, mFrameMaterials[2], r9);

            u16 local_e0;
            if (mWrapFlags & 8) {
                local_e0 = 0;
            } else {
                local_e0 = 0x8000;
            }

            u16 local_e2;
            if (mWrapFlags & 4) {
                local_e2 = 0x8000;
            } else {
                local_e2 = 0;
            }
            drawFrameTexture(dVar16, dVar12, textures[0]->getWidth(), dVar17 - dVar12, local_e0, local_e2, local_e0 ^ 0x8000, local_e2,
                             mFrameMaterials[2], false);
        }

        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetNumTexGens(0);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}

void J2DWindowEx::drawContents(const JGeometry::TBox2<f32>& p1)
{
    if (!p1.isValid() || mContentsMaterial == nullptr) {
        return;
    }
    if (!mContentsMaterial->isVisible()) {
        return;
    }
    mContentsMaterial->setGX();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    JUTColor TStack_b4(mContentsColorA);
    JUTColor TStack_bc(mContentsColorC);
    JUTColor TStack_b8(mContentsColorB);
    JUTColor TStack_c0(mContentsColorD);
    if (mContentsMaterial->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
        if (mContentsMaterial->getMaterialAlphaCalc() == 1) {
            TStack_b4.a = (TStack_b4.a * mColorAlpha) / 0xff;
            TStack_bc.a = (TStack_bc.a * mColorAlpha) / 0xff;
            TStack_b8.a = (TStack_b8.a * mColorAlpha) / 0xff;
            TStack_c0.a = (TStack_c0.a * mColorAlpha) / 0xff;
        }
    } else if (mIsInfluencedAlpha) {
        GXSetChanMatColor(GX_ALPHA0, JUTColor(mColorAlpha));
    }
    bool bVar5 = false;
    f32 in_f31;
    f32 in_f30;
    f32 in_f29;
    f32 in_f28;
    if (mContentsMaterial->getTevBlock() != nullptr) {
        if (mContentsMaterial->getTevBlock()->getTexture(0)) {
            bVar5 = true;
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            f32 dVar15 = mContentsMaterial->getTevBlock()->getTexture(0)->getWidth();
            f32 dVar14 = mContentsMaterial->getTevBlock()->getTexture(0)->getHeight();

            f32 dVar12 = p1.getWidth();
            f32 dVar13 = p1.getHeight();
            in_f31     = -(dVar12 / dVar15 - 1.0f) / 2;
            in_f30     = -((dVar13 / dVar14) - 1.0f) / 2;
            in_f29     = in_f31 + (dVar12 / dVar15);
            in_f28     = in_f30 + (dVar13 / dVar14);
        }
    }
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    f32 zero = 0.0f;
    GXPosition3f32(p1.i.x, p1.i.y, zero);
    GXColor1u32(TStack_b4);
    if (bVar5) {
        GXTexCoord2f32(in_f31, in_f30);
    }
    GXPosition3f32(p1.f.x, p1.i.y, zero);
    GXColor1u32(TStack_b8);
    if (bVar5) {
        GXTexCoord2f32(in_f29, in_f30);
    }
    GXPosition3f32(p1.f.x, p1.f.y, zero);
    GXColor1u32(TStack_c0);
    if (bVar5) {
        GXTexCoord2f32(in_f29, in_f28);
    }
    GXPosition3f32(p1.i.x, p1.f.y, zero);
    GXColor1u32(TStack_bc);
    if (bVar5) {
        GXTexCoord2f32(in_f31, in_f28);
    }
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
}

void J2DWindowEx::drawFrameTexture(f32 x, f32 y, f32 width, f32 height, u16 p5, u16 p6, u16 p7, u16 p8, J2DMaterial* material, bool p10)
{
    if (material != nullptr && material->isVisible()) {
        f32 dVar15 = x + width;
        f32 dVar14 = y + height;
        if (p10) {
            material->setGX();
        }
        JUTColor aTStack_84(0xffffffff);
        J2DPane* parentPane = getParentPane();
        if (material->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
            if (material->getMaterialAlphaCalc() == 1) {
                u8 uVar11 = 0xff;
                if (material->getColorBlock()->getMatColor(0) != 0) {
                    uVar11 = material->getColorBlock()->getMatColor(0)->a;
                }
                if (parentPane != nullptr && mIsInfluencedAlpha != 0) {
                    uVar11 = ((uVar11 * parentPane->mColorAlpha) / 0xff);
                }
                aTStack_84 = JUTColor((u32)uVar11 | 0xffffff00);
            }
        } else if (parentPane != nullptr && mIsInfluencedAlpha != 0 && p10) {
            int matColorAlpha = material->getColorBlock()->getMatColor(0)->a;
            int colorAlpha    = parentPane->mColorAlpha;
            GXSetChanMatColor(GX_ALPHA0, JUTColor((matColorAlpha * colorAlpha / 0xff) & 0xff));
        }
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        f32 zero = 0.0f;
        GXPosition3f32(x, y, zero);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(p7, p8);

        GXPosition3f32(dVar15, y, zero);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(p5, p8);

        GXPosition3f32(dVar15, dVar14, zero);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(p5, p6);

        GXPosition3f32(x, dVar14, zero);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(p7, p6);
        GXEnd();
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    }
}

void J2DWindowEx::draw(const JGeometry::TBox2f& p1)
{
    bool isMissingTexture = false;
    JUTTexture* local_68[4];
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterials[i] == nullptr) {
            return;
        }
        if (mFrameMaterials[i]->getTevBlock() == nullptr) {
            return;
        }
        local_68[i] = mFrameMaterials[i]->getTevBlock()->getTexture(0);
        if (local_68[i] == nullptr) {
            isMissingTexture = true;
        }
    }
    JGeometry::TBox2<f32> aTStack_78;
    if (!isMissingTexture) {
        aTStack_78.set(local_68[0]->getWidth(), local_68[0]->getHeight(), p1.getWidth() - local_68[1]->getWidth(),
                       p1.getHeight() - local_68[2]->getHeight());
    } else {
        aTStack_78.set(0.0f, 0.0f, p1.getWidth(), p1.getHeight());
    }
    draw(p1, aTStack_78);
}

void J2DWindowEx::draw(const JGeometry::TBox2<f32>& p1, const JGeometry::TBox2<f32>& p2)
{
    rewriteAlpha();
    mColorAlpha = mAlpha;
    makeMatrix(p1.i.x, p1.i.y, 0.0f, 0.0f);
    GXLoadPosMtxImm(mPositionMtx, 0);
    GXSetCurrentMtx(0);
    draw_private(JGeometry::TBox2f(0.0f, 0.0f, p1.getWidth(), p1.getHeight()), p2);
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
    GXSetNumIndStages(0);
    for (int i = 0; i < 0x10; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    Mtx v1;
    PSMTXIdentity(v1);
    GXLoadPosMtxImm(v1, 0);
}

void J2DWindowEx::setTevOrder(bool p1)
{
    u16 infoData[2];
    if (!p1) {
        infoData[0] = 4;
        infoData[1] = 0xffff;
    } else {
        infoData[0] = 0xff;
        infoData[1] = 0xff04;
    }
    for (u8 i = 0; i < 2; i++) {
        J2DTevOrderInfo info;
        info.mTexCoord = infoData[i] >> 8;
        info.mTexMap   = infoData[i] >> 8;
        info.mColor    = infoData[i] & 0xFF;
        J2DTevOrder local_30(info);
        for (int j = 0; j < 4; j++) {
            if ((u8)mFrameMaterials[j]->getTevBlock()->getMaxStage() > i) {
                mFrameMaterials[j]->getTevBlock()->setTevOrder(i, local_30);
            }
        }
    }
}

void J2DWindowEx::setTevStage(bool p1)
{
    for (int i = 0; i < 4; i++) {
        J2DTevStage* stage = mFrameMaterials[i]->getTevBlock()->getTevStage(0);
        JUTTexture* tex    = mFrameMaterials[i]->getTevBlock()->getTexture(0);
        bool bVar1          = false;
        if (tex != nullptr && ((s32)tex->getFormat() == 0 || (s32)tex->getFormat() == 1) && tex->getTransparency() == 0) {
            bVar1 = true;
        }
        if (!p1) {
            setStage(stage, bVar1 ? STAGE_Unk1 : STAGE_Unk0);
        } else {
            setStage(stage, bVar1 ? STAGE_Unk4 : STAGE_Unk3);
            stage = mFrameMaterials[i]->getTevBlock()->getTevStage(1);
            setStage(stage, STAGE_Unk2);
        }
    }
}

void J2DWindowEx::setStage(J2DTevStage* stage, J2DWindowEx::stage_enum stageNum)
{
    const u8 colorData[6][4] = {
        { 0x0f, 0x08, 0x0a, 0x0f }, { 0x0f, 0x08, 0x0a, 0x0f }, { 0x0f, 0x0a, 0x00, 0x0f },
        { 0x02, 0x04, 0x08, 0x0f }, { 0x02, 0x04, 0x08, 0x0f }, { 0x0f, 0x0f, 0x0f, 0x0a },
    };
    const u8 alphaData[6][4] = {
        { 0x07, 0x04, 0x05, 0x07 }, { 0x05, 0x07, 0x07, 0x07 }, { 0x07, 0x05, 0x00, 0x07 },
        { 0x01, 0x02, 0x04, 0x07 }, { 0x07, 0x07, 0x07, 0x02 }, { 0x07, 0x07, 0x07, 0x05 },
    };
    const u8 opData[6][5] = {
        { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 },
    };

    stage->setTevColorAB(colorData[stageNum][0], colorData[stageNum][1]);
    stage->setTevColorCD(colorData[stageNum][2], colorData[stageNum][3]);
    stage->setTevColorOp(opData[stageNum][0], opData[stageNum][1], opData[stageNum][2], opData[stageNum][3], opData[stageNum][4]);
    stage->setAlphaABCD(alphaData[stageNum][0], alphaData[stageNum][1], alphaData[stageNum][2], alphaData[stageNum][3]);
    stage->setTevAlphaOp(opData[stageNum][0], opData[stageNum][1], opData[stageNum][2], opData[stageNum][3], opData[stageNum][4]);
}

bool J2DWindowEx::setBlack(JUTColor black)
{
    JUTColor oldBlack, oldWhite;

    if (!getBlackWhite(&oldBlack, &oldWhite)) {
        return false;
    }

    return setBlackWhite(black, oldWhite);
}

bool J2DWindowEx::setWhite(JUTColor white)
{
    JUTColor oldBlack, oldWhite;

    if (!getBlackWhite(&oldBlack, &oldWhite)) {
        return false;
    }

    return setBlackWhite(oldBlack, white);
}

bool J2DWindowEx::setBlackWhite(JUTColor black, JUTColor white)
{
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterials[i] == nullptr) {
            return false;
        }

        if (mFrameMaterials[i]->getTevBlock() == nullptr) {
            return false;
        }
    }

    if (!isSetBlackWhite(black, white)) {
        return false;
    }

    bool bVar1 = (black != 0 || white != 0xffffffff);
    u8 uVar3 = bVar1 ? 2 : 1;
    for (int i = 0; i < 4; i++) {
        mFrameMaterials[i]->getTevBlock()->setTevStageNum(uVar3);
    }
    setTevOrder(bVar1);
    setTevStage(bVar1);
    if (bVar1) {
        J2DGXColorS10 color0;
        J2DGXColorS10 color1;
        color0.r = black.r;
        color0.g = black.g;
        color0.b = black.b;
        color0.a = black.a;
        color1.r = white.r;
        color1.g = white.g;
        color1.b = white.b;
        color1.a = white.a;

        for (int i = 0; i < 4; i++) {
            mFrameMaterials[i]->getTevBlock()->setTevColor(0, color0);
            mFrameMaterials[i]->getTevBlock()->setTevColor(1, color1);
        }
    }
    return true;
}

bool J2DWindowEx::getBlackWhite(JUTColor* outBlack, JUTColor* outWhite) const
{
    if (mFrameMaterials[0] == nullptr) {
        return false;
    }

    if (mFrameMaterials[0]->getTevBlock() == nullptr) {
        return false;
    }

    bool cVar6 = mFrameMaterials[0]->getTevBlock()->getTevStageNum() != 1;
    *outBlack  = JUTColor(0);
    *outWhite  = JUTColor(0xffffffff);
    if (cVar6) {
        J2DGXColorS10* color0p = mFrameMaterials[0]->getTevBlock()->getTevColor(0);
        GXColorS10 color0;
        color0.r               = color0p->r;
        color0.g               = color0p->g;
        color0.b               = color0p->b;
        color0.a               = color0p->a;
        J2DGXColorS10* color1p = mFrameMaterials[0]->getTevBlock()->getTevColor(1);
        GXColorS10 color1;
        color1.r  = color1p->r;
        color1.g  = color1p->g;
        color1.b  = color1p->b;
        color1.a  = color1p->a;
        *outBlack = JUTColor((((u8)color0.r) << 0x18) | (((u8)color0.g) << 0x10) | (((u8)color0.b) << 0x8) | (((u8)color0.a)));
        *outWhite = JUTColor((((u8)color1.r) << 0x18) | (((u8)color1.g) << 0x10) | (((u8)color1.b) << 0x8) | (((u8)color1.a)));
    }
    return true;
}

bool J2DWindowEx::isSetBlackWhite(JUTColor black, JUTColor white) const
{
    if (black == 0 && white == 0xffffffff) {
        return true;
    }
    for (int i = 0; i < 4; i++) {
        if ((u8)mFrameMaterials[i]->getTevBlock()->getMaxStage() == 1) {
            return false;
        }
    }
    return true;
}

JUTColor J2DWindowEx::getBlack() const
{
    JUTColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return TCOLOR_BLACK_U32;
    }

    return black;
}

JUTColor J2DWindowEx::getWhite() const
{
    JUTColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return TCOLOR_WHITE_U32;
    }

    return white;
}

void J2DWindowEx::setAlpha(u8 alpha)
{
    mAlpha = alpha;
    J2DColorBlock* block;
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterials[i] && (block = &mFrameMaterials[i]->mColorBlock)) {
            block->mColors[0].a = alpha;
        }
    }
    if (mContentsMaterial && (block = &mContentsMaterial->mColorBlock)) {
        block->mColors[0].a = alpha;
    }
}

void J2DWindowEx::setCullBack(_GXCullMode cullMode)
{
    mCullMode = cullMode;

    for (int i = 0; i < 4; i++) {
        if (mFrameMaterials[i] != nullptr) {
            mFrameMaterials[i]->mColorBlock.mCullMode = cullMode;
        }
    }
    if (mContentsMaterial != nullptr) {
        mContentsMaterial->mColorBlock.mCullMode = cullMode;
    }
    J2DPane::setCullBack(cullMode);
}

void J2DWindowEx::rewriteAlpha()
{
    if (mContentsMaterial != nullptr) {
        mAlpha = mContentsMaterial->mColorBlock.mColors[0].a;
    }
}

JUTTexture* J2DWindowEx::getFrameTexture(u8 frameMaterialIndex, u8 textureIndex) const
{
    J2DMaterial* material = getFrameMaterial(frameMaterialIndex);
    if (material != nullptr && material->mTevBlock != nullptr) {
        return material->mTevBlock->getTexture(textureIndex);
    }
    return nullptr;
}

JUTTexture* J2DWindowEx::getContentsTexture(u8 textureIndex) const
{
    J2DMaterial* material = getContentsMaterial();
    if (material != nullptr && material->mTevBlock != nullptr) {
        return material->mTevBlock->getTexture(textureIndex);
    }
    return nullptr;
}

bool J2DWindowEx::isUsed(const ResTIMG* resource)
{
    for (u8 i = 0; i < 4; i++) {
        if (mFrameMaterials[i] != nullptr && mFrameMaterials[i]->mTevBlock != nullptr) {
            for (u32 j = 0; j < 8; j++) {
                JUTTexture* texture = mFrameMaterials[i]->mTevBlock->getTexture(j);
                if (texture != nullptr && texture->mTexInfo == resource) {
                    return true;
                }
            }
        }
    }
    if (mContentsMaterial != nullptr && mContentsMaterial->mTevBlock != nullptr) {
        for (u32 j = 0; j < 8; j++) {
            JUTTexture* texture = mContentsMaterial->mTevBlock->getTexture(j);
            if (texture != nullptr && texture->mTexInfo == resource) {
                return true;
            }
        }
    }
    return J2DPane::isUsed(resource);
}

void J2DWindowEx::setAnimation(J2DAnmColor* animation)
{
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterials[i]->setAnimation(animation);
        }
    }
    if (mContentsMaterial != nullptr) {
        mContentsMaterial->setAnimation(animation);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTextureSRTKey* animation)
{
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterials[i]->setAnimation(animation);
        }
    }
    if (mContentsMaterial != nullptr) {
        mContentsMaterial->setAnimation(animation);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTexPattern* animation)
{
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterials[i]->setAnimation(animation);
        }
    }
    if (mContentsMaterial != nullptr) {
        mContentsMaterial->setAnimation(animation);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTevRegKey* animation)
{
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterials[i]->setAnimation(animation);
        }
    }
    if (mContentsMaterial != nullptr) {
        mContentsMaterial->setAnimation(animation);
    }
}

bool J2DWindowEx::isNeedSetAnm(u8 frameMaterialIndex)
{
    for (u8 i = 0; i < frameMaterialIndex; i++) {
        if (mFrameMaterials[i] == mFrameMaterials[frameMaterialIndex]) {
            return false;
        }
    }
    return (mFrameMaterials[frameMaterialIndex] != nullptr);
}

void J2DWindowEx::setAnimation(J2DAnmVisibilityFull* animation) { mAnmVisibility = animation; }

void J2DWindowEx::setAnimation(J2DAnmVtxColor* animation)
{
    mAnmVtxColor = animation;
    _17C         = 0;
    if (animation != nullptr) {
        u32 uVar3 = animation->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if (_168[i] != 0xffff) {
                for (u16 j = 0; j < uVar3; j++) {
                    J3DAnmVtxColorIndexData* puVar1 = animation->getAnmVtxColorIndexData(0, j);
                    u16* indexPointer               = animation->getVtxColorIndexPointer(0);
                    u16* indexPointer2              = indexPointer + (u32)puVar1->mData;
                    for (u16 k = 0; k < puVar1->mNum; k++) {
                        if (indexPointer2[k] == _168[i]) {
                            mAnmVtxColor = animation;
                            _17C |= 1 << i;
                            goto nexti;
                        }
                    }
                }
            }
        nexti:;
        }
    }

    if (_17C == 0) {
        mAnmVtxColor = nullptr;
    }
}

const J2DAnmTransform* J2DWindowEx::animationPane(const J2DAnmTransform* animation)
{
    if (mAnmVisibility != 0 && _166 != 0xffff) {
        u8 visibility;
        mAnmVisibility->getVisibility(_166, &visibility);
        if (visibility) {
            J2DPane::show();
        } else {
            J2DPane::hide();
        }
    }

    JUTColor* local_38[4] = { nullptr };
    local_38[0]                   = &mContentsColorA;
    local_38[1]                   = &mContentsColorB;
    local_38[2]                   = &mContentsColorC;
    local_38[3]                   = &mContentsColorD;
    if (mAnmVtxColor != nullptr) {
        u32 uVar3 = mAnmVtxColor->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if ((_17C & (1 << i))) {
                for (u16 j = 0; j < uVar3; j++) {
                    J3DAnmVtxColorIndexData* data = mAnmVtxColor->getAnmVtxColorIndexData(0, j);
                    u16* indexPointer             = mAnmVtxColor->getVtxColorIndexPointer(0);
                    u16* indexPointer2            = indexPointer + (u32)data->mData;
                    for (u16 k = 0; k < data->mNum; k++) {
                        if (indexPointer2[k] == _168[i]) {
                            mAnmVtxColor->getColor(0, j, local_38[i]);
                            goto nexti;
                        }
                    }
                }
            }
        nexti:;
        }
    }
    return J2DPane::animationPane(animation);
}
