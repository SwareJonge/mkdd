#include <stdio.h>
#include "Kameda/BattleName2D.h"

#include <JSystem/JAudio/JASFakeMatch2.h>

BattleName2D *BattleName2D::mBattleName2D;

BattleName2D::BattleName2D(JKRArchive *archive)
{
    mBattleName2D = this;
    for (int i = 0; i < BATTLE_MAX; i++)
    {
        char buf[0x40];
        mTextures[i] = nullptr;

        snprintf(buf, sizeof(buf), "timg/Mozi_Battle%d.bti", i + 1);
        mTextures[i] = (const ResTIMG *)archive->getResource(buf);
    }
}

BattleName2D::~BattleName2D()
{
    if (mBattleName2D)
        mBattleName2D = nullptr;
}

const ResTIMG *BattleName2D::getBattleNameTexture(BattleName2D::BattleName name)
{
    const ResTIMG *tex = nullptr;
    switch (name)
    {
    case BALLOON_BATTLE:
    case BOMB_BATTLE:
    case SHINE_THIEF:
        tex = mTextures[name];
        break;
    }

    return tex;
}