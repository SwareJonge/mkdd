#include "Osako/AppMgr.h"
#include "Osako/NetGameMgr.h"
#include "Osako/RaceApp.h"
#include "Osako/ResMgr.h"
#include "Osako/system.h"

// WIP

RaceApp *RaceApp::mspRaceApp;
PadRecorder *RaceApp::mspRecorder;

RaceApp *RaceApp::create()
{
    if (mspRaceApp == nullptr)
        mspRaceApp = NEW_APP RaceApp();
    return mspRaceApp;
}

void RaceApp::call() { AppMgr::setNextApp(AppMgr::mcRace); }

RaceApp::RaceApp() : GameApp(0x600000, "Race", nullptr)
{
    switch (NetGameMgr::ptr()->getState())
    {
    case 6:
    }

    mRaceMgr = new RaceMgr(&gRaceInfo);
}

RaceApp::RaceApp() {
    delete mspForceEffectMgr;
    delete mPadRecorder;
    mspRecorder = nullptr;
    mspRaceApp = nullptr;

    if(mRaceMgr)
        delete mRaceMgr;
}

void RaceApp::draw() {
    if (mDrawFlags == 1 && SYSTEM_GetDisplay->_40)
    {
    }
}