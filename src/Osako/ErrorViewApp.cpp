#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/Movie/MoviePlayer.h"
#include "Osako/AppMgr.h"
#include "Osako/ErrorViewApp.h"
#include "Osako/NetGameMgr.h"
#include "Osako/ResetAgent.h"
#include "Osako/system.h"

#include <JSystem/JAudio/JASFakeMatch2.h>

const u8 caErrorPriority[] = {0, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3};

ErrorViewApp *ErrorViewApp::mspErrorViewApp;
ErrorViewApp::ErrorId ErrorViewApp::msErrorId;

ErrorViewApp *ErrorViewApp::create()
{
    if (mspErrorViewApp == nullptr)
    {
        mspErrorViewApp = NEW_APP ErrorViewApp();
    }
    return mspErrorViewApp;
}

void ErrorViewApp::call(ErrorId id)
{
    if (caErrorPriority[msErrorId] > caErrorPriority[id])
        return;

    msErrorId = id;

    switch (id)
    {
    case ERROR_FATAL:
        ResetAgent::msInvalid = 1;
        if (NetGameMgr::ptr()->getState() == 0xd)
        {
            msErrorId = ERROR_LA_TRANSFER;
            NetGameMgr::ptr()->end(false);
        }
    case ERROR_LA_TRANSFER:
    case ERROR_LA_TRANSFER2:
    case ERROR_LA_ROUTER:
    case ERROR_LA_CONNECTED:
    case ERROR_LA_CONNECTION:
        if (GetGameAudioMain()->isActive())
            GetGameAudioMain()->fadeOutAll(30);
        break;
    case ERROR_COVER_OPEN:
    case ERROR_NO_DISK:
    case ERROR_WRONG_DISK:
    case ERROR_RETRY:
        if (NetGameMgr::ptr()->getState() == 0xd)
        {
            msErrorId = ERROR_LA_TRANSFER;
            NetGameMgr::ptr()->end(false);
            if (GetGameAudioMain()->isActive())
                GetGameAudioMain()->fadeOutAll(30);
        }
        break;
    }
    System::getDisplay()->set_38(1);
    AppMgr::insertErrorViewApp();
}

ErrorViewApp::ErrorViewApp() : GameApp(0x10000, "Err View", nullptr)
{
    mErrorState = 0;
    mDvdError = new PrintDvdError(getHeap());
}

ErrorViewApp::~ErrorViewApp() { mspErrorViewApp = nullptr; }

void ErrorViewApp::draw() { mDvdError->draw(); }

void ErrorViewApp::calc()
{
    // another function with 3 nested switches, great
    switch (mErrorState)
    {
    case 0:
        mFadeStatus = SYSTEM_GetFaderStatus();
        SYSTEM_GetFader()->setStatus(JUTFader::Status_In, 0);
        mErrorState = 1;
        _14 = 0;
        System::haltRumble();
    case 1:
        switch (msErrorId)
        {
        case ERROR_COVER_OPEN:
        case ERROR_NO_DISK:
        case ERROR_WRONG_DISK:
        case ERROR_RETRY:
        case ERROR_FATAL:
            if (System::msDvdState == 1)
            {
                AppMgr::setNextApp(AppMgr::getPrevApp()); // Inline?
                switch (mFadeStatus)
                {
                case JUTFader::Status_Out:
                case JUTFader::Status_FadingOut:
                    SYSTEM_GetFader()->setStatus(JUTFader::Status_Out, 0);
                    break;
                case JUTFader::Status_In:
                case JUTFader::Status_FadingIn:
                    SYSTEM_GetFader()->setStatus(JUTFader::Status_In, 0);
                    break;
                }
                msErrorId = ERROR_NONE;
                mErrorState = 0;
                break;
            }
            mDvdError->init(msErrorId);
            mDvdError->calc();
            break;
        case ERROR_LA_TRANSFER:
        case ERROR_LA_TRANSFER2:
        case ERROR_LA_ROUTER:
        case ERROR_LA_CONNECTED:
        case ERROR_LA_CONNECTION:
            if (NetGameMgr::ptr()->getState() == 0x14)
                mDvdError->init(0xc); // ERROR_LA_DISCONNECT_LINK?
            else
            {
                mDvdError->init(msErrorId);
                mErrorState = 2;
            }
            mDvdError->calc();
        }
        break;
    case 2:
        if (gGamePad1P.testTrigger(PAD_BUTTON_A))
        {
            AppMgr::restartCurrentApp();
            msErrorId = ERROR_NONE;
            mErrorState = 0;
        }
        mDvdError->calc();
        break;
    }
}

void ErrorViewApp::reset()
{
    if (MoviePlayer::getPlayer())
        MoviePlayer::getPlayer()->pause();
}