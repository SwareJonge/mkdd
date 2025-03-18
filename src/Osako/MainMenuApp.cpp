#include "Osako/MainMenuApp.h"
#include "Kameda/SceneApp.h"
#include "Kameda/SequenceInfo.h"
#include "Kaneshige/Movie/MovieApp.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Osako/AppMgr.h"
#include "Osako/PlayerSelectApp.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemRecord.h"
#include "Osako/TestViewApp.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "dolphin/os/OSTime.h"
#include "kartEnums.h"
#include "types.h"
#include "mathHelper.h"

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
    _C = 0;
    _1C = 0;
    _1D = 0;
    _1E = 1;
    _1F = 0;
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
    switch (_1C) {
    case 0: {
        switch(_1D) {
        case 1:
            _1C = 2;
            break;
        case 0: {
            if (_20 == 3) {
                _1C = 4;
            }
            else {
                _1C = 3;
            }
            break;
        }  
        case 2:
            _1C = 7;
            break;
        }
        break;
    }
    case 1:
    case 2:
    case 5:
        _1C = 0;
        break;
    case 3:
        _1C = 1;
        break;
    case 4:
        _1C = 3;
        break;
    case 6:
        _1C = 5;
        break;
    case 7:
        _1C = 6;
        break;
    default:
        break;
    }
}

void MainMenuApp::down() {
    switch (_1C) {
    case 0: {
        switch(_1D) {
        case 1:
            _1C = 2;
            break;
        case 0: {
            _1C = 1;
            break;
        }  
        case 2:
            _1C = 5;
            break;
        }
        break;
    }
    case 1:
        _1C = 3;
        break;
    case 2:
    case 4:
    case 7:
        _1C = 0;
        break;
    case 3: {
        if (_20 == 3) {
            _1C = 4;
        }
        else {
            _1C = 0;
        }
        break;
    }
    case 5:
        _1C = 6;
        break;
    case 6:
        _1C = 7;
        break;
    default:
        break;
    }
}

void MainMenuApp::left() {}

void MainMenuApp::right() {}

void MainMenuApp::decide() {
    switch(_1D) {
    case 1: {
        switch(_1F) {
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
            _C = 4;
            break;
        }
        case 3:
            MovieApp::create();
            MovieApp::getMovieApp()->loadOpeningData();
            _C = 5;
            break;
        case 4: {
            gRaceInfo.settingForStaffRoll(false);
            ResMgr::loadCourseData(ENDING, ResMgr::mcOrder_2);
            _C = 4;
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
            gRaceInfo.setRace(GRAND_PRIX, 8, _1E, _1E, _1E);
            break;
        case 2: {
            int count = 4;
            if (_1E < 3)
                count = _1E;
            int status = 4;
            if (_1E <= 4)
                status = _1E;
            gRaceInfo.setRace(VERSUS_RACE, _1E, _1E, count, status);
            if (_1E == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
            gRaceInfo.setItemSlotType(gSystemRecord.mItemSlotType);
            break;
        }
        case 3: {
            int count = 4;
            if (_1E < 3)
                count = _1E;
            int status = 4;
            if (_1E <= 4)
                status = _1E;

            switch (_21) {
            case 0:
                gRaceInfo.setRace(BALLOON_BATTLE, _1E, _1E, count, status);
                break;
            case 1:
                gRaceInfo.setRace(ROBBERY_BATTLE, _1E, _1E, count, status);
                break;
            case 2:
                gRaceInfo.setRace(BOMB_BATTLE, _1E, _1E, count, status);
                break;
            case 3: {
                gRaceInfo.setRace(ESCAPE_BATTLE, _1E, _1E, count, status);
                break;
            }
            }
            if (_1E == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
        }
        }
        gRaceInfo.setRandomSeed(OSGetTick());
        PlayerSelectApp::call(1);
        break;
    }
    case 2: {
        switch(_1C) {
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

void MainMenuApp::cancel() {}

void MainMenuApp::draw() {

}

void MainMenuApp::calc() {}
