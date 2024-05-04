#include "Osako/system.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

f32 TWipeWaveA::sWipeStartPos = 0.0f;
f32 TWipeWaveA::sWipeMoveRateMax = 0.6667f;
f32 TWipeWaveA::sWipeHeadWidth = 74;
f32 TWipeWaveA::sWipeHeadHeight = 32;
f32 TWipeWaveA::sAlphaMinRate = 0.0f;
f32 TWipeWaveA::sAlphaMaxRate = 1.0f;

TWipeWaveA::TWipeWaveA(JKRHeap *heap) : TWipeBase(heap) { init(); }

TWipeWaveA::~TWipeWaveA() {}

void TWipeWaveA::init()
{
    for (int i = 0; i < 4; i++)
        mOffsetY[i] = 0.0f;
}

void TWipeWaveA::calcMain(int camNo, f32 rate, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, camNo);
    mGraph[camNo]->place(viewport);

    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

    f32 booboo = System::get2DVpH();
    f32 r = sWipeHeadHeight * (viewport.getWidth() / System::get2DVpW());
    f32 offset = r + viewport.getHeight();
    f32 moveRate = SiUtil::getNormalRange(rate, 0.0f, sWipeMoveRateMax);
    mOffsetY[camNo] = (1.0f - moveRate) * offset - r;
}

void TWipeWaveA::drawMain(int camNo, f32 rate, JUTColor colorRGB)
{
    JGeometry::TBox2f bounds = mGraph[camNo]->getBounds();

    if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->setPort();
    JUTColor color;
    u8 alphaRate = colorRGB.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);
    color.setRGBA(colorRGB, alphaRate);

    if (alphaRate)
    {
        f32 fracW = bounds.getWidth() / System::get2DVpW();
        f32 fracH = bounds.getHeight() / System::get2DVpH(); // boo boo?
        f32 x = bounds.getWidth() / (sWipeHeadWidth * fracW);
        f32 h = sWipeHeadHeight * (fracW);

        drawQuad(bounds.i.x, h + (bounds.i.y + mOffsetY[camNo]), bounds.getWidth(), bounds.getHeight(), color);
        initGX(color);
        drawTex(bounds.i.x, (bounds.i.y + mOffsetY[camNo]), bounds.getWidth(), h, x, sWipeStartPos);
    }
}

void TWipeWaveA::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    J2DFillBox(l, t, w, h, color);
}

void TWipeWaveA::initGX(const JUTColor &color)
{
    Mtx m;

    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    PSMTXIdentity(m);
    GXLoadPosMtxImm(m, 0);
    GXLoadTexMtxImm(m, 0x3c, GX_MTX3x4);
    GXSetCurrentMtx(0);
    GXLoadTexMtxImm(m, 0x3c, GX_MTX3x4);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    getTexture(2)->load(GX_TEXMAP0);
    GXSetTevKColor(GX_KCOLOR0, color);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
}

void TWipeWaveA::drawTex(f32 l, f32 t, f32 w, f32 h, f32 n, f32 f)
{
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(l, t);
    GXPosition2f32(0.0f, n + f);
    GXPosition2f32(l + w, t);
    GXPosition2f32(0.0f, f);
    GXPosition2f32(l + w, t + h);
    GXPosition2f32(1.0f, f);
    GXPosition2f32(l, t + h);
    GXPosition2f32(1.0f, n + f);

    GXEnd();
}