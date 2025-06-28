#include "Osako/PlayerSelectApp.h"
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/RaceInfo.h"
#include "Osako/AppMgr.h"
#include "Osako/GameApp.h"
#include "Osako/MainMenuApp.h"
#include "Osako/MapSelectApp.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "kartEnums.h"
#include "types.h"
#include "mathHelper.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

PlayerSelectApp *PlayerSelectApp::mspPlayerSelectApp;
u32 PlayerSelectApp::msCallFlag;

static const char *scpaKartName[] = {
    "Mario",
    "Donkey",
    "Yoshi",
    "Nokonoko",
    "Peach",
    "BabyMario",
    "Wario",
    "Koopa",
    "Luigi",
    "Diddy",
    "Catherine",
    "Patapata",
    "Daisy",
    "BabyLuigi",
    "Waluigi",
    "KoopaJr",
    "Kinopio",
    "Kinopico",
    "BossTeresa", 
    "BossPakkun",
    "Extra"
};

static const char *scpaCharName[] = {
    "",
    "BabyMario","BabyLuigi",
    "Patapata", /*Shikanoko*/ "Nokonoko" /*Koshitantan*/,
    "Peach", "Daisy",
    "Mario", "Luigi",
    "Wario", "Waluigi",
    "Yoshi", "Catherine",
    "Donkey", "Diddy",
    "Koopa", "KoopaJr",
    "Kinopio", "Kinopico",
    "BossTeresa", "BossPakkun"
};

static const char *scpaPadName[] = {
    "--",
    "1P",
    "2P",
    "3P",
    "4P",
    "5P",
    "6P",
    "7P",
    "8P",
};

static const ECharID scaaCharPair[21][2] = {
    {MARIO, LUIGI},
    {DONKEY_KONG, DIDDY_KONG},
    {YOSHI, BIRDO},
    {KOOPATROOPA, PARATROOPA},
    {PEACH, DAISY},
    {BABY_MARIO, BABY_LUIGI},
    {WARIO, WALUIGI},
    {BOWSER, BOWSER_JR},
    {LUIGI, DAISY},
    {DIDDY_KONG, BOWSER_JR},
    {BIRDO, WALUIGI},
    {PARATROOPA, BABY_LUIGI},
    {DAISY, MARIO},
    {BABY_LUIGI, BOWSER_JR},
    {WALUIGI, MARIO},
    {BOWSER_JR, PARATROOPA},
    {TOAD, KOOPATROOPA},
    {TOADETTE, KOOPATROOPA},
    {KOOPATROOPA, KOOPATROOPA},
    {KOOPATROOPA, KOOPATROOPA},
    {MARIO, PEACH},
};

PlayerSelectApp *PlayerSelectApp::create() {
    if (mspPlayerSelectApp == nullptr) {
        mspPlayerSelectApp = NEW_APP PlayerSelectApp();
    } 
    return mspPlayerSelectApp;
}

void PlayerSelectApp::call(u32 callFlag) {
    msCallFlag = callFlag;
    AppMgr::setNextApp(AppMgr::mcPlayerSelect);
}

void PlayerSelectApp::reset() {
    _c = 0;
    mCursorY = 0;
    mKartCount = gRaceInfo.getKartNumber();
    switch (gRaceInfo.getRaceMode()) {
    case BALLOON_BATTLE:
    case ROBBERY_BATTLE: 
    case BOMB_BATTLE: 
    case ESCAPE_BATTLE: 
        mLevel = LVL_50CC;
        break;
    case TIME_ATTACK:
        mLevel = LVL_150CC;
        mKartCount = 1;
        break;
    default:
        mLevel = LVL_100CC;
        break;
    }

    for (u8 i = 0; i < mKartCount; i++) {
        _20[i].mKartIdx = i;
        _20[i].mCharId1 = scaaCharPair[i][0];
        _20[i].mCharId2 = scaaCharPair[i][1];
        _20[i].mPadNo1 = (i < gRaceInfo.getPlayerKartNumber()) ? i + 1 : 0;
        _20[i].mPadNo2 = 0;
        _20[i]._8 = i < gRaceInfo.getPlayerKartNumber();
    }
}

PlayerSelectApp::PlayerSelectApp() : GameApp(0, "Kart Sel", nullptr) {
    reset();
}

PlayerSelectApp::~PlayerSelectApp() {
    mspPlayerSelectApp = nullptr;
}

void PlayerSelectApp::decide() {
    for (int i = 0; i < mKartCount; i++) {
        KartGamePad *pad1 = _20[i].mPadNo1 > 0 ? gpaKartPad[_20[i].mPadNo1 - 1] : nullptr;
        KartGamePad *pad2 = _20[i].mPadNo2 > 0 ? gpaKartPad[_20[i].mPadNo2 - 1] : nullptr;
        gRaceInfo.setKart(i, (EKartID)_20[i].mKartIdx, (ECharID)_20[i].mCharId1, pad1, (ECharID)_20[i].mCharId2, pad2);
        if (i < gRaceInfo.getStatusNumber()) {
            gRaceInfo.setConsoleTarget(i, i, false);
        }
        gRaceInfo.setStartPosIdx(i, i);
    }
    gRaceInfo.setRaceLevel((ERaceLevel)mLevel);
    MapSelectApp::call();
}

void PlayerSelectApp::cancel() {
    MainMenuApp::call();
}

void PlayerSelectApp::draw() {
    System::getJ2DOrtho()->setPort();
    System::getJ2DPrint()->initiate();
    System::getJ2DPrint()->print(80.0f, 60.0f, "Player Select(kari)");

    int y;
    int i;
    int yOffset;
    
    for (i = 0; i < mKartCount; i++) {
        y = (i * 20) + 110;
        System::getJ2DPrint()->print(80.0f, y, "%d:%s", i + 1, scpaKartName[_20[i].mKartIdx]);
        System::getJ2DPrint()->print(240.0f, y , "%s:%s", scpaCharName[_20[i].mCharId1], scpaPadName[_20[i].mPadNo1]);
        System::getJ2DPrint()->print(400.0f, y, "%s:%s",scpaCharName[_20[i].mCharId2], scpaPadName[_20[i].mPadNo2]);    
    }

    System::getJ2DPrint()->print(60.0f, (mCursorY * 20) + 110, ">");
    switch (mLevel) {
    case LVL_50CC:
        System::getJ2DPrint()->print(390.0f, 70.0f, "50cc");
        break;
    case LVL_100CC:
        System::getJ2DPrint()->print(390.0f, 70.0f, "100cc");
        break;
    case LVL_150CC:
        System::getJ2DPrint()->print(390.0f, 70.0f, "150cc");
        break;
    case LVL_MIRROR:
        System::getJ2DPrint()->print(390.0f, 70.0f, "mirror");
        break;
    }

    System::getJ2DPrint()->print(100.0f, 290.0f, "Stick<->:Kart Change");
    System::getJ2DPrint()->print(100.0f, 310.0f, "L + Stick<->:Front Change");
    System::getJ2DPrint()->print(100.0f, 330.0f, "R + Stick<->:Back Change");
    System::getJ2DPrint()->print(100.0f, 350.0f, "Y:Pad Change  Z:Level");
}

void PlayerSelectApp::calc() {
    if (msCallFlag & 1) {
        reset();
        msCallFlag &= ~1;
    }

    if (gpaKartPad[0]->testRepeat(KartGamePad::MAINSTICK_UP)) {
        GetGameAudioMain()->startSystemSe(0x20000);
        if (--mCursorY < 0) {
            mCursorY = mKartCount - 1;
        }
    }
    else if (gpaKartPad[0]->testRepeat(KartGamePad::MAINSTICK_DOWN)) {
        GetGameAudioMain()->startSystemSe(0x20000);
        if (++mCursorY >= mKartCount) {
            mCursorY = 0;
        }
    }
    else if (gpaKartPad[0]->testRepeat(KartGamePad::MAINSTICK_LEFT)) {
        GetGameAudioMain()->startSystemSe(0x20000);
        if (gpaKartPad[0]->testButton(KartGamePad::L)) {
            if (--_20[this->mCursorY].mCharId1 <= cCharIDNone) {
                _20[this->mCursorY].mCharId1 = PETEY;
            }
        }
        else if (gpaKartPad[0]->testButton(KartGamePad::R)) {
            if (--_20[this->mCursorY].mCharId2 <= cCharIDNone) {
                _20[this->mCursorY].mCharId2 = PETEY;
            }
        }
        else {
            if (--_20[this->mCursorY].mKartIdx < 0) {
                _20[this->mCursorY].mKartIdx = PARADE_KART;
            }
            _20[this->mCursorY].mCharId1 = scaaCharPair[_20[this->mCursorY].mKartIdx][0];
            _20[this->mCursorY].mCharId2 = scaaCharPair[_20[this->mCursorY].mKartIdx][1];
        }
    }
    else if (gpaKartPad[0]->testRepeat(KartGamePad::MAINSTICK_RIGHT)) {
        GetGameAudioMain()->startSystemSe(0x20000);
        if (gpaKartPad[0]->testButton(KartGamePad::L)) {
            if (++_20[this->mCursorY].mCharId1 >= cCharIDMax) {
                _20[this->mCursorY].mCharId1 = BABY_MARIO;
            }
        }
        else if (gpaKartPad[0]->testButton(KartGamePad::R)) {
            if (++_20[this->mCursorY].mCharId2 >= cCharIDMax) {
                _20[this->mCursorY].mCharId2 = BABY_MARIO;
            }
        }
        else {
            if (++_20[this->mCursorY].mKartIdx >= cKartIDMax) {
                _20[this->mCursorY].mKartIdx = RED_FIRE;
            }
            _20[this->mCursorY].mCharId1 = scaaCharPair[_20[this->mCursorY].mKartIdx][0];
            _20[this->mCursorY].mCharId2 = scaaCharPair[_20[this->mCursorY].mKartIdx][1];
        }
    }
    else if (gpaKartPad[0]->testTrigger(KartGamePad::Y)) {
        GetGameAudioMain()->startSystemSe(0x20003);
        changePad(mCursorY);
    }

    for (u8 i = 1; i < gRaceInfo.getPlayerKartNumber(); i++) {
        if (gpaKartPad[i]->testRepeat(KartGamePad::MAINSTICK_LEFT)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            if (gpaKartPad[i]->testButton(KartGamePad::L)) {
                if (--_20[i].mCharId1 <= cCharIDNone) {
                    _20[i].mCharId1 = PETEY;
                }
            }
            else if (gpaKartPad[i]->testButton(KartGamePad::R)) {
                if (--_20[i].mCharId2 <= cCharIDNone) {
                    _20[i].mCharId2 = PETEY;
                }
            }
            else {
                if (--_20[i].mKartIdx < 0) {
                    _20[i].mKartIdx = PARADE_KART;
                }
                _20[i].mCharId1 = scaaCharPair[_20[i].mKartIdx][0];
                _20[i].mCharId2 = scaaCharPair[_20[i].mKartIdx][1];
            }
        }
        else if (gpaKartPad[i]->testRepeat(KartGamePad::MAINSTICK_RIGHT)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            if (gpaKartPad[i]->testButton(KartGamePad::L)) {
                if (++_20[i].mCharId1 >= cCharIDMax) {
                    _20[i].mCharId1 = BABY_MARIO;
                }
            }
            else if (gpaKartPad[i]->testButton(KartGamePad::R)) {
                if (++_20[i].mCharId2 >= cCharIDMax) {
                    _20[i].mCharId2 = BABY_MARIO;
                }
            }
            else {
                if (++_20[i].mKartIdx >= cKartIDMax) {
                    _20[i].mKartIdx = RED_FIRE;
                }
                _20[i].mCharId1 = scaaCharPair[_20[i].mKartIdx][0];
                _20[i].mCharId2 = scaaCharPair[_20[i].mKartIdx][1];
            }
        }
    }

    if (gpaKartPad[0]->testTrigger(KartGamePad::A)) {
        GetGameAudioMain()->startSystemSe(0x20002);
        decide();
    }
    else if (gpaKartPad[0]->testTrigger(KartGamePad::B)) {
        GetGameAudioMain()->startSystemSe(0x20004);
        cancel();
    }
    else if (gpaKartPad[0]->testTrigger(KartGamePad::X)) {
        GetGameAudioMain()->startSystemSe(0x20003);
        doTag();
    }
    else if (gpaKartPad[0]->testTrigger(KartGamePad::Z)) {
        GetGameAudioMain()->startSystemSe(0x20003);
        if (++mLevel >= LVL_MAX) {
            mLevel = LVL_50CC;
        }
    }
}

void PlayerSelectApp::doTag() {
    s8 padNo = 1;
    for (u8 i = 0; i < mKartCount; i++) {
        if (i < gRaceInfo.getStatusNumber() && padNo <= 4) {
            _20[i].mPadNo1 = padNo++;
            _20[i].mPadNo2 = padNo++;
            _20[i]._8 = 2;
        }
        else {
            _20[i].mPadNo1 = 0;
            _20[i].mPadNo2 = 0;
            _20[i]._8 = 0;
        }
    }
}

void PlayerSelectApp::changePad(u8 idx) {
    if (++_20[idx]._8 > 2) {
        _20[idx]._8 = 0;
    }

    s8 padNo = 1;
    for (u8 i = 0; i < mKartCount; i++) {
        if (_20[i]._8 != 0) {
            _20[i].mPadNo1 = padNo++;
            if (_20[i]._8 > 1 && padNo <= 16) {
                _20[i].mPadNo2 = padNo++;
            }
            else {
                _20[i].mPadNo2 = 0;
                _20[i]._8 = 1;                
            }
        }
        else {
            _20[i].mPadNo1 = 0;
            _20[i].mPadNo2 = 0;
            _20[i]._8 = 0;
        }

        if (padNo > 16)
            break;
    }
}
