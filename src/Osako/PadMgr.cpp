#include <string.h>

#include "Osako/PadMgr.h"
#include "Osako/NetGameMgr.h"

#include <ppcdis.h>
#include <JSystem/JAudio/JASFakeMatch2.h>

#ifdef DEBUG
static const float float_slack_PadMgr[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(float_slack_PadMgr)
#pragma pop
#endif

namespace PadMgr
{
    KartPadInput msKartPadInput;
    PadRecorder *mspRecorder;

    void init()
    {
        for (int i = 0; i < 4; i++)
            gpaGamePad[i]->setButtonRepeat(0xf000000, 0x14, 8);

        for (int i = 0; i < 16; i++)
            gpaKartPad[i]->setButtonRepeat(0xf000000, 0x14, 8);
    }

    void framework()
    {
        JUTGamePad::read();
        gGamePad1P.checkReset();

        processKartPad();
        if (msKartPadInput == Input_Net)
        {
            NetGameMgr::ptr()->progressReadData();
            NetGameMgr::ptr()->fetchPadData();
        }
    }

    void processKartPad()
    {
        KartPadData padData;
        if (mspRecorder)
        {

            for (u8 i = 0; i < 16; i++)
            {
                getPadData(i, msKartPadInput, &padData);
                mspRecorder->line(i, &padData);
                gpaKartPad[i]->expand(padData);
            }
            mspRecorder->framework();
        }
        else
        {
            for (u8 i = 0; i < 16; i++)
            {
                getPadData(i, msKartPadInput, &padData);
                gpaKartPad[i]->expand(padData);
            }
        }
    }

    void getPadData(u8 port, KartPadInput padInput, KartPadData *padData)
    {
        if (padInput == Input_Local)
        {
            if (port < 4)
            {
                s32 siType = JUTGamePad::getSIType(port);
                switch (siType)
                {
                case SI_TYPE_GC:
                {
                    KartGamePad::compress(JUTGamePad::getLGPosition(port), padData);
                    return;
                }
                case SI_GC_CONTROLLER:
                case SI_GC_WAVEBIRD:
                    KartGamePad::compress(JUTGamePad::getPadStatus(port), padData);
                    return;
                }
            }
            memset(padData, 0, sizeof(KartPadData));
        }
        else
        {
            NetGameMgr::ptr()->getKartPad(port, padData);
        }
    }

    void setKartPadInput(KartPadInput padInput)
    {
        if (msKartPadInput == padInput)
            return;

        msKartPadInput = padInput;
        resetPort();
    }

    void resetPort()
    {
        switch (msKartPadInput)
        {
        case Input_Local:
        {
            for (int i = 0; i < 16; i++)
            {
                gpaKartPad[i]->setPadType(KartGamePad::NORMAL);
                if (i < 4)
                {
                    gpaKartPad[i]->setPadPort((KartGamePad::PadPort)i);
                }
                else
                {
                    gpaKartPad[i]->setPadPort(KartGamePad::PORT_INV);
                }
            }
            break;
        }
        case Input_Net:
        {
            for (int i = 0; i < 16; i++)
            {
                gpaKartPad[i]->setPadType(KartGamePad::NETWORK);

                u8 port = NetGameMgr::ptr()->getPadConvPort(i);
                u8 num = NetGameMgr::ptr()->getPadConvNum(i);

                if (num == NetGameMgr::ptr()->getNetworkNum())
                {
                    gpaKartPad[i]->setPadPort((KartGamePad::PadPort)port);
                }
                else
                {
                    gpaKartPad[i]->setPadPort(KartGamePad::PORT_NET);
                }
            }
            break;
        }
        }
    }
}