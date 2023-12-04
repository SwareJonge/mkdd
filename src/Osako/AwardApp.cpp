#include <JSystem/JAudio/JASFakeMatch2.h>

#include "Osako/AppMgr.h"
#include "Osako/AwardApp.h"
#include "Osako/RaceApp.h"
#include "Osako/ResMgr.h"
#include "Osako/system.h"

AwardApp *AwardApp::mspAwardApp;

AwardApp *AwardApp::create() {
    if(mspAwardApp == nullptr)
        mspAwardApp = NEW_APP AwardApp();
    return mspAwardApp;
}

void AwardApp::call() {
    AppMgr::setNextApp(AppMgr::mcAward);
}

AwardApp::AwardApp() : GameApp(0x7d000, "Award", nullptr)
{
    mState = 0;
}

void AwardApp::calc() {
    switch(mState) {
        case 0:
            ResMgr::loadAwardData(gRaceInfo.getGpCup(), mHeap);
            mState = 1;
            break;
        case 1:
            if(ResMgr::isFinishLoadingArc(ResMgr::mcArcAward)) {
                RaceApp::call();
            }
            break;
    }
}
