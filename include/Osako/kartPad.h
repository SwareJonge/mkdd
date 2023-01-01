#ifndef KARTPAD_H
#define KARTPAD_H

#include "types.h"
#include "JSystem/JUtility/JUTGamePad.h"

class KartGamePad : public JUTGamePad
{
public:
    // placeholder Enums
    enum PadPort
    {
        PORT_1 = 0,
        PORT_2 = 1,
        PORT_3 = 2,
        PORT_4 = 3,
    };

    enum PadType
    {

    };

    enum PadState
    {

    };
    KartGamePad(JUTGamePad::EPadPort ePadPort, PadPort padPort, PadType padType, PadState padState) : JUTGamePad(ePadPort)
    {
        mPadType = padType;
        mPadPort = padPort;
        mPadState = padState;
    }
    virtual ~KartGamePad() {}

    PadType getPadType()
    {
        return mPadType;
    }

    PadPort getPadPort()
    {
        return mPadPort;
    }
    PadState getPadState()
    {
        return mPadState;
    }

private:
    PadType mPadType;
    PadPort mPadPort;
    PadState mPadState;
};

extern KartGamePad gGamePad1P;
extern KartGamePad gGamePad2P;
extern KartGamePad gGamePad3P;
extern KartGamePad gGamePad4P;
extern KartGamePad gGamePad5P;
extern KartGamePad gGamePad6P;
extern KartGamePad gGamePad7P;
extern KartGamePad gGamePad8P;
extern KartGamePad gGamePad9P;
extern KartGamePad gGamePad10P;
extern KartGamePad gGamePad11P;
extern KartGamePad gGamePad12P;
extern KartGamePad gGamePad13P;
extern KartGamePad gGamePad14P;
extern KartGamePad gGamePad15P;
extern KartGamePad gGamePad16P;

extern KartGamePad * gpaGamePad[16];

#endif KARTPAD_H