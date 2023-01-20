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
    enum PadPort {
        PORT_1 = 0,
        PORT_2 = 1,
        PORT_3 = 2,
        PORT_4 = 3,
    };

    enum PadType { };
    enum PadState { };

    KartGamePad(EPadPort ePadPort, PadPort padPort, PadType padType, PadState padState) : JUTGamePad(ePadPort) {
        mPadType = padType;
        mPadPort = padPort;
        mPadState = padState;
    }
    virtual ~KartGamePad() {}

    static void compress(PADStatus const &padStatus, KartPadData *kartPadData);
    static void compress(LGPosition const &lgPosition, KartPadData *kartPadData);
    void expand(KartPadData const &);

    PadType getPadType() { return mPadType; }
    PadPort getPadPort() { return mPadPort; }
    PadState getPadState() { return mPadState;}

private:
    PadType mPadType;
    PadPort mPadPort;
    PadState mPadState;
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

extern KartGamePad * gpaGamePad[4];
extern KartGamePad * gpaKartPad[16];

#endif KARTPAD_H