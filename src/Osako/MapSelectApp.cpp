#include "Osako/MapSelectApp.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/JUtility/TColor.h"
#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceInfo.h"
#include "Osako/AppMgr.h"
#include "Osako/GameApp.h"
#include "Osako/MainMenuApp.h"
#include "Osako/NetGateApp.h"
#include "Osako/PlayerSelectApp.h"
#include "Osako/RaceApp.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/SystemRecord.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "kartEnums.h"
#include "types.h"

#include "mathHelper.h"

MapSelectApp *MapSelectApp::mspMapSelectApp;
MapSelectApp::Menu *MapSelectApp::Menu::mspLastMenu;
MapSelectApp::Menu *MapSelectApp::Menu::mspSelectedMenu;
u32 MapSelectApp::Menu::msItemNum;
s16 MapSelectApp::Menu::msX;
s16 MapSelectApp::Menu::msY;

static MapSelectApp::Menu menu0(50, 80, "Luigi", LUIGI_CIRCUIT);
static MapSelectApp::Menu menu1("Peach", PEACH_BEACH);
static MapSelectApp::Menu menu2("Baby", BABY_PARK);
static MapSelectApp::Menu menu3("Nokonoko", MUSHROOM_BRIDGE);
static MapSelectApp::Menu menu4("Desert", DRY_DRY_DESERT);
static MapSelectApp::Menu menu5("Daisy", DAISY_CRUISER);
static MapSelectApp::Menu menu6("Snow", SHERBET_LAND);
static MapSelectApp::Menu menu7("Mario", MARIO_CIRCUIT);
static MapSelectApp::Menu menu8("Patapata", MUSHROOM_CITY);
static MapSelectApp::Menu menu9("Waluigi", WALUIGI_STADIUM);
static MapSelectApp::Menu menuA("Donkey", DK_MOUNTAIN);
static MapSelectApp::Menu menuB("Koopa", BOWSER_CASTLE);
static MapSelectApp::Menu menuC("Diddy", DINO_DINO_JUNGLE);
static MapSelectApp::Menu menuD("Yoshi", YOSHI_CIRCUIT);
static MapSelectApp::Menu menuE("Wario", WARIO_COLOSSEUM);
static MapSelectApp::Menu menuF("Rainbow", RAINBOW_ROAD);
static MapSelectApp::Menu menu10(195, 80, "Test 1", TEST_1);
static MapSelectApp::Menu menu11("Test 2", TEST_2);
static MapSelectApp::Menu menu12("Test 3", TEST_3);
static MapSelectApp::Menu menu13("Test 4", TEST_4);
static MapSelectApp::Menu menu14("Test 5", TEST_5);
static MapSelectApp::Menu menu15("Test 6", TEST_6);
static MapSelectApp::Menu menu16("Test 7", TEST_7);
static MapSelectApp::Menu menu17("Test 8", TEST_8);
static MapSelectApp::Menu menu18("Test 9", TEST_9);
static MapSelectApp::Menu menu19("Test 10", TEST_10);
static MapSelectApp::Menu menu1A("Test 11", TEST_11);
static MapSelectApp::Menu menu1B("Test 12", TEST_12);
static MapSelectApp::Menu menu1C("E3 90%%", E3);
static MapSelectApp::Menu menu1D("A3", A3);
static MapSelectApp::Menu menu1E("Dekoboko", DEBOKOKO); // early version of sherbet land, if memory serves me right
static MapSelectApp::Menu menu20(350, 80, "Test 13", TEST_13);
static MapSelectApp::Menu menu21("Test 14", TEST_14);
static MapSelectApp::Menu menu22("Test 15", TEST_15);
static MapSelectApp::Menu menu23("Test 16", TEST_16);
static MapSelectApp::Menu menu24("Test 17", TEST_17);
static MapSelectApp::Menu menu25("Test 18", TEST_18);
static MapSelectApp::Menu menu26("Test 19", TEST_19);
static MapSelectApp::Menu menu27("Test 20", TEST_20);
static MapSelectApp::Menu menu28("Test 21", TEST_21);
static MapSelectApp::Menu menu29("Test 22", TEST_22);
static MapSelectApp::Menu menu2A("Test 23", TEST_23);
static MapSelectApp::Menu menu2B("Test 24", TEST_24);
static MapSelectApp::Menu menu2C("Test 25", TEST_25);
static MapSelectApp::Menu menu2D("Test 26", TEST_26);
static MapSelectApp::Menu menu2E("Test 27", TEST_27);
static MapSelectApp::Menu menu30(510, 80, "Mini 1", MINI_1);
static MapSelectApp::Menu menu31("Mini 2", MINI_2);
static MapSelectApp::Menu menu32("Mini 3", MINI_3);
static MapSelectApp::Menu menu33("Mini 4", MINI_4);
static MapSelectApp::Menu menu34("Mini 5", MINI_5);
static MapSelectApp::Menu menu35("Mini 6", MINI_6);
static MapSelectApp::Menu menu36("Mini 7", MINI_7);
static MapSelectApp::Menu menu37("Mini 8", MINI_8);
static MapSelectApp::Menu menu38("Rev 1", REVERSE_1);
static MapSelectApp::Menu menu39("Rev 2", REVERSE_2);
static MapSelectApp::Menu menu3A("Rev 3", REVERSE_3);
static MapSelectApp::Menu menu3B("Rev 4", REVERSE_4);
static MapSelectApp::Menu menu3C("Rev 5", REVERSE_5);
static MapSelectApp::Menu menu3D("Rev 6", REVERSE_6);
static MapSelectApp::Menu menu3E("Rev 7", REVERSE_7);
static MapSelectApp::Menu menu3F("Rev 8", REVERSE_8);

void MapSelectApp::Menu::init(const char *crsName, ECourseID crsId) {
    mCrsName = crsName;
    mCrsId = crsId;
    if (msItemNum == 0) {
        mMapType = 2;
        mspSelectedMenu = this;
    }
    else if (crsId == CRS_ID_INV) {
        mMapType = 0;
    }
    else {
        mMapType = 1;
    }
    
    mLastMenu = mspLastMenu;
    mspLastMenu = this;
    for (int i = 0; i < 4; i++) {
        mSubMenus[i] = nullptr;
    }
    msItemNum++;
}

MapSelectApp::Menu::Menu(s16 x, s16 y, const char *crsName, ECourseID crsId) {
    msX = mX = x;
    msY = mY = y;
    init(crsName, crsId);
}

MapSelectApp::Menu::Menu(const char *crsName, ECourseID crsId) {
    mX = msX;
    msY += 21;
    mY = msY;
    init(crsName, crsId);
}

void MapSelectApp::Menu::link() {
    int closestUpDist;
    int closestDownDist;
    int closestLeftDist;
    int closestRightDist;
    int dx2, dy2;
    Menu *menu;
    Menu *lastMenu;
    
    int dx, dy;
    int distSq;
    for (menu = mspLastMenu; menu != nullptr; menu = menu->mLastMenu) {        
        Menu *subMenus[4] = {
            nullptr, nullptr, nullptr, nullptr
        };
        f32 directionalBias[4] = {
            0.0f, 0.0f, 0.0f, 0.0f
        };
        
        for (lastMenu = mspLastMenu; lastMenu != nullptr; lastMenu = lastMenu->mLastMenu) {
            if (lastMenu == menu || lastMenu->mMapType == 0) 
                continue;

            dx = lastMenu->mX - menu->mX;
            dy = lastMenu->mY - menu->mY;
            dx2 = dx * dx;
            dy2 = dy * dy;
            distSq = dx2 + dy2;

            if (dy < 0) { // Up
                if (menu->mSubMenus[0] == nullptr || closestUpDist > distSq) {
                    menu->mSubMenus[0] = lastMenu;
                    closestUpDist = distSq;
                }
                if (distSq > 0) {
                    f32 score = -(dy * dy2) / (f32)distSq;
                    if (score > directionalBias[1]) {
                        directionalBias[1] = score;
                        subMenus[1] = lastMenu;
                    }
                }
            }

            if (dy > 0) { // Down
                if (menu->mSubMenus[1] == nullptr || closestDownDist > distSq) {
                    menu->mSubMenus[1] = lastMenu;
                    closestDownDist = distSq;
                }
                if (distSq > 0) {
                    f32 score = (dy * dy2) / (f32)distSq;
                    if (score > directionalBias[0]) {
                        directionalBias[0] = score;
                        subMenus[0] = lastMenu;
                    }
                }
            }

            if (dx < 0) { // Left
                if (menu->mSubMenus[2] == nullptr || closestLeftDist > distSq) {
                    menu->mSubMenus[2] = lastMenu;
                    closestLeftDist = distSq;
                }
                if (distSq > 0) {
                    f32 score = -(dx * dx2) / (f32)distSq;
                    if (score > directionalBias[3]) {
                        directionalBias[3] = score;
                        subMenus[3] = lastMenu;
                    }
                }
            }

            if (dx > 0) { // Right
                if (menu->mSubMenus[3] == nullptr || closestRightDist > distSq) {
                    menu->mSubMenus[3] = lastMenu;
                    closestRightDist = distSq;
                }
                if (distSq > 0) {
                    f32 score = (dx * dx2) / (f32)distSq;
                    if (score > directionalBias[2]) {
                        directionalBias[2] = score;
                        subMenus[2] = lastMenu;
                    }
                }
            }
        }

        for (u8 i = 0; i < 4; i++) {
            if (menu->mSubMenus[i] == nullptr) {
                menu->mSubMenus[i] = subMenus[i];
            }
        }
    }
}

void MapSelectApp::Menu::draw() {
    static const JUTColor gray(0x80, 0x80, 0x80, 0x80);
    static const JUTColor white(0xff, 0xff, 0xff, 0xff);
    
    System::getJ2DOrtho()->setPort();
    J2DPrint *print = System::getJ2DPrint();
    print->initiate();

    if (mMapType == 0) {
        print->setCharColor(gray);
        print->setGradColor(gray);
    }
    else {
        print->setCharColor(white);
        print->setGradColor(white);
    }

    print->print(mX, mY, mCrsName);
    if (mMapType == 2) {
        print->print(mX - 30, mY, "=>");
    }
}

bool MapSelectApp::Menu::move(Direction direction) {
    // fabricated return, but size matches this way
    if (mspSelectedMenu->mSubMenus[direction]) {
        mspSelectedMenu->mMapType = 1;
        mspSelectedMenu = mspSelectedMenu->mSubMenus[direction];
        mspSelectedMenu->mMapType = 2;
        GetGameAudioMain()->startSystemSe(0x20000);
        return true;
    }
    return false;
}

void MapSelectApp::Menu::decide() {
    GetGameAudioMain()->startSystemSe(0x20002);
    if (gRaceInfo.getStatusNumber() > 1) {
        ResMgr::loadCourseData((ECourseID)mspSelectedMenu->mCrsId, ResMgr::mcOrder_1);
    }
    else {
        ResMgr::loadCourseData((ECourseID)mspSelectedMenu->mCrsId, ResMgr::mcOrder_2);
    }
}

void MapSelectApp::Menu::cancel() {
    GetGameAudioMain()->startSystemSe(0x20004);
    PlayerSelectApp::call(0);
}

void MapSelectApp::call() {
    AppMgr::setNextApp(AppMgr::mcMapSelect);
}

MapSelectApp *MapSelectApp::create() {
    if (mspMapSelectApp == nullptr)
        mspMapSelectApp = NEW_APP MapSelectApp();
    return mspMapSelectApp;
}

MapSelectApp::MapSelectApp() : GameApp(0, "Map Sel", nullptr) {
    Menu::link();
    mPad = gpaKartPad[0];
    mState = 1;
}

MapSelectApp::~MapSelectApp() {
    mspMapSelectApp = nullptr;
}

void MapSelectApp::draw() {    
    System::getJ2DOrtho()->setPort();
    System::getJ2DPrint()->initiate();
    System::getJ2DPrint()->print(80.0f, 60.0f, "Map Select(kari)");

    for (Menu *menu = Menu::mspLastMenu; menu != nullptr; menu = menu->mLastMenu) {
        menu->draw();
    }
}

void MapSelectApp::calc() {
    switch (mState) {
            case 1: {
            if (mPad->testRepeat(KartGamePad::MAINSTICK_UP)) {
                Menu::move(Menu::Direction_Up);
            }
            else if (mPad->testRepeat(KartGamePad::MAINSTICK_DOWN)) {
                Menu::move(Menu::Direction_Down);
            }
            else if (mPad->testRepeat(KartGamePad::MAINSTICK_LEFT)) {
                Menu::move(Menu::Direction_Left);
            }
            else if (mPad->testRepeat(KartGamePad::MAINSTICK_RIGHT)) {
                Menu::move(Menu::Direction_Right);
            }

            if (mPad->testTrigger(KartGamePad::A)) {
                Menu::decide();
                mState = 2;
            }
            else if (mPad->testTrigger(KartGamePad::B)) {
                Menu::cancel();
            }
            break;
        }
        case 2: {
            if (ResMgr::isFinishLoadingArc(ResMgr::mcArcCourse)) {
                delete NetGateApp::ptr();
                delete MainMenuApp::ptr();
                delete PlayerSelectApp::ptr();
                AppMgr::deleteCurrentApp();
                if (gRaceInfo.getRaceMode() == TIME_ATTACK && !(gSystemRecord.mOptions & 8)) {
                    gSequenceInfo.setGhostFlag(1);
                    SYSTEM_SetFaderStatus(JUTFader::Status_Out, 0);
                    SequenceApp::call(Scene::SCENE_GHOST_LOAD_SAVE);
                }
                else {
                    RaceApp::call();
                }
            }
        }
    }
}

#include "JSystem/JAudio/JASFakeMatch2.h"
