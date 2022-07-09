#ifndef KARTPAD_H
#define KARTPAD_H

#include "types.h"
#include "JSystem/JUtility/JUTGamePad.h"

class KartGamePad : JUTGamePad {
    public:
    // placeholder Enums
    enum PadPort {
        
    };

    enum PadType {

    };

    enum PadState {

    };
    KartGamePad(JUTGamePad::EPadPort, PadPort, PadType, PadState); // defined here
    virtual ~KartGamePad(); // defined here

    PadType getPadType() {
        return padType;
    }

    PadPort getPadPort() {
        return padPort;
    }
    PadState getPadState() {
        return padState;
    }

    private:
    PadType padType;
    PadPort padPort;
    PadState padState;
        
};

#endif KARTPAD_H