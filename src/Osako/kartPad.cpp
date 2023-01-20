#include "Osako/kartPad.h"
#include "Osako/KartPadData.h"

// TODO: add KartGamePad instances

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
        ;
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

        if (input & 8)
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

// Not tested
void KartGamePad::expand(KartPadData const &kartPadData) {
    u16 btn;
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_A, A);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_B, B);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_X, X);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_Y, Y);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_L, L);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_R, R);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_Z, Z);
    ConvertBtn(btn, kartPadData.mButtons, KARTBTN_START, START);
    PADStatus padStatus;
    padStatus.mStickX = (s8)(kartPadData.mStickX * 4.0f);
    padStatus.mStickY = (s8)(kartPadData.mStickY * 18.0f);
    padStatus.mButton = btn;
    padStatus.mAnalogA = 0;
    padStatus.mTriggerLeft = 0;
    padStatus.mTriggerRight = 0;

    u32 btnret = mCStick.update(padStatus.mStickX, padStatus.mStickY, sStickMode, WhichStick_ControlStick, mButtons.mInput);
    mButtons.update(&padStatus, btnret);
}