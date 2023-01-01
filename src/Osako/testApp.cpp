#include "JSystem/JKernel/JKRHeap.h"
#include "Osako/AppMgr.h"
#include "Osako/kartPad.h"
#include "Osako/MainMenuApp.h"
#include "Osako/system.h"
#include "Osako/testApp.h"

TestApp * TestApp::mspTestApp;

TestApp * TestApp::create(void)
{
    if (!mspTestApp)
        mspTestApp = new(System::getAppHeap(), 0) TestApp();

    return mspTestApp;
}

TestApp::TestApp() : GameApp(null, "test", nullptr) {

}

TestApp::~TestApp() {
    mspTestApp = 0;
}

void TestApp::draw() {
    System::getJ2DOrtho()->setPort();
    J2DPrint *j2dPrint = System::getJ2DPrint();
    j2dPrint->initiate();
    j2dPrint->print(100.0f, 20.0f, "test");
}

void TestApp::calc() {
    if (gGamePad1P[0].testTrigger(0x200)) {
        AppMgr::msRequest |= 1;
        MainMenuApp::call();
    }
}