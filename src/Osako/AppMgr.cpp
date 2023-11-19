#include <JSystem/JAudio/JASFakeMatch2.h>
#include "Osako/AppMgr.h"
#include "Osako/AwardApp.h"
#include "Osako/CardAgent.h"
#include "Osako/ErrorViewApp.h"
#include "Osako/LogoApp.h"
#include "Osako/MapSelectApp.h"
#include "Osako/MainMenuApp.h"
#include "Osako/NetGateApp.h"
#include "Osako/PlayerSelectApp.h"
#include "Osako/RaceApp.h"
#include "Osako/ResetAgent.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/system.h"
#include "Osako/testApp.h"
#include "Osako/TestViewApp.h"
#include "Kaneshige/Movie/MovieApp.h"
#include "Kameda/SceneApp.h"

AppMgr::KartAppEnum AppMgr::msGameApp;
AppMgr::KartAppEnum AppMgr::msNextGameApp;
AppMgr::KartAppEnum AppMgr::msNextNextGameApp;
AppMgr::KartAppEnum AppMgr::msPrevGameApp;
GameApp *AppMgr::mspGameApp;
int AppMgr::msRequest;
u8 AppMgr::msDrawStallFrame;
u8 AppMgr::msCalcStallFrame;
u8 AppMgr::msChangeStallFrame;

void AppMgr::draw()
{
    if (msDrawStallFrame != 0)
    {
        msDrawStallFrame--;
    }
    else
    {
        if (mspGameApp)
        {
            mspGameApp->draw();
        }
    }

    CardAgent::draw();
    ResetAgent::draw();
}

void AppMgr::calc()
{
    if (msRequest & 2)
    {
        if(isRestartable()) {
            restartApp();
            msRequest &= ~(0x2);
            msRequest &= ~(0x4);
            msRequest &= ~(0x8);
        }
    }
    if(msRequest & 4) {
        if(msRequest & 8) {
            if(mspGameApp == SequenceApp::ptr()) {
                SequenceApp::ptr()->freeForMovieApp();
                MovieApp::create();
                LogoApp::ptr()->setLoading(true);
                SYSTEM_RequestTask(LogoApp::createMoviePlayer, MovieApp::getMovieApp()->getHeap(), nullptr);
                MovieApp::getMovieApp()->loadOpeningData();
                SequenceApp::ptr()->ready(Scene::SCENE_TITLE);
                SequenceApp::ptr()->ready(Scene::SCENE_MENU);
                msRequest &= ~(0x8);
            }
        }

        if(ResMgr::isFinishLoadingArc(ResMgr::mcArcOpening)) {
            setNextApp(mcMovie);
            msRequest &= ~(0x4);
        }
    }

    if(msNextGameApp) {
        if (msChangeStallFrame)
        {
            if(ResMgr::isFinishLoadingAram())
                msChangeStallFrame--;
            msCalcStallFrame = 1;
        }
        else {
            if(msRequest & 1) {
                delete mspGameApp;
                mspGameApp = 0;
                msRequest &= ~(0x1);
            }
            mspGameApp = createApp(msNextGameApp);
            msPrevGameApp = msGameApp;
            msGameApp = msNextGameApp;
            msNextGameApp = msNextNextGameApp;
            msNextNextGameApp = mcKartApp0;

            if(mspGameApp) {
                mspGameApp->getHeap()->becomeCurrentHeap();
            }
            if(mspGameApp) {
                mspGameApp->reset();
            }
        }
    }

    if(msCalcStallFrame) {
        msCalcStallFrame--;
        msDrawStallFrame = 1;
    }
    else {
        if(mspGameApp) {
            mspGameApp->calc();
        }
    }

    CardAgent::calc();
    ResetAgent::calc();
}

bool AppMgr::setNextApp(KartAppEnum nextApp)
{
    if (mspGameApp == nullptr || (msGameApp != nextApp && msNextGameApp != nextApp))
    {
        msNextGameApp = nextApp;
        msChangeStallFrame = 1;
        msDrawStallFrame = 1;
        return true;
    }

    return false;
}

void AppMgr::insertErrorViewApp()
{
    if (msNextGameApp == mcErrorView)
        return;
    if (msGameApp == mcErrorView)
        return;

    msNextNextGameApp = msNextGameApp;
    msNextGameApp = mcErrorView;
    msChangeStallFrame = 1;
    msDrawStallFrame = 1;
}

GameApp *AppMgr::createApp(KartAppEnum app)
{
    switch (app)
    {
    case mcErrorView:
        return ErrorViewApp::create();
    case mcLogo:
        return LogoApp::create();
    case mcMainMenu:
        return MainMenuApp::create();
    case mcPlayerSelect:
        return PlayerSelectApp::create();
    case mcMapSelect:
        return MapSelectApp::create();
    case mcRace:
        return RaceApp::create();
    case mcAward:
        return AwardApp::create();
    case mcSequence:
        return SequenceApp::create();
    case mcNetGate:
        return NetGateApp::create();
    case mcMovie:
        return MovieApp::create();
    case mcScene:
        return SceneApp::create();
    case mcTest:
        return TestApp::create();
    case mcTestView:
        return TestViewApp::create();
    }
    return nullptr;
}

void AppMgr::restartApp()
{
    KartAppEnum activeApp = msGameApp;
    if (msGameApp == mcErrorView)
        activeApp = msPrevGameApp;

    if (FLAG_ON(msRequest, 1) || (mspGameApp != RaceApp::ptr()))
        delete RaceApp::ptr();

    if (FLAG_ON(msRequest, 1) || (mspGameApp != AwardApp::ptr()))
        delete AwardApp::ptr();

    if (FLAG_ON(msRequest, 1) || (mspGameApp != NetGateApp::ptr()))
        delete NetGateApp::ptr();

    delete MainMenuApp::ptr();
    delete PlayerSelectApp::ptr();
    delete MapSelectApp::ptr();
    delete SceneApp::ptr();
    delete TestApp::ptr();
    delete TestViewApp::ptr();

    switch (activeApp)
    {
    case mcLogo:
        LogoApp::ptr()->restart();
        break;
    case mcMovie:
        delete MovieApp::getMovieApp();
    default:
        SequenceApp::call(Scene::SCENE_TITLE);
    }

    ResetAgent::msInvalid = false;
}

bool AppMgr::isRestartable() {
    if(LogoApp::ptr()->isLoading())
        return false;
    
    if(ResMgr::isLoadingArc(ResMgr::mcArcOpening)) {
        return false;
    }

    if (NetGateApp::ptr() ? NetGateApp::ptr()->hasActiveTask() : false)
        return false;

    return true;
}
