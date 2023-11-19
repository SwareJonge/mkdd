#include <JSystem/J2D/J2DAnmLoader.h>
#include "Osako/MenuTitleLine.h"

static const char *scaFileName[] = {
    "HowManyDrivers.bti",
    "SelectCharacter.bti",
    "SelectMode.bti",
    "VsCoop.bti",
    "SelectCourse.bti",
    "SelectBattle.bti",
    "SelectCup.bti",
    "SelectMap.bti",
    "Mozi_GDCheck.bti",
    "Mozi_GhostData_Load.bti",
    "Mozi_GhostData_Save.bti"
};

MenuTitleLine *MenuTitleLine::mspMenuTitleLine;

MenuTitleLine::MenuTitleLine(JKRArchive *archive)
{
    mTransFrame = 0;
    mKeyFrame = 0;
    mAnmType = mcAnim0;
    mScreen.setPriority("menu_title_line.blo", 0x1020000, archive);

    void *bck = JKRGetNameResource("menu_title_line.bck", archive);
    void *btk = JKRGetNameResource("menu_title_line.btk", archive);

    mTransform = (J2DAnmTransform *)J2DAnmLoaderDataBase::load(bck);
    mTexSRTKey = (J2DAnmTextureSRTKey *)J2DAnmLoaderDataBase::load(btk);

    mScreen.setAnimation(mTransform);
    mScreen.setAnimation(mTexSRTKey);

    for (int i = 0; i < mcTitleMax; i++)
        mImages[i] = (ResTIMG *)JKRFileLoader::getGlbResource(scaFileName[i], archive);

    mPicture = (J2DPicture *)mScreen.search('T_Mozi');
    mGhostPicture = (J2DPicture *)mScreen.search('T_Mozi1');
    mPicture->show();
    mGhostPicture->hide();
    mTitle = mcHowManyPlayers;
    mPicture->changeTexture(mImages[mTitle], 0);
}

void MenuTitleLine::drop(Title nextTitle)
{
    if (mTitle != nextTitle) {
        mTitle = nextTitle;
        switch (mTitle)
        {
        case mcHowManyPlayers:
        case mcCharacterSelect:
        case mcSelectMode:
        case mcVsCoop:
        case mcCourseSelect:
        case mcBattleSelect:
        case mcCupSelect:
        case mcMapSelect:
            mPicture->changeTexture(mImages[mTitle], 0);
            mPicture->show();
            mGhostPicture->hide();
            break;
        case mcGhostCheck:
        case mcGhostLoad:
        case mcGhostSave:
            mGhostPicture->changeTexture(mImages[mTitle], 0);
            mPicture->hide();
            mGhostPicture->show();
            break;
        }
    }

    mAnmType = mcAnimDrop;
}

void MenuTitleLine::lift()
{
    mAnmType = mcAnimLift;
}

void MenuTitleLine::calc()
{
    mTransform->setFrame(mTransFrame);
    mTexSRTKey->setFrame(mKeyFrame);

    switch (mAnmType)
    {
    case 1:
        if (++mTransFrame >= 11)
        {
            mAnmType = mcAnim2;
            mTransFrame = 10;
        }
        break;
    case 3:
        if (--mTransFrame < 0)
        {
            mAnmType = mcAnim0;
            mTransFrame = 0;
        }
        break;
    }

    if (++mKeyFrame >= 540)
    {
        mKeyFrame = 0;
    }
    mScreen.animation();
}