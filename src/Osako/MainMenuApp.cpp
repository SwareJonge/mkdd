#include "Osako/MainMenuApp.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JUtility/TColor.h"
#include "Kameda/SceneApp.h"
#include "Kameda/SequenceInfo.h"
#include "Kaneshige/Movie/MovieApp.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Osako/AppMgr.h"
#include "Osako/AwardApp.h"
#include "Osako/KartPadData.h"
#include "Osako/PlayerSelectApp.h"
#include "Osako/RaceApp.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/SystemRecord.h"
#include "Osako/TestViewApp.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "dolphin/os/OSTime.h"
#include "kartEnums.h"
#include "types.h"
#include "mathHelper.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

static const char *scpaDebugMode[] = {
    "Race",
    "Test",
    "Record"
};

static const char *scpaTestMode[] = {
    "Scene",
    "View",
    "Award",
    "Movie",
    "Ending"
};

static const char *scpaGameMode[] = {
    "TimeAttack",
    "GrandPrix",
    "Versus",
    "Battle"
};

static const char *scpaBattleMode[] = {
    "Balloon",
    "Robbery",
    "Bomb",
    "Escape"
};

static const char *scpaGameFlag[] = {
    "Kinopio combi",
    "Boss combi",
    "Mirror",
    "Special cup",
    "All tour cup",
    "Ending",
    "Special ending",
    "Mini Luigi",
    "Mini Mario"
};

static const char *scpaSecretKart[] = {
    "Baby luigi",
    "Patapata",
    "Diddy",
    "KoopaJr",
    "Kinopio",
    "Kinopico",
    "Luigi",
    "Daisy",
    "Catherine",
    "Waluigi",
    "Pakkun",
    "Teressa",
    "Extra"
};

static const char *scpaCupName[] = {
    "Mushroom",
    "Flower",
    "Star",
    "Special",
    "Allcup"
};

static const char *scpaLevelName[] = {
    "50cc",
    "100cc",
    "150cc",
    "Mirror"
};

MainMenuApp *MainMenuApp::mspMainMenuApp;

MainMenuApp *MainMenuApp::create() {
    if (mspMainMenuApp == nullptr) {
        mspMainMenuApp = NEW_APP MainMenuApp();
    } 
    return mspMainMenuApp;
}

void MainMenuApp::call() {
    AppMgr::setNextApp(AppMgr::mcMainMenu);
}

MainMenuApp::MainMenuApp() : GameApp(0, "Menu", nullptr) {
    _14 = 0;
    _18 = 0;
    _c = 0;
    _1c = 0;
    _1d = 0;
    _1e = 1;
    _1f = 0;
    _20 = 0;
    _21 = 0;
    mGameFlagIdx = 0;
    mSecretKartId = 0;
    mCupIdx = 0;
    mLevelIdx = 0;
    _26 = 0;
}

MainMenuApp::~MainMenuApp() {
    mspMainMenuApp = nullptr;
}

void MainMenuApp::up() {
    switch (_1c) {
    case 0: {
        switch(_1d) {
        case 1:
            _1c = 2;
            break;
        case 0: {
            if (_20 == 3) {
                _1c = 4;
            }
            else {
                _1c = 3;
            }
            break;
        }  
        case 2:
            _1c = 7;
            break;
        }
        break;
    }
    case 1:
    case 2:
    case 5:
        _1c = 0;
        break;
    case 3:
        _1c = 1;
        break;
    case 4:
        _1c = 3;
        break;
    case 6:
        _1c = 5;
        break;
    case 7:
        _1c = 6;
        break;
    default:
        break;
    }
}

void MainMenuApp::down() {
    switch (_1c) {
    case 0: {
        switch(_1d) {
        case 1:
            _1c = 2;
            break;
        case 0: {
            _1c = 1;
            break;
        }  
        case 2:
            _1c = 5;
            break;
        }
        break;
    }
    case 1:
        _1c = 3;
        break;
    case 2:
    case 4:
    case 7:
        _1c = 0;
        break;
    case 3: {
        if (_20 == 3) {
            _1c = 4;
        }
        else {
            _1c = 0;
        }
        break;
    }
    case 5:
        _1c = 6;
        break;
    case 6:
        _1c = 7;
        break;
    default:
        break;
    }
}

void MainMenuApp::left() {
    switch(_1c) {
    case 0: {
        if (--_1d < 0) {
            _1d = 2;
        }
        break;
    }
    case 1: {
        if (--_1e <= 0) {
            _1e = 8;
        }
        if ((_20 == 0) && (_1e != 1)) {
            _20 = 1;
        }
        if (_20 == 1) {
            if  (_1e > 2) {
                _20 = 2;
            }
        }
        break;
    }
    case 2: {
        if (--_1f < 0) {
            _1f = 4;
        }
        break;
    }
    case 3: {
        if (--_20 < 0) {
            _20 = 3;
        }
        if ((_20 == 1) && (_1e > 2)) {
            _20 = 3;
        }
        if (_20 == 0) {
            if (_1e != 1) {
                _20 = 3;
            }
        }
        break;
    }
    case 4:{
        if (--_21 < 0) {
            _21 = 3;
        }
        break;
    }
    case 5: {
        if (--mGameFlagIdx < 0) {
            mGameFlagIdx = 8;
        }
        break;
    }
    case 6: {
        if (--mSecretKartId < 0) {
            mSecretKartId = 0xc;
        }
        break;
    }
    case 7: {
        if (--_26 < 0) {
            _26 = 2;

        }
        break;
    }
    default:
        break;
    }
}

void MainMenuApp::right() {
switch(_1c) {
    case 0: {
        if (++_1d >= 3) {
            _1d = 0;
        }
        break;
    }
    case 1: {
        if (++_1e > 8) {
            _1e = 1;
        }
        if ((_20 == 0) && (_1e != 1)) {
            _20 = 1;
        }
        if (_20 == 1) {
            if  (_1e > 2) {
                _20 = 2;
            }
        }
        break;
    }
    case 2: {
        if (++_1f >= 5) {
            _1f = 0;
        }
        break;
    }
    case 3: {
        if (++_20 >= 4) {
            _20 = 0;
        }
        if ((_20 == 0) && (_1e != 1)) {
            _20 = 1;
        }
        if (_20 == 1) {
            if (_1e > 2) {
                _20 = 2;
            }
        }
        break;
    }
    case 4:{
        if (++_21 >= 4) {
            _21 = 0;
        }
        break;
    }
    case 5: {
        if (++mGameFlagIdx >= 9) {
            mGameFlagIdx = 0;
        }
        break;
    }
    case 6: {
        if (++mSecretKartId >= 0xd) {
            mSecretKartId = 0;
        }
        break;
    }
    case 7: {
        if (++_26 > 2) {
            _26 = 0;
        }
        break;
    }
    default:
        break;
    }
}

void MainMenuApp::decide() {
    switch(_1d) {
    case 1: {
        switch(_1f) {
        case 0:
            SceneApp::call();
            break;
        case 1:
            TestViewApp::call();
            break;
        case 2: {
            gRaceInfo.settingForAwardDemo();
            gSequenceInfo.set_318_14(0, 0);
            gSequenceInfo.set_318_14(1, 1);
            
            gSequenceInfo.set_318_RaceTime(0, RaceTime(119999));
            gSequenceInfo.set_318_kartId(0, RED_FIRE);
            gSequenceInfo.set_318_charId(0, 0, MARIO);
            gSequenceInfo.set_318_charId(0, 1, LUIGI);
            gSequenceInfo.set_354(1);
            gSequenceInfo.setRaceLevel(LVL_150CC);        
            gSequenceInfo.set_318_Pad(0, gpaKartPad[0]);
            ResMgr::loadCourseData(AWARD, ResMgr::mcOrder_2);
            _c = 4;
            break;
        }
        case 3:
            MovieApp::create();
            MovieApp::getMovieApp()->loadOpeningData();
            _c = 5;
            break;
        case 4: {
            gRaceInfo.settingForStaffRoll(false);
            ResMgr::loadCourseData(ENDING, ResMgr::mcOrder_2);
            _c = 4;
            break;
        }
        }
        break;
    }
    case 0: {
        switch (_20) {
        case 0:
            gRaceInfo.setRace(TIME_ATTACK, 4, 4, 1, 1);
            break;
        case 1:
            gRaceInfo.setRace(GRAND_PRIX, 8, _1e, _1e, _1e);
            break;
        case 2: {
            int count = 4;
            if (_1e < 3)
                count = _1e;
            int status = 4;
            if (_1e <= 4)
                status = _1e;
            gRaceInfo.setRace(VERSUS_RACE, _1e, _1e, count, status);
            if (_1e == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
            gRaceInfo.setItemSlotType(gSystemRecord.mItemSlotType);
            break;
        }
        case 3: {
            int count = 4;
            if (_1e < 3)
                count = _1e;
            int status = 4;
            if (_1e <= 4)
                status = _1e;

            switch (_21) {
            case 0:
                gRaceInfo.setRace(BALLOON_BATTLE, _1e, _1e, count, status);
                break;
            case 1:
                gRaceInfo.setRace(ROBBERY_BATTLE, _1e, _1e, count, status);
                break;
            case 2:
                gRaceInfo.setRace(BOMB_BATTLE, _1e, _1e, count, status);
                break;
            case 3: {
                gRaceInfo.setRace(ESCAPE_BATTLE, _1e, _1e, count, status);
                break;
            }
            }
            if (_1e == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
        }
        }
        gRaceInfo.setRandomSeed(OSGetTick());
        PlayerSelectApp::call(1);
        break;
    }
    case 2: {
        switch(_1c) {
        case 5:
           gSystemRecord.unlockGame((SystemRecord::GameFlag)mGameFlagIdx);
           break;
        case 6:
           gSystemRecord.unlockKart(mSecretKartId);
           break;
        case 7: {
            GPRecord rec;            
            rec.set(7, 5, 0, mLevelIdx, _26, 30, false, "OSK", RaceTime(123456));
            gSystemRecord.setGPRecord((ERaceGpCup)mCupIdx, (ERaceLevel)mLevelIdx, rec);
            break;
        }
        }
    }
    }

}

void MainMenuApp::cancel() {
}

void MainMenuApp::draw() {\
    int x = 70;
    System::getJ2DOrtho()->setPort();
    J2DPrint *print = System::getJ2DPrint();
    print->setWhiteColor(0xffffffff);
    print->initiate();
    print->print(80.0f, 80.0f, "Main Menu(kari)");

    if (_1c == 1) {
        print->print(35.0f,120.0f,"-");
    }

    for (u8 i = 0; i < 3; i++) {
        if (_1d == i) {
            print->setWhiteColor(0xffffffff);
            print->initiate();
        }
        else {
            print->setWhiteColor(0x80808080);
            print->initiate();
        }

        print->print(x, 145.0f, scpaDebugMode[i]);
        x += print->getWidth(scpaDebugMode[i]) + 35;
    }

    x = 70;
    switch(_1d) {
    case 0: {
        print->setWhiteColor(0xffffffff);
        print->initiate();
        if (_1c == 1) {
            print->print(35.0f,145.0f,"-");
        }

        for (u8 i = 0; i < 9; i++) {
            if (_1e == i) {
                print->setWhiteColor(0xffffffff);
                print->initiate();
            }
            else {
                print->setWhiteColor(0x80808080);
                print->initiate();
            }

            print->print(x, 145.0f, "%dP",scpaGameFlag[i]);
            x += print->getWidth("%dP",scpaGameFlag[i]) + 35;
        }
        break;
    }
    }

    print->setWhiteColor(0xffffffff);
    print->initiate();
}

void MainMenuApp::calc() {
    if (_18 == 1) {
        _14++;
    }
    switch (_c) {
    case 0: {
        GetGameAudioMain()->stopSequenceBgm(30);
        SYSTEM_StartFadeIn(30);
        _18 = 1;
        _c = 1;
        break;
    }
    case 1: {
        if ((_14 / 60.0f) >= 0.5f) {
            _18 = 2;
            _c = 3;
        }
        break;
    }
    case 2:
        break;
    case 3: {
        if (gGamePad1P.testRepeat(KartGamePad::MAINSTICK_UP)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            up();
        }
        else if (gGamePad1P.testRepeat(KartGamePad::MAINSTICK_DOWN)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            down();
        }
        else if (gGamePad1P.testRepeat(KartGamePad::MAINSTICK_LEFT)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            left();
        }
        else if (gGamePad1P.testRepeat(KartGamePad::MAINSTICK_RIGHT)) {
            GetGameAudioMain()->startSystemSe(0x20000);
            right();
        }
        if (gGamePad1P.testTrigger(KartGamePad::A)) {
            GetGameAudioMain()->startSystemSe(0x20002);
            decide();
        }
        else if (gGamePad1P.testTrigger(KartGamePad::B)) {
            GetGameAudioMain()->startSystemSe(0x20004);
            AppMgr::deleteCurrentApp();
            SequenceApp::call(Scene::SCENE_TITLE);
        }
        else if (gGamePad1P.testTrigger(KartGamePad::Y)) {
            if (_1d == 2) {
                GetGameAudioMain()->startSystemSe(0x20002);
                switch (_1c) {
                case 5: {
                    for (int i = 0; i < 9; i++) {
                        gSystemRecord.unlockGame((SystemRecord::GameFlag)i);
                    }
                    break;
                }
                case 6: {
                    for (int i = 0; i < 0xd; i++) {
                        gSystemRecord.unlockKart(i);
                    }
                    break;
                }
                }
            }
        }

        if (_1c == 7) {
            if (gGamePad1P.testTrigger(KartGamePad::CSTICK_UP)) {
                if (--mLevelIdx < 0) {
                    mLevelIdx = 3;
                }
            }
            else if (gGamePad1P.testTrigger(KartGamePad::CSTICK_DOWN)) {
                if (++mLevelIdx > 3) {
                    mLevelIdx = 0;
                }
            }
            else if (gGamePad1P.testTrigger(KartGamePad::CSTICK_LEFT)) {
                if (--mCupIdx < 0) {
                    mCupIdx = 4;
                }
            }
            else if (gGamePad1P.testTrigger(KartGamePad::CSTICK_RIGHT)) {
                if (++mCupIdx > 4) {
                    mCupIdx = 0;
                }
            }
        }
        break;
    }
    case 5: {
        if (ResMgr::isFinishLoadingArc(ResMgr::mcArcOpening)) {
            MovieApp::call();
        }
        break;
    }
    case 4: {
        if (ResMgr::isFinishLoadingArc(ResMgr::mcArcCourse)) {
            AppMgr::deleteCurrentApp();
            if (_1d == 1 && _1f == 2) {
                AwardApp::call();
            }
            else {
                RaceApp::call();
            }
        }
        break;
    }
    }
}
