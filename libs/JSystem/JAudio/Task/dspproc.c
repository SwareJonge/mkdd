#include "types.h"
#include "JSystem/JAudio/DSP.h"

static volatile int flag;
static u16 DSP_MIXERLEVEL = 0x4000;

void DSPSendCommands(u32 *p1, u32 p2)
{
    // UNUSED FUNCTION
}

void DSPReleaseHalt3(u32 p1, u16 p2)
{
    // UNUSED FUNCTION
}

void DSPReleaseHalt2(u32 msg)
{
    u32 msgs[2];
    u16 dspMap = DSP_CreateMap2(msg);
    msgs[0] = (msg << 16) | dspMap;

    DSPSendCommands2(msgs, 0, nullptr);
}

void DSPReleaseHalt()
{
    // UNUSED FUNCTION
}

void DSPWaitFinish()
{
    // UNUSED FUNCTION
}

void Dswap(u32 p1, u32 p2, u32 p3)
{
    // UNUSED FUNCTION
}

void Dmix(u32 p1, u32 p2, u32 p3, s16 p4)
{
    // UNUSED FUNCTION
}

void Dcopy(u32 p1, u32 p2, u32 p3)
{
    // UNUSED FUNCTION
}

void DloadBuffer1(u32 p1, u32 p2)
{
    // UNUSED FUNCTION
}

void DloadBuffer(u32 p1, u32 p2, u32 p3)
{
    // UNUSED FUNCTION
}

void DsaveBuffer(u16 p1, u32 p2, u32 p3)
{
    // UNUSED FUNCTION
}

static void setup_callback(u16 p1) { flag = 0; }

void DsetupTable(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5)
{
    u32 commands[5];
    commands[0] = p1 & 0xFFFF | 0x81000000;
    commands[1] = p2;
    commands[2] = p3;
    commands[3] = p4;
    commands[4] = p5;
    flag = 1;
    DSPSendCommands2(commands, 5, setup_callback);
    while (flag != 0)
        ;
}

void DsetMixerLevel(f32 mixerLevel) { DSP_MIXERLEVEL = 4096.0f * mixerLevel; }

void DsyncFrame(u32 param_0, u32 param_1, u32 param_2)
{
    u32 msgs[5];
    msgs[0] = (param_0 & 0xff) << 0x10 | 0x82000000 | DSP_MIXERLEVEL;
    msgs[1] = param_1;
    msgs[2] = param_2;
    DSPSendCommands2(msgs, 3, 0);
}

void wait_callback(u16 p1)
{
    // UNUSED FUNCTION
}

void DwaitFrame()
{
    // UNUSED FUNCTION
}

void DiplSec(u32 p1, void (*p2)(u16))
{
    // UNUSED FUNCTION
}

void DagbSec(u32 p1, void (*p2)(u16))
{
    // UNUSED FUNCTION
}

void dummy_callback(u16 p1)
{
    // UNUSED FUNCTION
}

void DsetDolbyDelay(u32 p1, u16 p2)
{
    // UNUSED FUNCTION
}