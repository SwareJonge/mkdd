#include "Kaneshige/GhostScreen.h"
#include "Osako/system.h"

GhostScreen::GhostScreen() : mSceneTex(System::getDisplay()->getEfbWidth(), System::getDisplay()->getEfbHeight(), GX_TF_RGB565),
                             mGhostTex(System::getDisplay()->getEfbWidth(), System::getDisplay()->getEfbHeight(), GX_TF_I8)
{
    mStartX = 0;
    mStartY = 0;
    mEndX = System::getDisplay()->getEfbWidth();
    mEndY = System::getDisplay()->getEfbHeight();
    mFlush = false;
    reset();
}

void GhostScreen::reset() { mIsHide = false; }

void GhostScreen::captureScene()
{
    JUtility::TColor color(0, 0, 0, 0);
    GXSetCopyClear(color, 0xffffff);
    mSceneTex.capture(mStartX, mStartY, mSceneTex.getFormat(), false, 1);
}

void GhostScreen::captureGhost()
{
    mGhostTex.capture(mStartX, mStartY, GX_TF_A8, false, 0);
    mFlush = true;
}

void GhostScreen::draw()
{
    if (!mFlush)
        return;

    Mtx pos;

    GXSetNumChans(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);
    mSceneTex.load(GX_TEXMAP0);
    mGhostTex.load(GX_TEXMAP1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetNumTexGens(1);
    GXSetNumIndStages(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_8_8);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    PSMTXIdentity(pos);
    GXLoadPosMtxImm(pos, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_U16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3u16(mStartX, mStartY, 0);
    GXTexCoord2u8(0, 0);
    GXPosition3u16(mStartX + mEndX, mStartY, 0);
    GXTexCoord2u8(1, 0);
    GXPosition3u16(mStartX + mEndX, mStartY + mEndY, 0);
    GXTexCoord2u8(1, 1);
    GXPosition3u16(mStartX, mStartY + mEndY, 0);
    GXTexCoord2u8(0, 1);
    GXEnd();
}