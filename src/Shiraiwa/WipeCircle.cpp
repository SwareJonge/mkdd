#include "Osako/system.h"

#include "Kaneshige/RaceMgr.h"

#include "Kameda/J2DManager.h"
#include "Kameda/WipeManager.h"

#include "Sato/ObjUtility.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

f32 TWipeCircle::sAlphaMinRate = 0.0f;
f32 TWipeCircle::sAlphaMaxRate = 1.0f;

TWipeCircle::TWipeCircle(JKRHeap *heap) : TWipeBase(heap)
{
    init();
}

TWipeCircle::~TWipeCircle() {}

void TWipeCircle::init()
{
    for (int i = 0; i < mConsoleNum; i++)
    {
        _20[i] = 0;
        // This stuff makes no sense
        JGeometry::TBox2f &bounds = mGraph[i]->getBounds();
        f32 t = bounds.i.y;
        f32 r = bounds.i.x;
        f32 l = bounds.f.x;
        f32 b = bounds.f.y;

        mPos[i].set(l + 0.5f * (r - l), b + 0.5f * (t - b)); // this has to be inlined, right? there's no way you'd write to something and then setting it back to 0
        mPos[i].zero();

        _64[i] = 0.0f;
    }
}

void TWipeCircle::calcMain(int status, f32 rate, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, status);

    mGraph[status]->place(viewport);
    mGraph[status]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

    f32 frac = (viewport.getHeight() / System::get2DVpH());
    _64[status] = 0.5f * ((1.0f - rate) * (1000.0f * frac));

    if (GETWipeManager()->getWipeState(J2DManager::getStatus2Kart(status)) == 4)
    {
        JGeometry::TVec3f kartPos;
        ObjUtility::getKartPos(RCMGetManager()->getTargetKartNo(status), &kartPos);
        SiUtil::getScreenPos(status, kartPos, &mPos[status], false);
    }
    else
    {
        mPos[status].set(0.5f * viewport.getWidth(), 0.5f * viewport.getHeight());
        mPos[status].add(viewport.i);
    }
}

void TWipeCircle::drawMain(int status, f32 rate, JUTColor color)
{
    JGeometry::TBox2f bounds = mGraph[status]->getBounds();

    if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
        return;

    mGraph[status]->setPort();
    initGX();

    JGeometry::TBox2f other(mPos[status].x - _64[status], mPos[status].y - _64[status], mPos[status].x + _64[status], mPos[status].y + _64[status]);
    u8 a = color.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);

    GXSetTevKColor(GX_KCOLOR0, (GXColor){color.r, color.g, color.b, a});

    drawBufferImage(getTexture(1), other, bounds);
}

void TWipeCircle::initGX()
{
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
}

void TWipeCircle::drawBufferImage(JUTTexture *tex, JGeometry::TBox2f &img, JGeometry::TBox2f &bounds)
{
    f32 l, t, r, b;
    if (img.getWidth() <= 0.0f || img.getHeight() <= 0.0f)
    {
        r = img.i.x;
        l = r;
        b = img.i.y;
        t = b;
    }
    else
    {
        l = (bounds.i.x - img.i.x) / img.getWidth();
        r = 1.0f + (bounds.f.x - img.f.x) / img.getWidth();
        t = (bounds.i.y - img.i.y) / img.getHeight();
        b = 1.0f + (bounds.f.y - img.f.y) / img.getHeight();
    }

    tex->load(GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(bounds.i.x, bounds.i.y);
    GXPosition2f32(l, t);
    GXPosition2f32(bounds.f.x, bounds.i.y);
    GXPosition2f32(r, t);
    GXPosition2f32(bounds.f.x, bounds.f.y);
    GXPosition2f32(r, b);
    GXPosition2f32(bounds.i.x, bounds.f.y);
    GXPosition2f32(l, b);
    GXEnd();
}