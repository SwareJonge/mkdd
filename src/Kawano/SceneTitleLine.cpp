#include <JSystem/J2D/J2DAnmLoader.h>
#include <JSystem/J2D/J2DScreen.h>
#include <stdio.h>

#include "Kawano/SceneTitleLine.h"

const u64 SceneTitleLine::mTag_Titleline[10] = {
    'T_line',
    'T_Mozi',
    'T_Mozi1',
    'T_Mozi2',
    'T_Mozi3',
    'T_Mozi4',
    'T_Mozi5',
    'T_Mozi6',
    'T_Mozi7',
    'T_Mozi8'
};

SceneTitleLine::SceneTitleLine(JKRHeap *heap)
{
    // UNUSED, fabricated

    mScreen = new (heap, 0) J2DScreen();

    mScreen->set("menu_title_line.blo", 0x1040000, nullptr);

    mTransform = (J2DAnmTransform *)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("menu_title_line.bck"));
    mScreen->setAnimation(mTransform);

    mTexSRTKey = (J2DAnmTextureSRTKey *)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("menu_title_line.btk"));
    mScreen->setAnimation(mTexSRTKey);

    for (int i = 0; i < NUM_TAGS; i++)
    {
        mPane[i] = mScreen->search(mTag_Titleline[i]);
        J2DPane *pane = mPane[i];

        if (pane)
            pane->setAnimation(mTransform);
    }
}

SceneTitleLine::SceneTitleLine(JKRArchive *arc, JKRHeap *heap, char *name)
{
    mScreen = new (heap, 0) J2DScreen();

    char buf[128];
    sprintf(buf, "%s.blo", name);
    mScreen->set(buf, 0x1040000, arc);

    sprintf(buf, "%s.bck", name);
    mTransform = (J2DAnmTransform *)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(buf, arc));
    mScreen->setAnimation(mTransform);

    sprintf(buf, "%s.btk", name);
    mTexSRTKey = (J2DAnmTextureSRTKey *)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(buf, arc));
    mScreen->setAnimation(mTexSRTKey);

    for (int i = 0; i < NUM_TAGS; i++)
    {
        mPane[i] = mScreen->search(mTag_Titleline[i]);
        J2DPane *pane = mPane[i];

        if (pane)
            pane->setAnimation(mTransform);
    }
}

SceneTitleLine::~SceneTitleLine()
{
    delete mScreen;
}

void SceneTitleLine::init_titleline(int paneNo) {
    for (int i = 0; i < NUM_TAGS; i++)
    {
        if (mPane[i])
            mPane[i]->hide();
    }

    mPane[0]->show();

    if(mPane[paneNo])
        mPane[paneNo]->show();

    mAnmFrame = 0;
    mMatAnmFrame = 0;
}

void SceneTitleLine::drawTitleLine(J2DOrthoGraph *ortho)
{
    ortho->setPort();
    mScreen->draw(0.0f, 0.0f, ortho);
}

void SceneTitleLine::calcAnmTitleLine(int increment)
{
    if (increment > 0)
        increment = 1;
    if (increment < 0)
        increment = -1;

    mAnmFrame += increment;
    if (mAnmFrame < 0)
    {
        mAnmFrame = 0;
    }
    if (mAnmFrame > 14)
    {
        mAnmFrame = 14;
    }

    mTransform->setFrame(mAnmFrame);
    for (int i = 0; i < NUM_TAGS; i++)
    {
        if (mPane[i])
            mPane[i]->animationTransform();
    }
}

void SceneTitleLine::calcMatAnmTitleLine()
{
    if (++mMatAnmFrame >= 540)
        mMatAnmFrame = 0;

    mTexSRTKey->setFrame(mMatAnmFrame);
    mScreen->animation();
}
