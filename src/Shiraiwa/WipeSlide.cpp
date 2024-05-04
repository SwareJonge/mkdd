#include "Osako/system.h"

#include "Kameda/J2DManager.h"
#include "Kameda/WipeManager.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

u8 TWipeSlide::sWipePatternMask = 1;

TWipeSlide::TWipeSlide(JKRHeap *heap) : TWipeBase(heap)
{
    init();
}

TWipeSlide::~TWipeSlide() {}

void TWipeSlide::init() {
    for (int i = 0; i < 4; i++) {
        _24[i] = 0;
        _1c[i] = 0;
    }
}

void TWipeSlide::calcMain(int status, f32 f, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, status);

    _1c[status] = 90 * (viewport.getHeight() / System::get2DVpH());
    f32 drawPos = (_1c[status] * 32) / getTexture(0)->getHeight();
    _24[status] = (f * f) * (2.0f * drawPos + viewport.getWidth()) - drawPos;

    int wipeState = GETWipeManager()->getWipeState(J2DManager::getStatus2Kart(status));
    if(wipeState <= 1u || wipeState == 2)
        mPattern = 0;
    else
        mPattern = 1;
}

void TWipeSlide::drawMain(int camNo, f32 rate, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, camNo);

    if (viewport.getWidth() <= 0.0f || viewport.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(viewport);

    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();

    JUTTexture *tex = getTexture(0);

    s16 h = _1c[camNo];
    f32 w = _24[camNo];
    f32 l = viewport.i.x;
    f32 t = viewport.i.y;
    f32 imgW = (h * 32) / tex->getHeight();

    if (h <= 0)
        return;

    for (int i = 0; ((i - 1) * h) < viewport.getHeight(); i++)
    {
        if (i & 1)
        {
            f32 p = i * h;
            drawQuad(l, t + p, w, h, color);
        }
        else
        {
            f32 p = i * h;
            drawQuad(l + viewport.getWidth() - w, t + p, w, h, color);
        }
    }

    initGXForTex(color);

    f32 x = l + w;

    bool pattern = !(mPattern ^ sWipePatternMask);

    for (int i = 0; ((i - 1) * h) < viewport.getHeight(); i++)
    {
        if (i & 1)
        {
            f32 p = i * h;
            drawImage(tex, pattern, x, t + p, imgW, h);
        }
        else
        {
            f32 p = i * h;
            drawImage(tex, !pattern, l + viewport.getWidth() - w - imgW, t + p, imgW, h);
        }
    }
}

void TWipeSlide::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    J2DFillBox(l, t, w, h, color);
}

void TWipeSlide::drawImage(JUTTexture *tex, bool p2, f32 l, f32 t, f32 w, f32 h)
{
    tex->load(GX_TEXMAP0);

    u8 idont;
    u8 understandGX;

    if (p2)
    {
        idont = 0;
        understandGX = 1;
    }
    else {
        idont = 1;
        understandGX = 0;
    }

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(l, t);
    GXTexCoord2u8(idont, 0);
    GXPosition2f32(l + w, t);
    GXTexCoord2u8(understandGX, 0);
    GXPosition2f32(l + w, t + h);
    GXTexCoord2u8(understandGX, 1);
    GXPosition2f32(l, t + h);
    GXTexCoord2u8(idont, 1);
    GXEnd();
}

void TWipeSlide::initGXForTex(JUTColor color)
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

    if (!(mPattern ^ sWipePatternMask)) // inline?
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
    else
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){color.r, color.g, color.b, color.a}); // i hate you
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
}

