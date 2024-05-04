
#include "Osako/AppMgr.h"
#include "Osako/CardAgent.h"
#include "Osako/ErrorViewApp.h"
#include "Osako/LogoApp.h"
#include "Osako/MainMenuApp.h"
#include "Osako/system.h"
#include "Osako/TestViewApp.h"

// Future goal: PAL equivalents

#ifdef DEBUG

static const char *scpaMenu[] = {
    "Error", "Progressive", "MemoryCard"};

static const char *scpaDVDError[] = {
    "",
    "Cover Open",
    "No Disk",
    "Wrong Disk",
    "Retry Error",
    "Fatal Error",
    "LA_ERROR_2",
    "LA_ERROR_2",
    "LA_ERROR_3",
    "LA_ERROR_1",
    "LA_ERROR_0",
    "LA_DISCONNECTING",
    "LA_DISCONNECTING_ERROR"};

static const char *scpaProgressive[] = {
    "SHOW_PG",
    "SET_PG",
    "SET_IL",
    "LOGO"};

static const char *scpaMemoryCard[] = {
    "MC_NO_CARD_CONTINUE",
    "MC_NO_USE_CONTINUE",
    "MC_WITHOUT_MK_CONTINUE",
    "MC_BROKEN_NO_SAVE_CONTINUE",
    "MC_BROKEN_GO_FORMAT",
    "MC_NO_FILE_NO_SAVE",
    "MC_GAME_SAVING_NO_TOUCH",
    "MC_GST_SAVING_NO_TOUCH",
    "MC_MISS_SAVED",
    "MC_IN_CARD_PUSH_BUTTON",
    "MC_CHANGE_CARD_PUSH_BUTTON",
    "MC_MISS_FORMAT",
    "MC_FORMAT_LOST",
    "MC_FORMAT_NO_TOUCH",
    "MC_NO_SAVE_CONTINUE",
    "MC_MAKE_FILE",
    "MC_MAKE_FILE_NO_TOUCH",
    "MC_MADE_FILE",
    "MC_MISS_MAKE_FILE",
    "MC_NO_SPACE_GO_IPL",
    "MC_TOO_MANY_FILE_GO_IPL",
    "MC_NO_USE",
    "MC_WITHOUT_MK",
    "MC_BROKEN_NO_USE",
    "MC_BROKEN_NEED_FORMAT",
    "MC_GST_LOAD",
    "MC_LOADED",
    "MC_NO_SPACE_NO_SAVE",
    "MC_TOO_MANY_FILE",
    "MC_GST_SAVE",
    "MC_SAVED",
    "MC_GST_OVERWRITE",
    "MC_NO_GST",
    "MC_GST_NO_LOAD",
    "MC_GST_BROKEN_NO_LOAD",
    "MC_BROKEN_NO_LOAD",
    "MC_TOO_MANY_FILE_CONTINUE",
    "MC_NO_SPACE_CONTINUE"};
#endif

TestViewApp *TestViewApp::mspTestViewApp;

TestViewApp *TestViewApp::create()
{
    if (!mspTestViewApp)
        mspTestViewApp = NEW_APP TestViewApp();

    return mspTestViewApp;
}

bool TestViewApp::call()
{
    return AppMgr::setNextApp(AppMgr::mcTestView);
}

TestViewApp::TestViewApp() : GameApp(0, "TestView", 0)
{
    reset();
}

TestViewApp::~TestViewApp()
{
    mspTestViewApp = nullptr;
}

void TestViewApp::reset()
{
    mDvdError = 1;
    mPrintOption = 0;
    mMemoryCardOption = 0;
    mMenuIndex = 0;
    mMenuOption = 0;
    mJ2DInitialized = FALSE;
}

void TestViewApp::draw()
{
#ifdef DEBUG
    J2DPrint *j2dPrint; // questionable
    if (mJ2DInitialized == FALSE)
    {
        System::getJ2DOrtho()->setPort();
        j2dPrint = System::getJ2DPrint();
        j2dPrint->initiate();
    }

    switch (mMenuIndex)
    {
    case 0:
        j2dPrint->print(70.0f, 100.0f, scpaMenu[mMenuOption]);
        break;
    case 1:
        j2dPrint->print(70.0f, 100.0f, scpaMenu[mMenuOption]);
        j2dPrint->print(70.0f, 160.0f, "%d:%s", mDvdError, scpaDVDError[mDvdError]);
        break;
    case 2:
        j2dPrint->print(70.0f, 100.0f, scpaMenu[mMenuOption]);
        j2dPrint->print(70.0f, 160.0f, "%d:%s", mPrintOption, scpaProgressive[mPrintOption]);
        break;
    case 3:
        j2dPrint->print(70.0f, 100.0f, scpaMenu[mMenuOption]);
        j2dPrint->print(70.0f, 160.0f, "%d:%s", mMemoryCardOption, scpaMemoryCard[mMemoryCardOption]);
        break;
    case 4:
        ErrorViewApp::ptr()->getPrintDvdError()->draw();
        break;
    case 5:
        LogoApp::ptr()->getPrintApp()->draw();
        break;
    case 6:
        CardAgent::getPrintMemoryCard()->draw();
        break;
    }
#endif
}

void TestViewApp::calc()
{
#ifdef DEBUG
    switch (mMenuIndex)
    {
    case 0:
    {
        if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_UP))
        {
            if (--mMenuOption < 0)
                mMenuOption = 2;
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_DOWN))
        {
            if (++mMenuOption >= 3)
                mMenuOption = 0;
        }

        if (gGamePad1P.testTrigger(JUTGamePad::A))
        {
            switch (mMenuOption)
            {
            case 0:
                mMenuIndex = 1;
                break;
            case 1:
                mMenuIndex = 2;
                break;
            case 2:
                mMenuIndex = 3;
                break;
            }
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            AppMgr::deleteCurrentApp();
            MainMenuApp::call();
        }

        break;
    }
    case 1:
        if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_UP))
        {
            if (--mDvdError <= 0)
                mDvdError = ErrorViewApp::ERROR_LA_DISCONNECT_LINK;
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_DOWN))
        {
            if (++mDvdError >= ErrorViewApp::ERROR_MAX)
                mDvdError = ErrorViewApp::ERROR_COVER_OPEN;
        }
        if (gGamePad1P.testTrigger(JUTGamePad::A))
        {
            mMenuIndex = 4;
            mJ2DInitialized = TRUE;
            ErrorViewApp::ptr()->getPrintDvdError()->init(mDvdError);
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 0;
        }
        break;
    case 2:
        if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_UP))
        {
            if (--mPrintOption < PRINTAPP_TYPE::mcShow_Dialog)
                mPrintOption = PRINTAPP_TYPE::mcShow_Logo;
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_DOWN))
        {
            if (++mPrintOption > PRINTAPP_TYPE::mcShow_Logo)
                mPrintOption = PRINTAPP_TYPE::mcShow_Dialog;
        }
        if (gGamePad1P.testTrigger(JUTGamePad::A))
        {
            mMenuIndex = 5;
            mJ2DInitialized = TRUE;
            LogoApp::ptr()->getPrintApp()->init((PRINTAPP_TYPE::MessageID)mPrintOption);
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 0;
        }
        break;
    case 3:
        if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_UP))
        {
            if (--mMemoryCardOption < PrintMemoryCard::mcNoCardContinue)
                mMemoryCardOption = PrintMemoryCard::mcMsg39;
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::MAINSTICK_DOWN))
        {
            if (++mMemoryCardOption > PrintMemoryCard::mcMsg40)
                mMemoryCardOption = PrintMemoryCard::mcNoCardContinue;
        }
        if (gGamePad1P.testTrigger(JUTGamePad::A))
        {
            mMenuIndex = 6;
            mJ2DInitialized = TRUE;
            CardAgent::getPrintMemoryCard()->init((PrintMemoryCard::MessageID)mMemoryCardOption);
        }
        else if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 0;
        }
        break;
    case 4:
        ErrorViewApp::ptr()->getPrintDvdError()->calc();
        if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 1;
            mJ2DInitialized = FALSE;
        }
        break;
    case 5:
        LogoApp::ptr()->getPrintApp()->calc(-1);
        if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 2;
            mJ2DInitialized = FALSE;
        }
        break;
    case 6:
        CardAgent::getPrintMemoryCard()->calc();
        if (gGamePad1P.testTrigger(JUTGamePad::B))
        {
            mMenuIndex = 3;
            mJ2DInitialized = FALSE;
        }
        break;
    }
#endif
}