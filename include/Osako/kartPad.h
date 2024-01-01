#ifndef KARTPAD_H
#define KARTPAD_H

#include "types.h"
#include "dolphin/lg.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "Osako/KartPadData.h"

class KartGamePad : public JUTGamePad
{
public:
    // placeholder Enums
    enum PadPort
    {
        PORT_NET = -2,
        PORT_INV = -1, // uninitialized/local pad?
        PORT_1 = 0,
        PORT_2 = 1,
        PORT_3 = 2,
        PORT_4 = 3,
    };

    enum PadType
    {
        NORMAL,
        NETWORK,
        RECORD
    };
    enum PadState
    {
        STATE_0, // available?
        STATE_1  // unavailable?
    };

    KartGamePad(EPadPort ePadPort, PadPort padPort, PadType padType, PadState padState) : JUTGamePad(ePadPort)
    {
        mPadType = padType;
        mPadPort = padPort;
        mPadState = padState;
    }
    virtual ~KartGamePad() {}

    static void compress(const PADStatus &padStatus, KartPadData *kartPadData);
    static void compress(const LGPosition &lgPosition, KartPadData *kartPadData);
    void expand(const KartPadData &kartPadData);

    PadType getPadType() { return mPadType; }
    PadPort getPadPort() { return mPadPort; }
    PadState getPadState() { return mPadState; }

    void setPadType(PadType padType) { mPadType = padType; }
    void setPadPort(PadPort padPort) { mPadPort = padPort; }
    void setPadState(PadState padState) { mPadState = padState; }

private:
    PadType mPadType;   // A8
    PadPort mPadPort;   // AC
    PadState mPadState; // B0
};

extern KartGamePad gGamePad1P; // has -common on and bss bug
extern KartGamePad gGamePad2P;
extern KartGamePad gGamePad3P;
extern KartGamePad gGamePad4P;

extern KartGamePad gKartPad1P;
extern KartGamePad gKartPad2P;
extern KartGamePad gKartPad3P;
extern KartGamePad gKartPad4P;
extern KartGamePad gKartPad5P;
extern KartGamePad gKartPad6P;
extern KartGamePad gKartPad7P;
extern KartGamePad gKartPad8P;
extern KartGamePad gKartPad9P;
extern KartGamePad gKartPad10P;
extern KartGamePad gKartPad11P;
extern KartGamePad gKartPad12P;
extern KartGamePad gKartPad13P;
extern KartGamePad gKartPad14P;
extern KartGamePad gKartPad15P;
extern KartGamePad gKartPad16P;

extern KartGamePad *gpaGamePad[];
extern KartGamePad *gpaKartPad[];

#endif KARTPAD_H