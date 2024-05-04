#include "Osako/system.h"

#include "Kameda/J2DManager.h"
#include "Kameda/WipeManager.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

f32 TWipeCurtain::sLineWidth = 102.0f;
f32 TWipeCurtain::sAlphaMinRate = 0.0f;
f32 TWipeCurtain::sAlphaMaxRate = 1.0f;

TWipeCurtain::TWipeCurtain(JKRHeap *heap) : TWipeBase(heap) { init(); }

TWipeCurtain::~TWipeCurtain() {}

void TWipeCurtain::init()
{
    for (int i = 0; i < 4; i++)
    {
        mHeight[i] = 0;
    }
}

void TWipeCurtain::calcMain(int camNo, f32 rate, JUTColor color)
{
    if (camNo > 0)
        return;
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, 0, 1);
    f32 fracH = (viewport.getHeight() / System::get2DVpH());
    f32 h = getTexture(0)->getWidth() * fracH;
    f32 combinedHeight = (h * 2.0f + viewport.getHeight());
    mHeight[camNo] = rate * (combinedHeight * rate) - h;
}

void TWipeCurtain::drawPreIn(int camNo, JUTColor color)
{
    if (camNo > 0)
        return;

    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, 0, 1);
    if (viewport.getWidth() <= 0.0f || viewport.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(viewport);

    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();
    J2DFillBox(viewport, color);
}

void TWipeCurtain::drawMain(int camNo, f32 rate, JUTColor colorRGB)
{
    if (camNo > 0)
        return;

    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, 0, 1);

    if (viewport.getWidth() <= 0.0f || viewport.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(viewport);

    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();

    JUTTexture *tex = getTexture(0);

    JUTColor color;
    u8 alphaRate = colorRGB.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);
    color.setRGBA(colorRGB, alphaRate);

    if (color.a == 0)
        return;

    f32 fracW = viewport.getWidth() / System::get2DVpW();
    f32 w = sLineWidth * fracW;

    for (int i = 0; ((i - 1) * w) < viewport.getWidth(); i++)
        drawQuad((i * w) + viewport.i.x, viewport.i.y, w, mHeight[camNo], color);

    initGXForTex(color);

    f32 fracH = viewport.getHeight() / System::get2DVpH();
    f32 h = (sLineWidth * (fracH * tex->getWidth())) / tex->getHeight();

    for (int i = 0; ((i - 1) * w) < viewport.getWidth(); i++)
        drawImage(tex, (i * w) + viewport.i.x, viewport.i.y + mHeight[camNo], w, h);
}

void TWipeCurtain::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color) { J2DFillBox(l, t, w, h, color); }

void TWipeCurtain::drawImage(JUTTexture *tex, f32 l, f32 t, f32 w, f32 h)
{
    tex->load(GX_TEXMAP0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(l, t);
    GXTexCoord2u8(0, 1);
    GXPosition2f32(l + w, t);
    GXTexCoord2u8(0, 0);
    GXPosition2f32(l + w, t + h);
    GXTexCoord2u8(1, 0);
    GXPosition2f32(l, t + h);
    GXTexCoord2u8(1, 1);
    GXEnd();
}

void TWipeCurtain::initGXForTex(JUTColor color)
{
    GXSetCullMode(GX_CULL_BACK);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);

    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){color.r, color.g, color.b, color.a}); // i hate you
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
}