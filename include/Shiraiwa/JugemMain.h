#ifndef JUGEMMAIN_H
#define JUGEMMAIN_H

#include "Sato/StateObserver.h"
#include "Shiraiwa/JugemRodSignal.h"
// perhaps this could go in it's own file
// Inherited from GeographyObj, StateObserver and TAnmPlayer?
struct TJugem {
    void signalGo() {
        _0x250 = true;
        mObserver.setState(0);
        
    };
    bool isAbleStart();/* {
        return true;
    }*/
    bool isCallThree() {
        return !mJugemRod ? false : mJugemRod->isCallThree();
    }
    u8 _0[0x14c];
    StateObserver mObserver; // Temporary
    u8 _0x154[0x250 - 0x154];
    bool _0x250;
    u8 _0x251[0x26c - 0x251];
    TJugemRodSignal * mJugemRod;
};

#endif
