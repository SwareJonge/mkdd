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
    mCursorPos = 0;
    mDebugMode = 0;
    mPlayerCount = 1;
    mTestMode = 0;
    mGameMode = 0;
    mBattleMode = 0;
    mGameFlagIdx = 0;
    mSecretKartId = 0;
    mCupIdx = 0;
    mLevelIdx = 0;
    mRankIdx = 0;
}

MainMenuApp::~MainMenuApp() {
    mspMainMenuApp = nullptr;
}

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

void MainMenuApp::up() {
    switch (mCursorPos) {
    case 0: {
        switch(mDebugMode) {
        case 1:
            mCursorPos = 2;
            break;
        case 0: {
            if (mGameMode == 3) {
                mCursorPos = 4;
            }
            else {
                mCursorPos = 3;
            }
            break;
        }  
        case 2:
            mCursorPos = 7;
            break;
        }
        break;
    }
    case 1:
    case 2:
    case 5:
        mCursorPos = 0;
        break;
    case 3:
        mCursorPos = 1;
        break;
    case 4:
        mCursorPos = 3;
        break;
    case 6:
        mCursorPos = 5;
        break;
    case 7:
        mCursorPos = 6;
        break;
    default:
        break;
    }
}

void MainMenuApp::down() {
    switch (mCursorPos) {
    case 0: {
        switch(mDebugMode) {
        case 1:
            mCursorPos = 2;
            break;
        case 0: {
            mCursorPos = 1;
            break;
        }  
        case 2:
            mCursorPos = 5;
            break;
        }
        break;
    }
    case 1:
        mCursorPos = 3;
        break;
    case 2:
    case 4:
    case 7:
        mCursorPos = 0;
        break;
    case 3: {
        if (mGameMode == 3) {
            mCursorPos = 4;
        }
        else {
            mCursorPos = 0;
        }
        break;
    }
    case 5:
        mCursorPos = 6;
        break;
    case 6:
        mCursorPos = 7;
        break;
    default:
        break;
    }
}

void MainMenuApp::left() {
    switch(mCursorPos) {
    case 0: {
        if (--mDebugMode < 0) {
            mDebugMode = 2;
        }
        break;
    }
    case 1: {
        if (--mPlayerCount <= 0) {
            mPlayerCount = 8;
        }
        if ((mGameMode == 0) && (mPlayerCount != 1)) {
            mGameMode = 1;
        }
        if (mGameMode == 1) {
            if  (mPlayerCount > 2) {
                mGameMode = 2;
            }
        }
        break;
    }
    case 2: {
        if (--mTestMode < 0) {
            mTestMode = 4;
        }
        break;
    }
    case 3: {
        if (--mGameMode < 0) {
            mGameMode = 3;
        }
        if ((mGameMode == 1) && (mPlayerCount > 2)) {
            mGameMode = 3;
        }
        if (mGameMode == 0) {
            if (mPlayerCount != 1) {
                mGameMode = 3;
            }
        }
        break;
    }
    case 4:{
        if (--mBattleMode < 0) {
            mBattleMode = 3;
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
        if (--mRankIdx < 0) {
            mRankIdx = 2;

        }
        break;
    }
    default:
        break;
    }
}

void MainMenuApp::right() {
switch(mCursorPos) {
    case 0: {
        if (++mDebugMode >= 3) {
            mDebugMode = 0;
        }
        break;
    }
    case 1: {
        if (++mPlayerCount > 8) {
            mPlayerCount = 1;
        }
        if ((mGameMode == 0) && (mPlayerCount != 1)) {
            mGameMode = 1;
        }
        if (mGameMode == 1) {
            if  (mPlayerCount > 2) {
                mGameMode = 2;
            }
        }
        break;
    }
    case 2: {
        if (++mTestMode >= 5) {
            mTestMode = 0;
        }
        break;
    }
    case 3: {
        if (++mGameMode >= 4) {
            mGameMode = 0;
        }
        if ((mGameMode == 0) && (mPlayerCount != 1)) {
            mGameMode = 1;
        }
        if (mGameMode == 1) {
            if (mPlayerCount > 2) {
                mGameMode = 2;
            }
        }
        break;
    }
    case 4:{
        if (++mBattleMode >= 4) {
            mBattleMode = 0;
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
        if (++mRankIdx > 2) {
            mRankIdx = 0;
        }
        break;
    }
    default:
        break;
    }
}

void MainMenuApp::decide() {
    switch(mDebugMode) {
    case 1: {
        switch(mTestMode) {
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
        switch (mGameMode) {
        case 0:
            gRaceInfo.setRace(TIME_ATTACK, 4, 4, 1, 1);
            break;
        case 1:
            gRaceInfo.setRace(GRAND_PRIX, 8, mPlayerCount, mPlayerCount, mPlayerCount);
            break;
        case 2: {
            int count = 4;
            if (mPlayerCount < 3)
                count = mPlayerCount;
            int status = 4;
            if (mPlayerCount <= 4)
                status = mPlayerCount;
            gRaceInfo.setRace(VERSUS_RACE, mPlayerCount, mPlayerCount, count, status);
            if (mPlayerCount == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
            gRaceInfo.setItemSlotType(gSystemRecord.mItemSlotType);
            break;
        }
        case 3: {
            int count = 4;
            if (mPlayerCount < 3)
                count = mPlayerCount;
            int status = 4;
            if (mPlayerCount <= 4)
                status = mPlayerCount;

            switch (mBattleMode) {
            case 0:
                gRaceInfo.setRace(BALLOON_BATTLE, mPlayerCount, mPlayerCount, count, status);
                break;
            case 1:
                gRaceInfo.setRace(ROBBERY_BATTLE, mPlayerCount, mPlayerCount, count, status);
                break;
            case 2:
                gRaceInfo.setRace(BOMB_BATTLE, mPlayerCount, mPlayerCount, count, status);
                break;
            case 3: {
                gRaceInfo.setRace(ESCAPE_BATTLE, mPlayerCount, mPlayerCount, count, status);
                break;
            }
            }
            if (mPlayerCount == 3) {
                gRaceInfo.setConsoleTarget(3, 0, true);
            }
        }
        }
        gRaceInfo.setRandomSeed(OSGetTick());
        PlayerSelectApp::call(1);
        break;
    }
    case 2: {
        switch(mCursorPos) {
        case 5:
           gSystemRecord.unlockGame((SystemRecord::GameFlag)mGameFlagIdx);
           break;
        case 6:
           gSystemRecord.unlockKart(mSecretKartId);
           break;
        case 7: {
            GPRecord rec;            
            rec.set(7, 5, 0, mLevelIdx, mRankIdx, 30, false, "OSK", RaceTime(123456));
            gSystemRecord.setGPRecord((ERaceGpCup)mCupIdx, (ERaceLevel)mLevelIdx, rec);
            break;
        }
        }
    }
    }
}

void MainMenuApp::cancel() {
    AppMgr::deleteCurrentApp();
    SequenceApp::call(Scene::SCENE_TITLE);
}

void MainMenuApp::draw() {
    const JUTColor highlightColor(0xff, 0xff, 0xff, 0xff);
    const JUTColor gray(0x80, 0x80, 0x80, 0x80);
    const char *dash = "-";
    int x = 70;
    J2DPrint *print = System::getJ2DPrint();
    
    System::getJ2DOrtho()->setPort();
    System::getJ2DPrint()->initiate();

    print->setWhiteColor(highlightColor);
    print->initiate();
    print->print(80.0f, 80.0f, "Main Menu(kari)");

    if (mCursorPos == 0) {
        print->print(35.0f,120.0f,dash);
    }

    u8 i;
    for (i = 0; i < 3; i++) {
        if (mDebugMode == i) {
            print->setWhiteColor(highlightColor);
            print->initiate();
        }
        else {
            print->setWhiteColor(gray);
            print->initiate();
        }

        print->print(x, 120.0f, scpaDebugMode[i]);
        x += (int)print->getWidth(scpaDebugMode[i]);
        x += 35;
    }

    x = 70;
    switch(mDebugMode) {
    case 0: {
        print->setWhiteColor(highlightColor);
        print->initiate();
        if (mCursorPos == 1) {
            print->print(35.0f,145.0f,dash);
        }

        for (i = 1; i <= 8; i++) {
            if (mPlayerCount == i) {
                print->setWhiteColor(highlightColor);
                print->initiate();
            }
            else {
                print->setWhiteColor(gray);
                print->initiate();
            }

            print->print(x, 145.0f, "%dP", i);
            x += (int)print->getWidth("%dP", i);
            x += 35;
        }

        x = 70;
        print->setWhiteColor(highlightColor);
        print->initiate();

        if (mCursorPos == 3) {
            print->print(35.0f,170.0f,dash);
        }

        for (i = 0; i < 4; i++) {
            if (mGameMode == i) {
                print->setWhiteColor(highlightColor);
                print->initiate();
            }
            else {
                print->setWhiteColor(gray);
                print->initiate();
            }

            print->print(x,170.0f, scpaGameMode[i]);
            x += (int)print->getWidth(scpaGameMode[i]);
            x += 35;
        }

        if (mGameMode == 3) {
            x = 0x46;
            print->setWhiteColor(highlightColor);
            print->initiate();
            if (mCursorPos == 4) {
                print->print(35.0f,195.0f,dash);
            }

            for (i = 0; i < 4; i++) {
                if (mBattleMode == i) {
                    print->setWhiteColor(highlightColor);
                    print->initiate();
                }
                else {
                    print->setWhiteColor(gray);
                    print->initiate();
                }
                print->print(x, 195.0f,scpaBattleMode[i]);
                x += (int)print->getWidth(scpaBattleMode[i]);
                x += 35;
            }
        }
        break;
    }
    case 1: {
        print->setWhiteColor(highlightColor);
        print->initiate();
        if (mCursorPos == 2) {
            print->print(35.0f,145.0f,dash);
        }

        for (i = 0; i < 5; i++) {
            if (mTestMode == i) {
                print->setWhiteColor(highlightColor);
                print->initiate();
            }
            else {
                print->setWhiteColor(gray);
                print->initiate();
            }
            print->print(x, 145.0f, scpaTestMode[i]);
            x += (int)print->getWidth(scpaTestMode[i]);
            x += 35;
        }
        break;
    }
    case 2: {
        print->setWhiteColor(highlightColor);
        print->initiate();
        if (mCursorPos == 5) {
            print->print(35.0f,145.0f,dash);
        }

        print->print(70.0f,145.0f,"GameFlag");
        print->print(250.0f,145.0f,scpaGameFlag[mGameFlagIdx]);
        print->print(430.0f,145.0f,"%s", gSystemRecord.isGameUnlocked((SystemRecord::GameFlag)mGameFlagIdx) ? "On" : "Off");
        if (mCursorPos == 6) {
            print->print(35.0f,170.0f,dash);
        }

        print->print(70.0f,170.0f,"SecretKart");
        print->print(250.0f,170.0f,scpaSecretKart[mSecretKartId]);
        print->print(430.0f, 170.0f, "%s", gSystemRecord.isKartUnlocked(mSecretKartId) ? "On" : "Off");
        if (mCursorPos == 7) {
            print->print(35.0f, 195.0f, dash);
        }

        print->print(70.0f,195.0f,"GP rank %d", mRankIdx + 1);
        print->print(250.0f, 195.0f, scpaCupName[mCupIdx]);
        print->print(430.0f,195.0f,scpaLevelName[mLevelIdx]);
        if (mCursorPos == 7) {
            print->print(70.0f, 245.0f, "Select GPRecord with C stick.\nSelect rank with stick.\nPress A Button to decide.");
        }
        else {
            print->print(70.0f,245.0f,"Press Y button to reverse all flags.");
        }
        break;
    }
    }

    print->setWhiteColor(highlightColor);
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
            cancel();
        }
        else if (gGamePad1P.testTrigger(KartGamePad::Y)) {
            if (mDebugMode == 2) {
                GetGameAudioMain()->startSystemSe(0x20002);
                switch (mCursorPos) {
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

        if (mCursorPos == 7) {
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
            if (mDebugMode == 1 && mTestMode == 2) {
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
