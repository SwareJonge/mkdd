#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include <dolphin/gx/GXFrameBuffer.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/Movie/MoviePlayer.h"
#include "Osako/AppMgr.h"
#include "Osako/CardAgent.h"
#include "Osako/ErrorViewApp.h"
#include "Osako/NetGameMgr.h"
#include "Osako/ResetAgent.h"
#include "Osako/system.h"
#include "Osako/SystemRecord.h"
#include "Osako/SystemFile.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

namespace ResetAgent {
    int msState;
    int msCount;
    bool msForceMenu;
    bool msInvalid;
    bool msAudioReset;

    void callbackReset(int p1, void *p2) {
        if(p1 < 1)
            reset(false);
    }

    void reset(bool toMenu) {
        if (msState != 0)
            return;
        if(msInvalid)
            return;

        msState = 1;
        msForceMenu = toMenu;
    }

    void draw() {}

    void calc() {
        switch (msState)
        {
            case 1: {
                msCount = 0;
                msState = 2;
                if(CardAgent::msState != 0 && (CardAgent::msState != 0 && (CardAgent::msState != 0xf))) {
                    CardAgent::msFlags |= 4;
                }

                CardAgent::getPrintMemoryCard()->closeWindowNoSe();
                
                if (GetGameAudioMain()->isActive())
                {
                    GetGameAudioMain()->resetAudio(5);
                    msAudioReset = true;
                }
                else {
                    msAudioReset = false;
                }

                NetGameMgr::ptr()->end(true);

                SYSTEM_StartFadeOut(30);
                SYSTEM_GetFader()->setResetState(true);
                SYSTEM_GetFader()->setColor(TCOLOR_BLACK);
                break;
            }
            
            case 2: {
                msCount++;
                if (msCount >= 30 && (CardAgent::getState() == 0xf || (CardAgent::getState() == 0)) && NetGameMgr::ptr()->getState() != 0x14)
                {
                    if(DVDCheckDisk() == 0 || msForceMenu) {
                        if(msCount >= 80) {
                            System::haltRumble();
                            GXAbortFrame();
                            GXDrawDone();
                            VISetBlack(TRUE);
                            VIFlush();
                            VIWaitForRetrace();
                            OSResetSystem(TRUE, 0, msForceMenu);
                        }
                    }
                    else {
                        SYSTEM_GetFader()->setResetState(false);
                        JUTGamePad::recalibrate(0xf0000000);
#ifdef DEBUG
                        System::haltRumble();
#endif
                        AppMgr::msRequest |= 2; // probably an inline

                        if(msAudioReset) {
                            GetGameAudioMain()->resumeAudio();
                        }
                        gSystemRecord.applyAudioSetting();
#ifdef DEBUG
                        gSystemFile._6024 = 0;
#endif

                        if (MoviePlayer::getPlayer()) {
                            MoviePlayer::getPlayer()->quit();
                        }
                        CardAgent::reset();
                        System::reset();
                        ErrorViewApp::msErrorId = ErrorViewApp::ERROR0; // Inline?
                        ErrorViewApp::mspErrorViewApp->mErrorState = 0; // Inline?
                        msState = 0;
                        msInvalid = true;
                    }
                }
            }
            break;
        }
    }

}