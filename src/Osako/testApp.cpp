#include "JSystem/JKernel/JKRHeap.h"
#include "Osako/AppMgr.h"
#include "Osako/kartPad.h"
#include "Osako/MainMenuApp.h"
#include "Osako/system.h"
#include "Osako/testApp.h"

TestApp *TestApp::mspTestApp;

TestApp *TestApp::create(void)
{
    if (!mspTestApp)
        mspTestApp = NEW_APP TestApp();

    return mspTestApp;
}

TestApp::TestApp() : GameApp(0, "test", nullptr) {}

TestApp::~TestApp() { mspTestApp = nullptr; }

void TestApp::draw()
{
#ifdef DEBUG
    System::getJ2DOrtho()->setPort();
    J2DPrint *j2dPrint = System::getJ2DPrint();
    j2dPrint->initiate();
    j2dPrint->print(100.0f, 20.0f, "test");
#endif
}

void TestApp::calc()
{
#ifdef DEBUG
    if (gGamePad1P.testTrigger(PAD_BUTTON_B))
    {
        AppMgr::deleteCurrentApp();
        MainMenuApp::call();
    }
#endif
}