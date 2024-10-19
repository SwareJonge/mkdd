#include "Kaneshige/DarkAnmMgr.h"


// NG = Normal Ghost
// SG = Staff Ghost
DarkAnmMgr *DarkAnmMgr::sManager;
KartTevColor DarkAnmPlayer::sMarioNormColor(-20,-20,-20,80);
KartTevColor DarkAnmPlayer::sMarioNGNormColor(-100,-30,50,80);
KartTevColor DarkAnmPlayer::sMarioSGNormColor(50,-30,-100,80);
KartTevColor DarkAnmPlayer::sMarioBlackColor(-20,-20,-20,40);
KartTevColor DarkAnmPlayer::sMarioGreenColor(-100,0,-80,60);
KartTevColor DarkAnmPlayer::sMarioRedColor(-30,-50,-100,60);
KartTevColor DarkAnmPlayer::sMarioNightColor(-40,-10,20,60);
KartTevColor DarkAnmPlayer::sKoopaNormColor(-20,-20,-20,100);
KartTevColor DarkAnmPlayer::sKoopaNGNormColor(-100,-30,50,100);
KartTevColor DarkAnmPlayer::sKoopaSGNormColor(50,-30,-100,100);
KartTevColor DarkAnmPlayer::sKoopaBlackColor(-20,-20,-20,50);
KartTevColor DarkAnmPlayer::sKoopaGreenColor(-100,-30,-80,60);
KartTevColor DarkAnmPlayer::sKoopaRedColor(-30,-40,-100,60);
KartTevColor DarkAnmPlayer::sKoopaNightColor(-50,-30,10,80);
KartTevColor DarkAnmPlayer::sPeachNormColor(-20,-20,-20,60);
KartTevColor DarkAnmPlayer::sPeachNGNormColor(-100,-30,50,60);
KartTevColor DarkAnmPlayer::sPeachSGNormColor(50,-30,-100,60);
KartTevColor DarkAnmPlayer::sPeachBlackColor(-20,-20,-20,30);
KartTevColor DarkAnmPlayer::sPeachGreenColor(-100,-10,-80,30);
KartTevColor DarkAnmPlayer::sPeachRedColor(-30,-40,-100,30);
KartTevColor DarkAnmPlayer::sPeachNightColor(-30,-20,10,60);
KartTevColor DarkAnmPlayer::sMarioKartNormColor(0,0,0,100);
KartTevColor DarkAnmPlayer::sMarioKartNGNormColor(-100,-30,50,100);
KartTevColor DarkAnmPlayer::sMarioKartSGNormColor(50,-30,-100,100);
KartTevColor DarkAnmPlayer::sMarioKartBlackColor(0,0,0,50);
KartTevColor DarkAnmPlayer::sMarioKartGreenColor(-100,0,-80,60);
KartTevColor DarkAnmPlayer::sMarioKartRedColor(0,-30,-100,70);
KartTevColor DarkAnmPlayer::sMarioKartNightColor(-40,-10,20,70);
KartTevColor DarkAnmPlayer::sDonkeyKartNormColor(0,0,0,60);
KartTevColor DarkAnmPlayer::sDonkeyKartNGNormColor(-100,-30,50,60);
KartTevColor DarkAnmPlayer::sDonkeyKartSGNormColor(50,-30,-100,60);
KartTevColor DarkAnmPlayer::sDonkeyKartBlackColor(0,0,0,30);
KartTevColor DarkAnmPlayer::sDonkeyKartGreenColor(-100,0,-80,40);
KartTevColor DarkAnmPlayer::sDonkeyKartRedColor(-20,-30,-100,40);
KartTevColor DarkAnmPlayer::sDonkeyKartNightColor(-40,-20,0,60);

KartTevColor ObjDarkAnmPlayer::sObjJugemNormColor(-0x14,-0x14,-0x14,0x50);
KartTevColor ObjDarkAnmPlayer::sObjJugemBlackColor(-0x14,-0x14,-0x14,0x28);
KartTevColor ObjDarkAnmPlayer::sObjJugemGreenColor(-100,-10,-0x50,0x3c);
KartTevColor ObjDarkAnmPlayer::sObjJugemRedColor(-0x1e,-0x32,-100,0x3c);
KartTevColor ObjDarkAnmPlayer::sObjJugemNightColor(-0x28,-10,0x14,0x3c);
KartTevColor ObjDarkAnmPlayer::sObjCarNormColor(0,0,0,0xff);
KartTevColor ObjDarkAnmPlayer::sObjCarBlackColor(0,0,0,0xff);
KartTevColor ObjDarkAnmPlayer::sObjCarGreenColor(-0x3c,-10,-0x28,0xff);
KartTevColor ObjDarkAnmPlayer::sObjCarRedColor(-10,-0x32,-100,0xff);
KartTevColor ObjDarkAnmPlayer::sObjCarNightColor(-0x1e,0,0x14,0xff);

KartTevColor ItemDarkAnmPlayer::sItemGTurtleNormBColor(0,0,0,100);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleNormFColor(-0x28,-0x28,-0x28,0x46);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleBigNormFColor(-0x19,-0x19,-0x19,0x46);
KartTevColor ItemDarkAnmPlayer::sItemRTurtleNormFColor(-0x3c,-0x3c,-0x3c,0x46);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleBlackColor(0,0,0,0x32);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleGreenColor(-100,-10,-0x50,100);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleRedColor(-0x32,-0x46,-0x50,100);
KartTevColor ItemDarkAnmPlayer::sItemGTurtleNightColor(-0x28,-10,0x14,100);
KartTevColor ItemDarkAnmPlayer::sItemBananaNormBColor(0,0,0,0x3c);
KartTevColor ItemDarkAnmPlayer::sItemBananaNormFColor(-0x3c,-0x3c,-0x3c,0x1e);
KartTevColor ItemDarkAnmPlayer::sItemBananaBigNormFColor(-0x32,-0x3c,-0x3c,0x1e);
KartTevColor ItemDarkAnmPlayer::sItemBananaBlackColor(0,0,0,0x1e);
KartTevColor ItemDarkAnmPlayer::sItemBananaGreenColor(-100,-10,-0x50,0x3c);
KartTevColor ItemDarkAnmPlayer::sItemBananaRedColor(-0x32,-0x46,-0x50,0x3c);
KartTevColor ItemDarkAnmPlayer::sItemBananaNightColor(-0x28,-10,0x14,0x3c);
KartTevColor ItemDarkAnmPlayer::sItemKinokoNormBColor(0,0,0,0x80);
KartTevColor ItemDarkAnmPlayer::sItemKinokoNormFColor(-0x2d,-0x2d,-0x2d,0x40);
KartTevColor ItemDarkAnmPlayer::sItemKinokoBlackColor(0,0,0,0x40);
KartTevColor ItemDarkAnmPlayer::sItemKinokoGreenColor(-100,-10,-0x50,0x80);
KartTevColor ItemDarkAnmPlayer::sItemKinokoRedColor(-0x32,-0x46,-0x50,0x80);
KartTevColor ItemDarkAnmPlayer::sItemKinokoNightColor(-0x28,-10,0x14,0x80);
KartTevColor ItemDarkAnmPlayer::sItemBombNormBColor(0,0,0,0x6e);
KartTevColor ItemDarkAnmPlayer::sItemBombNormFColor(-0x50,-0x50,-0x50,100);
KartTevColor ItemDarkAnmPlayer::sItemBombBlackColor(0,0,0,0x37);
KartTevColor ItemDarkAnmPlayer::sItemBombGreenColor(-100,-10,-0x50,0x6e);
KartTevColor ItemDarkAnmPlayer::sItemBombRedColor(-0x32,-0x46,-0x50,0x6e);
KartTevColor ItemDarkAnmPlayer::sItemBombNightColor(-0x28,-10,0x14,0x6e);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggNormBColor(0,0,0,100);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggNormFColor(-0x3c,-0x3c,-0x3c,0x50);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggBlackColor(0,0,0,0x32);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggGreenColor(-100,-10,-0x50,100);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggRedColor(-0x32,-0x46,-0x50,100);
KartTevColor ItemDarkAnmPlayer::sItemYoshiEggNightColor(-0x28,-10,0x14,100);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleNormBColor(0,0,0,0x50);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleNormFColor(-0x28,-0x28,-0x28,0x50);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleBlackColor(0,0,0,0x28);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleGreenColor(-100,-10,-0x50,0x50);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleRedColor(-0x32,-0x46,-0x50,0x50);
KartTevColor ItemDarkAnmPlayer::sItemFlyTurtleNightColor(-0x28,-10,0x14,0x50);

DarkAnmMgr::DarkAnmMgr() {
    sManager = this;
    mColor = nullptr;
    reset();
}

DarkAnmMgr::~DarkAnmMgr() {
    sManager = nullptr;
}

void DarkAnmMgr::reset() {}

DarkAnmPlayer::DarkAnmPlayer() {
    mIsGhost = false;
    mNormColor = nullptr;

    for(int i = 0; i < 4; i++)
        mColors[i] = nullptr;

    mTevColor.set(0,0,0,0xff);
}

DarkAnmPlayer::DarkAnmPlayer(ECharID driverID, EGhostKind ghostKind) {
    switch (ghostKind)
    {
    case KIND_NORMAL_GHOST:
        mIsGhost = true;
        break;
    case KIND_STAFF_GHOST:
        mIsGhost = true;
        break;    
    default:
        mIsGhost = false;
        break;
    }
    
    switch(driverID) {
    case DAISY:
    case MARIO:
        setKartColorData(ghostKind,&sMarioNormColor,
                    &sMarioNGNormColor,&sMarioSGNormColor,
                    &sMarioBlackColor,&sMarioGreenColor,
                    &sMarioRedColor,&sMarioNightColor);
        break;
    case BABY_MARIO:
    case PARATROOPA:
    case KOOPATROOPA:
    case WARIO:
    case BOWSER:
    case BOWSERJR:
    case KING_BOO:
    case PETEY:
        setKartColorData(ghostKind, &sKoopaNormColor, &sKoopaNGNormColor, &sKoopaSGNormColor, &sKoopaBlackColor, &sKoopaGreenColor, &sKoopaRedColor, &sKoopaNightColor);
        break;
    case BABY_LUIGI:
    case PEACH:
    case LUIGI:
    case WALUIGI:
    case YOSHI:
    case BIRDO:
    case DONKEYKONG:
    case DONKEYJR:
    case TOAD:
    case TOADETTE:
        setKartColorData(ghostKind,&sPeachNormColor,
                            &sPeachNGNormColor,&sPeachSGNormColor,
                            &sPeachBlackColor,&sPeachGreenColor,
                            &sPeachRedColor,&sPeachNightColor);
        break;
    default:
#line 395
        JUT_ASSERT_F(0, "UNKNOWN DRIVER:%d", driverID)
    }
    reset();
}

DarkAnmPlayer::DarkAnmPlayer(EKartID kartID, EGhostKind ghostKind) {
    switch (ghostKind)
    {
    case KIND_NORMAL_GHOST:
        mIsGhost = true;
        break;
    case KIND_STAFF_GHOST:
        mIsGhost = true;
        break;    
    default:
        mIsGhost = false;
        break;
    }
    
    switch(kartID) {
    case RED_FIRE:
    case TURBO_YOSHI:
    case KOOPA_DASHER:
    case HEART_COACH:
    case GOO_GOO_BUGGY:
    case WARIO_CAR:
    case KOOPA_KING:
    case GREEN_FIRE:
    case BARREL_TRAIN:
    case TURBO_BIRDO:
    case PARA_WING:
    case BLOOM_COACH:
    case RATTLE_BUGGY:
    case BULLET_BLASTER:
    case TOAD_KART:
    case TOADETTE_KART:
    case BOO_PIPES:
    case PIRANHA_PIPES:
    case PARADE_KART:
        setKartColorData(ghostKind,&sMarioKartNormColor,
            &sMarioKartNGNormColor,&sMarioKartSGNormColor
            ,&sMarioKartBlackColor,&sMarioKartGreenColor,
            &sMarioKartRedColor,&sMarioKartNightColor);
        break;
    case DK_JUMBO:
    case WALUIGI_RACER:
        setKartColorData(ghostKind,&sDonkeyKartNormColor,
            &sDonkeyKartNGNormColor,&sDonkeyKartSGNormColor
            ,&sDonkeyKartBlackColor,&sDonkeyKartGreenColor,
            &sDonkeyKartRedColor,&sDonkeyKartNightColor);
        break;
    default:
#line 457
        JUT_ASSERT_F(0, "UNKNOWN KART:%d", kartID)
    }
    reset();
}

void DarkAnmPlayer::setKartColorData(EGhostKind ghostKind, KartTevColor *norm, KartTevColor *ngNorm, 
KartTevColor *sgNorm, KartTevColor *black, KartTevColor *green, 
KartTevColor *red, KartTevColor *night) {
    switch (ghostKind)
    {
    case KIND_NORMAL_GHOST:
        mNormColor = ngNorm;
        break;
    case KIND_STAFF_GHOST:
        mNormColor = sgNorm;
        break;    
    default:
        mNormColor = norm;
        break;
    }

    mColors[0] = black;
    mColors[1] = green;
    mColors[2] = red;
    mColors[3] = night;
}

void DarkAnmPlayer::reset() {
    if (mNormColor)
        mTevColor = *mNormColor;
}

void DarkAnmPlayer::calc(const CrsArea &area) {
    //f32 rate = area.getShadowRate();
    //int id = area.getShadowID();
    calc(area.getShadowID(), area.getShadowRate());
}

void DarkAnmPlayer::calc(int darkAnmID, f32 rate) {
    KartTevColor *normColor = mNormColor; // ...
    if (!normColor)
        return;
    if (mIsGhost)
        return;
#line 526
    JUT_MINMAX_ASSERT(0, darkAnmID, 4);

    KartTevColor *anmColor = mColors[darkAnmID];
    mTevColor.r = 0.5f + (rate * (anmColor->r - normColor->r) + normColor->r);
    mTevColor.g = 0.5f + (rate * (anmColor->g - normColor->g) + normColor->g);
    mTevColor.b = 0.5f + (rate * (anmColor->b - normColor->b) + normColor->b);
    mTevColor.a = 0.5f + (rate * (anmColor->a - normColor->a) + normColor->a);
}

void DarkAnmPlayer::setTevColor(ExModel *model) {
    if (mNormColor)
        model->setTevColor(0, mTevColor);
}

ObjDarkAnmPlayer::ObjDarkAnmPlayer(u32 objID) {
    switch (objID) {
    case 4001:
    case 4002:
    case 4003:
    case 4005:
    case 4006:
    case 4007:
    case 4008:
    case 4009:
    case 4010:
        mNormColor = &sObjCarNormColor;
        mColors[0] = &sObjCarBlackColor;
        mColors[1] = &sObjCarGreenColor;
        mColors[2] = &sObjCarRedColor;
        mColors[3] = &sObjCarNightColor;
        break;
    default: // was it that hard to add more cases? not that i mind
        if (objID >= 255 && objID < 266) {
            mNormColor = &sObjJugemNormColor;
            mColors[0] = &sObjJugemBlackColor;
            mColors[1] = &sObjJugemGreenColor;
            mColors[2] = &sObjJugemRedColor;
            mColors[3] = &sObjJugemNightColor;
        }
        break;
    }
    reset();
}

ItemDarkAnmPlayer::ItemDarkAnmPlayer(u32 itemID) {
    mBackColor = nullptr;
    mFrontColor = nullptr;
    mItemTevColor.set(0,0,0,0);

    switch (itemID) {
    case 0:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemGTurtleBlackColor;
        mColors[1] = &sItemGTurtleGreenColor;
        mColors[2] = &sItemGTurtleRedColor;
        mColors[3] = &sItemGTurtleNightColor;
        mBackColor = &sItemGTurtleNormBColor;
        mFrontColor = &sItemGTurtleNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 1:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemGTurtleBlackColor;
        mColors[1] = &sItemGTurtleGreenColor;
        mColors[2] = &sItemGTurtleRedColor;
        mColors[3] = &sItemGTurtleNightColor;
        mBackColor = &sItemGTurtleNormBColor;
        mFrontColor = &sItemGTurtleBigNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 2:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemGTurtleBlackColor;
        mColors[1] = &sItemGTurtleGreenColor;
        mColors[2] = &sItemGTurtleRedColor;
        mColors[3] = &sItemGTurtleNightColor;
        mBackColor = &sItemGTurtleNormBColor;
        mFrontColor = &sItemRTurtleNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 3:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemBananaBlackColor;
        mColors[1] = &sItemBananaGreenColor;
        mColors[2] = &sItemBananaRedColor;
        mColors[3] = &sItemBananaNightColor;
        mBackColor = &sItemBananaNormBColor;
        mFrontColor = &sItemBananaNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 4:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemBananaBlackColor;
        mColors[1] = &sItemBananaGreenColor;
        mColors[2] = &sItemBananaRedColor;
        mColors[3] = &sItemBananaNightColor;
        mBackColor = &sItemBananaNormBColor;
        mFrontColor = &sItemBananaBigNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 5:
    case 0xc:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemKinokoBlackColor;
        mColors[1] = &sItemKinokoGreenColor;
        mColors[2] = &sItemKinokoRedColor;
        mColors[3] = &sItemKinokoNightColor;
        mBackColor = &sItemKinokoNormBColor;
        mFrontColor = &sItemKinokoNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 8:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemBombBlackColor;
        mColors[1] = &sItemBombGreenColor;
        mColors[2] = &sItemBombRedColor;
        mColors[3] = &sItemBombNightColor;
        mBackColor = &sItemBombNormBColor;
        mFrontColor = &sItemBombNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 0xb:
        mNormColor = &mItemTevColor;
        mColors[0] = &sItemYoshiEggBlackColor;
        mColors[1] = &sItemYoshiEggGreenColor;
        mColors[2] = &sItemYoshiEggRedColor;
        mColors[3] = &sItemYoshiEggNightColor;
        mBackColor = &sItemYoshiEggNormBColor;
        mFrontColor = &sItemYoshiEggNormFColor;
        mItemTevColor= *mBackColor;
        break;
    case 0xd:
    mNormColor = &mItemTevColor;
    mColors[0] = &sItemFlyTurtleBlackColor;
    mColors[1] = &sItemFlyTurtleGreenColor;
    mColors[2] = &sItemFlyTurtleRedColor;
    mColors[3] = &sItemFlyTurtleNightColor;
    mBackColor = &sItemFlyTurtleNormBColor;
    mFrontColor = &sItemFlyTurtleNormFColor;
    mItemTevColor= *mBackColor;
    }

    DarkAnmPlayer::reset();
}

void ItemDarkAnmPlayer::calc(const CrsArea &area, f32 rate) {
    if (!mBackColor)
        return;

    DarkAnmPlayer::calc(area);

    KartTevColor *frontColor = mFrontColor;
    mTevColor.r = 0.5f + (rate * (frontColor->r - mTevColor.r) + mTevColor.r);
    mTevColor.g = 0.5f + (rate * (frontColor->g - mTevColor.g) + mTevColor.g);
    mTevColor.b = 0.5f + (rate * (frontColor->b - mTevColor.b) + mTevColor.b);
    mTevColor.a = 0.5f + (rate * (frontColor->a - mTevColor.a) + mTevColor.a);
}

#include "JSystem/JAudio/JASFakeMatch2.h"