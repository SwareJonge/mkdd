#ifndef KARTPAD_H
#define KARTPAD_H

#include "types.h"
#include "JSystem/JUtility/JUTGamePad.h"

class KartGamePad : JUTGamePad
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

#endif KARTPAD_H