#include "Kaneshige/ShimmerScreen.h"
#include "Osako/system.h"

GXIndTexScale ShimmerScreen::sIndTexScaleS;
GXIndTexScale ShimmerScreen::sIndTexScaleT;
u8 ShimmerScreen::sIndReplace;

u8 ShimmerScreen::sIndSinedOfs = 1;
s16 ShimmerScreen::sIndScaleExp = -6;

ShimmerScreen::ShimmerScreen(ResTIMG *img) : mIndImg(img),
                                             mTex1(System::getDisplay()->getEfbWidth() / 2, System::getDisplay()->getEfbHeight() / 2, GX_TF_RGB565),
                                             mTex2(System::getDisplay()->getEfbWidth() / 2, System::getDisplay()->getEfbHeight() / 2, GX_TF_IA8),
                                             mTex3(img, 0)

{
    _0 = false;

    // Kaneshige memes strike again
    for (int i = 0; i < NUM_LEVELS; i++)
    {
        s32 z;
        u8 blendRate;
        switch (i)
        {
        case 0:
            z = 0xf80;
            blendRate = 0x80;
            break;
        default:
            z = 0xfc0;
            blendRate = 0xff;
            break;
        }

        mOffsetZ[i] = z;
        mBlendRate[i] = blendRate;
    }
    // mOffsetZ[0] = 0xf80;
    // mOffsetZ[1] = 0xfc0;
    // mBlendRate[0] = 0x80;
    // mBlendRate[1] = 0xff;

    mStartX = 0;
    mStartY = 0;
    mEndX = mTex1.getWidth() * 2;
    mEndY = mTex1.getHeight() * 2;

    mFlush = false;
    _e0[0][0] = 0.4f;
    _e0[0][1] = 0.0f;
    _e0[0][2] = 0.0f;
    _e0[1][0] = 0.0f;
    _e0[1][1] = 0.2f;
    _e0[1][2] = 0.0f;
    PSMTXIdentity(mIndMtx);
    PSMTXScale(mIndMtx, 10.0f, 15.0f, 1.0f);
    _128 = 0;
    _12a = 16;
}

void ShimmerScreen::calc()
{
    mIndMtx[0][3] -= 0.0075f;
    if (mIndMtx[0][3] < -1.0f)
        mIndMtx[0][3] = 1.0f;

    mIndMtx[1][3] += 0.0075f;
    if (mIndMtx[1][3] < -1.0f)
        mIndMtx[1][3] = 1.0f;

    _128 += _12a;
}

void ShimmerScreen::capture()
{
    GXSetColorUpdate(0);
    GXSetAlphaUpdate(0);

    mTex1.capture(mStartX, mStartY, mTex1.getFormat(), true, 0);
    mTex2.capture(mStartX, mStartY, GX_TF_Z16, true, 0);

    GXSetColorUpdate(1);
    mFlush = true;
}

void ShimmerScreen::draw()
{
    capture();
    drawInit();

    for (int i = 0; i < NUM_LEVELS; i++)
        levelDraw(i);

    drawEnd();
}

void ShimmerScreen::drawInit()
{
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);

    mTex1.load(GX_TEXMAP0);

    if (isIndActive())
        mTex3.load(GX_TEXMAP1);

    mTex2.load(GX_TEXMAP2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);

    if (isIndActive())
    {
        GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e);
        GXSetNumTexGens(2);
    }
    else
    {
        GXSetNumTexGens(1);
    }

    GXSetNumTevStages(3);

    if (isIndActive())
        GXSetNumIndStages(1);
    else
        GXSetNumIndStages(0);
}

void ShimmerScreen::drawEnd()
{
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

void ShimmerScreen::levelDraw(int level)
{
#line 405
    JUT_MINMAX_ASSERT(0, level, NUM_LEVELS)
    Mtx pos;

    bool invalidIdx = (1 - level) == 0;
    u32 tev1RG = ((mOffsetZ[level] | 0xf000) - 1);
    u32 tev2RG;
    if (invalidIdx)
        tev2RG = 0xffff;
    else
        tev2RG = ((mOffsetZ[level + 1]) | 0xf000);

    JUtility::TColor color1(tev1RG & 0xff, (tev1RG >> 8) & 0xff, 0, 0);
    GXSetTevColor(GX_TEVREG0, color1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_8_8);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_C0, GX_CC_ONE, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVREG0);

    JUtility::TColor color2(tev2RG & 0xff, (tev2RG >> 8) & 0xff, 0, mBlendRate[level]);
    GXSetTevColor(GX_TEVREG1, color2);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_8_8);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP1);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    if (!invalidIdx)
    {
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A1, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    }
    else
    {
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A1);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    }

    if (isIndActive())
    {
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
        GXSetIndTexCoordScale(GX_INDTEXSTAGE0, sIndTexScaleS, sIndTexScaleT);
        GXSetTevIndWarp(GX_TEVSTAGE2, GX_INDTEXSTAGE0, sIndSinedOfs, sIndReplace, GX_ITM_0);
    }

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    PSMTXIdentity(pos);
    GXLoadPosMtxImm(pos, 0);
    GXLoadTexMtxImm(mIndMtx, 0x1e, GX_MTX2x4);
    GXSetCurrentMtx(0);
    GXSetIndTexMtx(GX_ITM_0, _e0, sIndScaleExp);
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