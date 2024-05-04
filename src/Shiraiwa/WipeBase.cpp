#include "Osako/system.h"

#include "Kameda/J2DManager.h"
#include "Kameda/WipeManager.h"

#include "Shiraiwa/SiUtil.h"
#include "Shiraiwa/Wipe.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

const char *TWipeBase::scTextureName[3] = {
    "timg/WipeSlideCircle.bti",
    "timg/WipeCircle.bti",
    "timg/wipe_nami.bti"};

JUTTexture *TWipeBase::sTexture[3] = {
    nullptr, nullptr, nullptr};

TWipeBase::TWipeBase(JKRHeap *heap)
{
    mGraph = nullptr;
    mConsoleNum = 0;
    mConsoleNum = SiUtil::getConsoleNum();

    mGraph = new (heap, 0) J2DOrthoGraph *[mConsoleNum];

    for (int i = 0; i < mConsoleNum; i++)
        mGraph[i] = nullptr;

    switch (mConsoleNum)
    {
    case 1:
        mGraph[0] = new (heap, 0) J2DOrthoGraph(System::get3DVpX(), System::get3DVpY(), System::get3DVpW(), System::get3DVpH(), -1.0f, 1.0f);
        mGraph[0]->scissor(System::get3DScisX(), System::get3DScisY(), System::get3DScisW(), System::get3DScisH());
        break;
    case 2:
        for (u8 i = 0; i < mConsoleNum; i++)
        {
            mGraph[i] = new (heap, 0) J2DOrthoGraph(System::get3DVpDiv2X(i), System::get3DVpDiv2Y(i), System::get3DVpDiv2W(i), System::get3DVpDiv2H(i), -1.0f, 1.0f);
            mGraph[i]->scissor(System::get3DScisDiv2X(i), System::get3DScisDiv2Y(i), System::get3DScisDiv2W(i), System::get3DScisDiv2H(i));
        }
        break;
    case 3:
    case 4:
        for (u8 i = 0; i < mConsoleNum; i++)
        {
            mGraph[i] = new (heap, 0) J2DOrthoGraph(System::get3DVpDiv4X(i), System::get3DVpDiv4Y(i), System::get3DVpDiv4W(i), System::get3DVpDiv4H(i), -1.0f, 1.0f);

            mGraph[i]->scissor(System::get3DScisDiv4X(i), System::get3DScisDiv4Y(i), System::get3DScisDiv4W(i), System::get3DScisDiv4H(i));
        }
        break;
    }

    init();
}

void TWipeBase::loadTexture(JKRHeap *heap)
{
    for (int i = 0; i < 3; i++)
    {
        JKRArchive *archive = GETJ2DManager()->getArchive();
        ResTIMG *img = (ResTIMG *)archive->getResource(scTextureName[i]);

#line 100
        JUT_ASSERT_F(img, "texture %s can\'t load.", scTextureName[i]);
        if (i == 2)
        {
            img->mWrapS = 0;
            img->mWrapT = 2;
        }

        sTexture[i] = new (heap, 0) JUTTexture(img);
    }
}

JUTTexture *TWipeBase::getTexture(u32 id)
{
#line 117
    JUT_MINMAX_ASSERT(0, id, 3);
    return sTexture[id];
}

TWipeBase::~TWipeBase()
{
    for (int i = 0; i < mConsoleNum; i++)
        delete mGraph[i];

    delete[] mGraph;
}

void TWipeBase::createBuffer(int statusNo, JKRHeap *heap)
{
    loadTexture(heap);
}

void TWipeBase::init()
{
    reset();
}

void TWipeBase::reset()
{
    for (int i = 0; i < 4; i++)
        mWipeState[i] = -1;
}

void TWipeBase::calc(int status, f32 f, JUtility::TColor color)
{
    int wipeState = GETWipeManager()->getWipeState(J2DManager::getStatus2Kart(status));
    switch (wipeState)
    {
    case 0:
    case 3:
        break;
    default:
    {
        if (mWipeState[status] != wipeState)
        {
            changeState(status, wipeState);
        }
    }
    }

    calcMain(status, f, color);
    mWipeState[status] = wipeState;
}

void TWipeBase::changeState(int status, int wipeState) {}

void TWipeBase::draw(int status, f32 f, JUtility::TColor color)
{
    int wipeState = GETWipeManager()->getWipeState(J2DManager::getStatus2Kart(status));
    switch (wipeState)
    {
    case 0:
        drawPreIn(status, color);
        break;
    case 3:
        drawPreOut(status, color);
        break;
    default:
        drawMain(status, f, color);
        break;
    }
}

void TWipeBase::drawPreIn(int camNo, JUtility::TColor color)
{
    JGeometry::TBox2f viewport;
    SiUtil::getViewport(&viewport, camNo);

    if (viewport.getWidth() <= 0.0f || viewport.getHeight() <= 0.0f)
        return;

    mGraph[camNo]->place(viewport);

    mGraph[camNo]->setOrtho(viewport.i.x, viewport.i.y, viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
    mGraph[camNo]->setPort();
    J2DFillBox(viewport, color);
}