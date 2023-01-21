#include "Osako/kartPad.h"
#include "Osako/KartPadData.h"

KartGamePad gGamePad1P(JUTGamePad::Port1, KartGamePad::PORT_1, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gGamePad2P(JUTGamePad::Port2, KartGamePad::PORT_2, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gGamePad3P(JUTGamePad::Port3, KartGamePad::PORT_3, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gGamePad4P(JUTGamePad::Port4, KartGamePad::PORT_4, KartGamePad::NORMAL, KartGamePad::STATE_0);

KartGamePad *gpaGamePad[] = {
    &gGamePad1P,
    &gGamePad2P,
    &gGamePad3P,
    &gGamePad4P};

KartGamePad gKartPad1P(JUTGamePad::Port_unknown, KartGamePad::PORT_1, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad2P(JUTGamePad::Port_unknown, KartGamePad::PORT_2, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad3P(JUTGamePad::Port_unknown, KartGamePad::PORT_3, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad4P(JUTGamePad::Port_unknown, KartGamePad::PORT_4, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad5P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad6P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad7P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad8P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad9P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad10P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad11P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad12P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad13P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad14P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad15P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
KartGamePad gKartPad16P(JUTGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);

KartGamePad *gpaKartPad[] = {
    &gKartPad1P,
    &gKartPad2P,
    &gKartPad3P,
    &gKartPad4P,
    &gKartPad5P,
    &gKartPad6P,
    &gKartPad7P,
    &gKartPad8P,
    &gKartPad9P,
    &gKartPad10P,
    &gKartPad11P,
    &gKartPad12P,
    &gKartPad13P,
    &gKartPad14P,
    &gKartPad15P,
    &gKartPad16P
};

void KartGamePad::compress(PADStatus const &padStatus, KartPadData *kartPadData) {
    s8 stickX, stickY;
    u8 buttons = 0;

    if (padStatus.mCurError == -1) {
        stickX = 0;
        stickY = 0;
    }
    else {
        stickX = (padStatus.mStickX / 4.0f);
        stickY = (padStatus.mStickY / 18.0f);

        if (stickX > 15)
            stickX = 15;
        if (stickX < -15)
            stickX = -15;
        if (stickY > 3)
            stickY = 3;
        if (stickY < -3)
            stickY = -3;

        // maybe there's a cleaner way to write this?
        ConvertBtn(buttons, padStatus.mButton, A, KARTBTN_A);
        ConvertBtn(buttons, padStatus.mButton, B, KARTBTN_B);
        ConvertBtn(buttons, padStatus.mButton, X, KARTBTN_X);
        ConvertBtn(buttons, padStatus.mButton, Y, KARTBTN_Y);
        ConvertBtn(buttons, padStatus.mButton, L, KARTBTN_L);
        ConvertBtn(buttons, padStatus.mButton, R, KARTBTN_R);
        ConvertBtn(buttons, padStatus.mButton, Z, KARTBTN_Z);
        ConvertBtn(buttons, padStatus.mButton, START, KARTBTN_START);
    }
    kartPadData->mStickX = stickX;
    kartPadData->mStickY = stickY;
    kartPadData->mButtons = buttons;
}

void KartGamePad::compress(LGPosition const &lgPosition, KartPadData *kartPadData)
{
    s8 stickX, stickY;
    u8 buttons = 0;

    if (lgPosition.err == -1)
    {
        stickX = 0;
        stickY = 0;
    }
    else
    {
        s8 lgStick = lgPosition.stick3;
        int iVar2 = (lgStick * lgStick) / 30;
        if (lgStick > 0)
        {
            if (iVar2 < 127)
                lgStick = iVar2;
            else
                lgStick = 127;
        }
        else
        {
            if (lgStick < 0)
            {
                if (iVar2 < 127)
                    lgStick = -iVar2;
                else
                    lgStick = -127;
            }
        }
        stickX = (lgStick / 3.7f);

        if (stickX > 15)
            stickX = 15;
        if (stickX < -15)
            stickX = -15;

        u16 input = lgPosition.input;
        ConvertBtn(buttons, input, A, KARTBTN_A);
        ConvertBtn(buttons, input, B, KARTBTN_B);
        ConvertBtn(buttons, input, X, KARTBTN_X);
        ConvertBtn(buttons, input, Y, KARTBTN_Y);
        ConvertBtn(buttons, input, L, KARTBTN_L);
        ConvertBtn(buttons, input, R, KARTBTN_R);
        ConvertBtn(buttons, input, Z, KARTBTN_Z);
        ConvertBtn(buttons, input, START, KARTBTN_START);

        if (input & 8) // is this D-Pad perhaps? afaik mkdd never makes use of D-Pad
            stickY = 3;
        else if (input & 4)
            stickY = -3;
        else
            stickY = 0;

        if (input & 1)
            stickX = -15;
        else if (input & 2)
            stickX = 15;

        // analog input detection(i thought the buttons were digital but ok)
        if (lgPosition.analogL > 175)
            buttons |= KARTBTN_L;
        if (lgPosition.analogR > 175)
            buttons |= KARTBTN_R;

        if (lgPosition.analogA > 80)
            buttons |= KARTBTN_A;
        if (lgPosition.analogB > 80)
            buttons |= KARTBTN_B;
    }
    kartPadData->mStickX = stickX;
    kartPadData->mStickY = stickY;
    kartPadData->mButtons = buttons;
}

void KartGamePad::expand(KartPadData const &kartPadData) 
{
    PADStatus padStatus;
    u16 btn = 0;
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_A, A);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_B, B);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_X, X);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_Y, Y);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_L, L);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_R, R);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_Z, Z);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_START, START);
    padStatus.mStickX = kartPadData.mStickX * 4.0f;
    padStatus.mStickY = kartPadData.mStickY * 18.0f;
    padStatus.mAnalogA = 0;
    padStatus.mAnalogB = 0;
    padStatus.mTriggerLeft = 0;
    padStatus.mTriggerRight = 0;
    padStatus.mButton = btn;

    u32 btnret = mControlStick.update(padStatus.mStickX, padStatus.mStickY, sStickMode, WhichStick_ControlStick, mButtons.mInput) << 24;
    mButtons.update(&padStatus, btnret);
}