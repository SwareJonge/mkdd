#include <JSystem/JUtility/JUTVideo.h>

#include "Osako/shadowMgr.h"
#include "Osako/shadowScr.h"
#include "Osako/system.h"

ShadowScreen::ShadowScreen() : mTexture(JUTGetVideoManager()->getFbWidth() / 2, JUTGetVideoManager()->getEfbHeight() / 2, GX_TF_I8)
{
    mStartX = 0;
    mStartY = 0;
    mEndX = JUTGetVideoManager()->getFbWidth();
    mEndY = JUTGetVideoManager()->getEfbHeight();

    PSMTXIdentity(mPosMtx);
    C_MTXOrtho(mProjMtx, System::getOrthoT(), System::getOrthoB(), System::getOrthoL(), System::getOrthoR(), 0.0f, 1.0f);
}

void ShadowScreen::draw()
{
    ShadowManager *shadowMgr = ShadowManager::ptr();
    
    u8 mode = shadowMgr->getDepthMode();
    switch (mode)
    {
    case 1:
        mTexture.capture(0, 0, GX_CTF_A8, true, 0);
        mTexture.load(GX_TEXMAP0);
    }

    GXSetViewport(System::get2DVpX(), System::get2DVpY(), System::get2DVpW(), System::get2DVpH(), 0.0f, 1.0f);
    GXSetScissor(System::get2DScisX(), System::get2DScisY(), System::get2DScisW(), System::get2DScisH());
    GXLoadPosMtxImm(mPosMtx, 0);
    GXSetCurrentMtx(0);
    GXSetProjection(mProjMtx, GX_ORTHOGRAPHIC);
    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, shadowMgr->getShadowColor());
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    switch (mode)
    {
    case 0:
        GXSetNumTevStages(1);
        GXSetNumTexGens(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_INVDSTALPHA, GX_BL_DSTALPHA, GX_LO_NOOP);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_U16, 0);

        // Fifo Writes
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3u16(mStartX, mStartY, 0);
        GXPosition3u16(mEndX, mStartY, 0);
        GXPosition3u16(mEndX, mEndY, 0);
        GXPosition3u16(mStartX, mEndY, 0);
        GXEnd();
        break;
    case 1:
        GXColorS10 color = {0, 0, 0, (shadowMgr->getDepth(1) & 0xfc) - 0xff};

        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorS10(GX_TEVREG0, color);
        GXSetTevColor(GX_TEVREG1, shadowMgr->getShadowColor());
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C1);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);

        // Fifo Writes
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3u16(mStartX, mStartY, 0);
        GXTexCoord2u8(0, 0);
        GXPosition3u16(mEndX, mStartY, 0);
        GXTexCoord2u8(1, 0);
        GXPosition3u16(mEndX, mEndY, 0);
        GXTexCoord2u8(1, 1);
        GXPosition3u16(mStartX, mEndY, 0);
        GXTexCoord2u8(0, 1);
        GXEnd();
        break;
    }
}
