#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

s16 TWipeCheck::sWipeHeightOrg = 75;
s16 TWipeCheck::sWipeWidthOrg = 75;
f32 TWipeCheck::sAlphaDiffRate = -0.5f;

TWipeCheck::TWipeCheck(JKRHeap *heap) : TWipeBase(heap)
{
    init();
    SiUtil::getViewport(&mVp, 0, 1);
}

TWipeCheck::~TWipeCheck() {}

void TWipeCheck::init()
{
    for (int i = 0; i < 4; i++)
        mAlpha[i] = 0xff;
}

void TWipeCheck::calcMain(int camNo, f32 rate, JUTColor color)
{
    if (camNo != 0)
        return;

    mAlpha[camNo] = SiUtil::getNormalRange(rate, 0.0f, 1.0f) * 255;
    mAlphaDiff[camNo] = SiUtil::getNormalRange(rate + sAlphaDiffRate, sAlphaDiffRate, 1.0f) * 255;
}

void TWipeCheck::drawPreIn(int camNo, JUTColor color)
{
    if (camNo != 0 || mVp.getWidth() <= 0.0f || mVp.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(mVp);

    mGraph[camNo]->setOrtho(mVp.i.x, mVp.i.y, mVp.getWidth(), mVp.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();
    J2DFillBox(mVp, color);
}

void TWipeCheck::drawMain(int camNo, f32 rate, JUTColor colorRGB)
{
    if (camNo != 0 || mVp.getWidth() <= 0.0f || mVp.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(mVp);

    mGraph[camNo]->setOrtho(mVp.i.x, mVp.i.y, mVp.getWidth(), mVp.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();

    f32 w = sWipeWidthOrg;
    f32 h = sWipeHeightOrg;

    JUTColor color;
    color.setRGBA(colorRGB, mAlpha[camNo]);

    int i = 0;
    for (f32 y = 0.0f; y < mVp.getHeight(); i++, y += h)
    {
        bool j = i & 1;
        for (f32 x = 0.0f; x < mVp.getWidth(); j = !j, x += w)
        {
            if (!j)
            {
                color.a = mAlphaDiff[camNo];
                drawQuad(x, y, w, h, color);
            }
        }
    }

    drawQuad(mVp.i.x, mVp.i.y, mVp.f.x - mVp.i.x, mVp.f.y - mVp.i.y, mAlpha[camNo]); // works because of u32 operator(sets RGB to 0)
}

void TWipeCheck::drawQuad(f32 l, f32 t, f32 w, f32 h, const JUTColor &color)
{
    J2DFillBox(l, t, w, h, color);
}
