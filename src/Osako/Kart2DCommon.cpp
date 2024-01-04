#include "Osako/Kart2DCommon.h"
#include "Osako/ResMgr.h"

const char *cpaCharName[20] = {
    "baby_mario", "baby_luigi",
    "pata", "noko",
    "peach", "daisy",
    "mario", "luigi",
    "wario", "waluigi",
    "yoshi", "catherine",
    "donky", "diddy",
    "koopa", "koopa_jr",
    "kinopio", "kinopico",
    "teresa", "pakkun"};

const char *cpaKartName[21] = {
    "Mario", "Donkey",
    "Yoshi", "Nokonoko",
    "Peach", "BabyMario",
    "Wario", "Koopa",
    "Luigi", "Diddy",
    "Catherine", "Patapata",
    "Daisy", "BabyLuigi",
    "Waluigi", "KoopaJr",
    "Kinopio", "Kinopico",
    "Teresa", "Pakkun",
    "Extra"};

const char *cpaPartsName[4] = {
    "kazu3Dten", "chara_window1", "Spark1", "White3_1"};

const char *cpaPartsLocName[1] = {
    "Word_Points"};

const char *cpaCupName[5] = {
    "MUSHROOM", "FLOWER", "STAR", "SPECIAL", "REVERSE2"};

const char *cpaCupIcon[5] = {
    "Kinoko", "Flower", "Star", "Special", "Reverse2"};

const char *cpaBattleIcon[3] = {
    "Balloon", "Bomb", "Shine"};

const char *cpaMarkName[5] = {
    "_Number_Plus", "_Number_XLine", "Exclamation", "Question", "Period"};

#pragma auto_inline off // TODO: hack?

Kart2DCommon *Kart2DCommon::mspKart2DCommon;

void Kart2DCommon::changeNumberTexture(int num, J2DPicture **paPicture, u8 places, bool showZero, bool drawMinus)
{
    if (num < 0)
        return;

    for (int place = 0; place < places; place++)
    {
        // clang-format off
#line 130 
        JUT_ASSERT(paPicture[ place ]->getTypeID() == J2DPaneType_Picture)
        // clang-format on

        // get remainder of num / 10, then divide num by 10 again to get each digit
        // eg if we have the number 420 it'll get 0 on the first iteration, 2 on the second etc.
        int tenths = num % 10;
        num /= 10;

        if (drawMinus)
            paPicture[place]->changeTexture(mMark[1], 0);
        else
            paPicture[place]->changeTexture(mNumberTex[tenths], 0);

        if (place != 0 && tenths == 0 && !showZero && num == 0)
            paPicture[place]->hide();
        else
            paPicture[place]->show();
    }
}

ResTIMG *Kart2DCommon::getAsciiTexture(char symbol)
{
    if (symbol >= 'A' && symbol <= 'Z')
        return mFontData[symbol - 'A'];

    if (symbol >= 'a' && symbol <= 'z')
        return mFontData[symbol - 'a'];

    if (symbol >= '0' && symbol <= '9')
        return mNumberTex[symbol - '0'];

    if (symbol == '+')
        return mMark[0];
    if (symbol == '-')
        return mMark[1];
    if (symbol == '!')
        return mMark[2];
    if (symbol == '?')
        return mMark[3];
    if (symbol == '.')
        return mMark[4];
    if (symbol == ' ')
        return mAlphaTex;

    return mMark[1];
}

void Kart2DCommon::changeStringTexture(const char *aString, J2DPicture **paPicture, u8 places)
{
#line 211
    JUT_ASSERT(aString)

    bool show = true;
    for (int place = 0; place < places; place++)
    {
        // clang-format off
#line 215 
        JUT_ASSERT(paPicture[ place ]->getTypeID() == J2DPaneType_Picture)
        // clang-format on

        if (show)
        {
            char sym = aString[place];
            if (sym == '\0')
            {
                show = false;
                paPicture[place]->hide();
            }
            else
            {
                paPicture[place]->changeTexture(getAsciiTexture(sym), 0);
                paPicture[place]->show();
            }
        }
        else
        {
            paPicture[place]->hide();
        }
    }
}

Kart2DCommon::Kart2DCommon()
{
    int i;
    char c;
    char buf[0x40];

    for (i = 0; i < 11; i++)
    {
        snprintf(buf, sizeof(buf), "timg/MarioFont_Number_%d.bti", i);
        mNumberTex[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0, c = 'A'; c <= 'Z'; c++, i++)
    {
        snprintf(buf, sizeof(buf), "timg/MarioFont_%c.bti", c);
        mFontData[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0; i < 5; i++)
    {
        snprintf(buf, sizeof(buf), "timg/MarioFont%s.bti", cpaMarkName[i]);
        mMark[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    mAlphaTex = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, "timg/AlphTex.bti");

    for (i = 1; i < cCharIDMax; i++) // ECharID starts at 1 so ig that's why this loop starts at 1
    {
        snprintf(buf, sizeof(buf), "timg/%s.bti", cpaCharName[i - 1]);
        mCharIconTex[i - 1] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0; i < cKartIDMax; i++)
    {
        snprintf(buf, sizeof(buf), "timg/IndexKart%s.bti", cpaKartName[i]);
        mKartIconTex[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0; i < 4; i++)
    {
        snprintf(buf, sizeof(buf), "timg/%s.bti", cpaPartsName[i]);
        mPartName[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0; i < 1; i++)
    {
        snprintf(buf, sizeof(buf), "timg/%s.bti", cpaPartsLocName[i]);
        mPartsLocName[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcMRAMLoc, buf);
    }

    for (i = 0; i < 8; i++)
    {
        snprintf(buf, sizeof(buf), "timg/position1_%d.bti", i + 1);
        mPosImg[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcMRAMLoc, buf);
    }

    for (i = 0; i < CUP_MAX; i++)
    {
        snprintf(buf, sizeof(buf), "timg/ResultCupName_%s_CUP.bti", cpaCupName[i]);
        mCupNameTex[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcMRAMLoc, buf);

        snprintf(buf, sizeof(buf), "timg/Cup_Pict_%s.bti", cpaCupIcon[i]);
        mCupIconTex[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }

    for (i = 0; i < 3; i++)
    {
        snprintf(buf, sizeof(buf), "timg/Cup_Pict_%s.bti", cpaBattleIcon[i]);
        mBattleIconTex[i] = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcRace2D, buf);
    }
}