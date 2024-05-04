#include "Osako/system.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

s16 TWipeShut::scWipeHeadHeight = 0x46;
s16 TWipeShut::scWipeHeadWidth = 0x50;
f32 TWipeShut::sChangeRate = 0.55f;
f32 TWipeShut::sSecondWidthOrg = 290.0f;
f32 TWipeShut::sAlphaMinRate = 0.0f;
f32 TWipeShut::sAlphaMaxRate = 1.0f;

TWipeShut::TWipeShut(JKRHeap *heap) : TWipeBase(heap) { init(); }

TWipeShut::~TWipeShut() {}

void TWipeShut::init()
{
    for (int i = 0; i < 4; i++)
        mWidth[i] = 0.0f;
}

// Calculate parameters for wiping effect based on camera number, rate, and color
void TWipeShut::calcMain(int cameraNumber, f32 rate, JUTColor color)
{
    // Get the viewport for the specified camera
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, cameraNumber);

    // Set the viewport and orthographic projection for the graphics of the specified camera
    mGraph[cameraNumber]->place(viewport);
    mGraph[cameraNumber]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

    // Calculate the fraction of width relative to the entire viewport width
    f32 fractionWidth = (viewport.getWidth() / System::get2DVpW());

    // Calculate the initial width for the wiping effect
    f32 initialWidth = scWipeHeadWidth * fractionWidth;
    f32 calculatedWipeWidth = 1.5f * initialWidth + 0.5f * viewport.getWidth();

    // Check if the rate is less than or equal to a predefined change rate
    if(rate <= sChangeRate) {
        // Calculate the width of the wiping effect based on the rate
        mWidth[cameraNumber] = (calculatedWipeWidth * (rate * rate)) / (sChangeRate * sChangeRate) - initialWidth;
    }
    else {
        // Calculate parameters for the second phase of the wiping effect
        f32 initialSecondWidth = sSecondWidthOrg * fractionWidth;
        f32 changeNormalization = (0.5f * (1.0f + sChangeRate));
        f32 rateMinusChangeNorm = rate - changeNormalization;
        f32 rateMinusOneMinusChangeNorm =  1.0f - changeNormalization;
        mWidth[cameraNumber] = (initialSecondWidth + (rateMinusChangeNorm * rateMinusChangeNorm * (calculatedWipeWidth - initialSecondWidth)) / (rateMinusOneMinusChangeNorm * rateMinusOneMinusChangeNorm)) - initialWidth;
    }
}

void TWipeShut::drawMain(int camNo, f32 rate, JUTColor colorRGB)
{
    JGeometry::TBox2f bounds = mGraph[camNo]->getBounds();

    if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->setPort();
    initGX();
    
    f32 headW = scWipeHeadWidth * bounds.getHeight() / System::get2DVpH();
    f32 w = mWidth[camNo] - headW;
    JUTColor color;
    u8 alphaRate = colorRGB.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);
    color.setRGBA(colorRGB, alphaRate);

   // f32 fracW = (bounds.getWidth() / System::get2DVpW());
    // f32 fracX = fracW * sWipeLineWidth;
    // f32 line = (fracW * sWipeLineWidth) / sWipeProgressLines;
    drawQuad(bounds.i.x, bounds.i.y, w, bounds.getHeight(), color);
    drawQuad(bounds.f.x - w, bounds.i.y, w, bounds.getHeight(), color);

    f32 hw = scWipeHeadHeight * bounds.getHeight() / System::get2DVpH();

    bool alternator = true;

    for (f32 y = 0.0f; y < bounds.getHeight(); y += hw, alternator = !alternator)
    {
        JGeometry::TVec2f trigPos[3];
        trigPos[0].x = (bounds.i.x + w);
        trigPos[1].x = headW + (bounds.i.x + w);
        trigPos[2].x = (bounds.i.x + w);
        trigPos[0].y = (bounds.i.y + y);
        trigPos[2].y = hw + (bounds.i.y + y);
        if(alternator) {
            trigPos[1].y = (bounds.i.y + y);
        }
        else {
            trigPos[1].y = hw + (bounds.i.y + y);
        }
        drawTriangle(trigPos, color);
        trigPos[0].x = bounds.f.x - w;
        trigPos[1].x = (bounds.f.x - w) - headW;
        trigPos[2].x = bounds.f.x - w;
        trigPos[0].y = hw + (bounds.i.y + y);
        trigPos[2].y = (bounds.i.y + y);
        if(alternator) {
            trigPos[1].y = hw + (bounds.i.y + y);
        }
        else {
            trigPos[1].y = (bounds.i.y + y);
        }
        drawTriangle(trigPos, color);
    }
}

void TWipeShut::initGX()
{
    Mtx m;

    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    PSMTXIdentity(m);
    GXLoadPosMtxImm(m, 0);
    GXLoadTexMtxImm(m, 0x3c, GX_MTX3x4);
    GXSetCurrentMtx(0);
    GXLoadTexMtxImm(m, 0x3c, GX_MTX3x4);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

void TWipeShut::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    if (color.a == 0)
        return;

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(l, t);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(l + w, t);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(l + w, t + h);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(l, t + h);
    GXColor4u8(color.r, color.g, color.b, color.a);

    GXEnd();
}

void TWipeShut::drawTriangle(JGeometry::TVec2f *pos, const JUTColor &color)
{
    if (color.a == 0)
        return;

    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);

    GXPosition2f32(pos[0].x, pos[0].y);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(pos[1].x, pos[1].y);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(pos[2].x, pos[2].y);
    GXColor4u8(color.r, color.g, color.b, color.a);

    GXEnd();
}