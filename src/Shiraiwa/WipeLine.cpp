#include "Osako/system.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

s16 TWipeLineA::sWipeLineWidth = 25;
s16 TWipeLineA::sWipeProgressLines = 20;
f32 TWipeLineA::sAlphaMinRate = 0.0f;
f32 TWipeLineA::sAlphaMaxRate = 1.0f;

s16 TWipeLineB::sWipeLineHeight = 20;
s16 TWipeLineB::sWipeProgressLines = 20;
f32 TWipeLineB::sAlphaMinRate = 0.0f;
f32 TWipeLineB::sAlphaMaxRate = 1.0f;

TWipeLineA::TWipeLineA(JKRHeap *heap) : TWipeBase(heap) { init(); }

TWipeLineA::~TWipeLineA()  {}

void TWipeLineA::init() {
    for (int i = 0; i < 4; i++)
        mOffset[i] = 0.0f;
}

void TWipeLineA::calcMain(int camNo, f32 rate, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, camNo);

    mGraph[camNo]->place(viewport);
    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

    mOffset[camNo] = rate * (sWipeLineWidth * sWipeProgressLines + viewport.getWidth());
}

void TWipeLineA::drawMain(int camNo, f32 rate, JUTColor colorRGB) {
    JGeometry::TBox2f bounds = mGraph[camNo]->getBounds();

    if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->setPort();
    JUTColor color;
    u8 alphaRate = colorRGB.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);
    color.setRGBA(colorRGB, alphaRate);

    f32 w = 0.0f;
    f32 fracW = (bounds.getWidth() / System::get2DVpW());
    f32 fracX = fracW * sWipeLineWidth;
    f32 line = (fracW * sWipeLineWidth) / sWipeProgressLines;
    
    f32 x = mOffset[camNo];
    for (; x >= -fracX; x -= fracX)
    {
        if (w < fracX)
            w = w + (line);
        else
            w = fracX;
    
        drawQuad(bounds.i.x + x, bounds.i.y, w, bounds.getHeight(), color);
    }
}

void TWipeLineA::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    if(color.a != 0)
        J2DFillBox(l, t, w, h, color);
}

TWipeLineB::TWipeLineB(JKRHeap *heap) : TWipeBase(heap) { init(); }

TWipeLineB::~TWipeLineB() {}

void TWipeLineB::init()
{
    for (int i = 0; i < 4; i++)
        mOffset[i] = 0.0f;
}

void TWipeLineB::calcMain(int camNo, f32 rate, JUTColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, camNo);

    mGraph[camNo]->place(viewport);
    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

    mOffset[camNo] = rate * (sWipeLineHeight * sWipeProgressLines + viewport.getHeight());
}

void TWipeLineB::drawMain(int camNo, f32 rate, JUTColor colorRGB) {
    JGeometry::TBox2f bounds = mGraph[camNo]->getBounds();

    if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->setPort();
    JUTColor color;
    u8 alphaRate = colorRGB.a * SiUtil::getNormalRange(rate, sAlphaMinRate, sAlphaMaxRate);
    color.setRGBA(colorRGB, alphaRate);

    f32 h = 0.0f;
    f32 fracH = (bounds.getHeight() / System::get2DVpH());
    f32 fracY = fracH * sWipeLineHeight;
    f32 line = (fracH * sWipeLineHeight) / sWipeProgressLines;

    f32 y = mOffset[camNo];
    for (; y >= -fracY; y -= fracY)
    {
        if (h < fracY)
            h = h + (line);
        else
            h = fracY;

        drawQuad(bounds.i.x, bounds.i.y + y, bounds.getWidth(), h, color);
    }
}

void TWipeLineB::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    if (color.a != 0)
        J2DFillBox(l, t, w, h, color);
}