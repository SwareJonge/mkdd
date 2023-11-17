#include <JSystem/J2D/J2DAnmLoader.h>
#include "Osako/MenuBackground.h"

MenuBackground *MenuBackground::mspMenuBackground;

MenuBackground::MenuBackground(JKRArchive *archive)
{
    mAnimFrame = 0;
    mScreen.setPriority("menu_back.blo", 0x20000, archive);
    mAnimation = (J2DAnmTextureSRTKey *)J2DAnmLoaderDataBase::load(JKRGetNameResource("menu_back.btk", archive));
    mScreen.setAnimation(mAnimation);
}

void MenuBackground::calc()
{
    mAnimation->setFrame(mAnimFrame);

    if (++mAnimFrame >= 360)
        mAnimFrame = 0;

    mScreen.animation();
}