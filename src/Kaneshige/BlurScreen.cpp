#include "Kaneshige/Blurscreen.h"
#include "Osako/system.h"

BlurScreen::BlurScreen() : mBlurTex(System::getDisplay()->getEfbWidth() / 2, System::getDisplay()->getEfbHeight() / 2, GX_TF_RGB565),
                           mSceneTex(System::getDisplay()->getEfbWidth() / 2, System::getDisplay()->getEfbHeight() / 2, GX_TF_RGB565)
{
    mStartX = 0;
    mStartY = 0;
    mEndX = System::getDisplay()->getEfbWidth();
    mEndY = System::getDisplay()->getEfbHeight();
    mFlushBlur = false;
    mFlush = false;
}

void BlurScreen::draw(const JUtility::TColor &color, u8 tevAlpha, u8 a, f32 addX, f32 addY)
{
    J2DOrthoGraph ortho(mStartX, mStartY, mEndX, mEndY, -1.0f, 1.0f);
    ortho.setPort();

    if(mFlushBlur && !mFlush) {
        J2DPicture picture(&mBlurTex);
        Mtx pos;
        picture.setCornerColor(color);
        picture.setAlpha(a);
        picture.draw(mStartX + addX, mStartY + addY, mEndX, mEndY, false, false, false);
        mSceneTex.load(GX_TEXMAP0);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
        GXSetNumTexGens(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, JUtility::TColor(0xff, 0xff, 0xff, tevAlpha));
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumTevStages(1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumIndStages(0);
        PSMTXIdentity(pos);
        GXLoadPosMtxImm(pos, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(mStartX, mStartY, 0);
        GXTexCoord2u8(0, 0);
        GXPosition3s16(mStartX + mEndX, mStartY, 0);
        GXTexCoord2u8(1, 0);
        GXPosition3s16(mStartX + mEndX, mStartY + mEndY, 0);
        GXTexCoord2u8(1, 1);
        GXPosition3s16(mStartX, mStartY + mEndY, 0);
        GXTexCoord2u8(0, 1);
        GXEnd();
    }

    mFlush = false;
}

void BlurScreen::captureScene()
{
    mSceneTex.capture(mStartX, mStartY, mSceneTex.getFormat(), true, 1);
}

void BlurScreen::captureBlur()
{
    mBlurTex.capture(mStartX, mStartY, mBlurTex.getFormat(), true, 0);
    mFlushBlur = true;
}