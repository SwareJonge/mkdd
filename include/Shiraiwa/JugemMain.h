#ifndef JUGEMMAIN_H
#define JUGEMMAIN_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"
#include "Shiraiwa/JugemRodSignal.h"
// perhaps this could go in it's own file
// Inherited from GeographyObj, StateObserver and TAnmPlayer?
// Todo: GeographyObj -> TMapObjHioNode
class TJugem : GeographyObj, StateObserver
{
public:
    TJugem();

    void signalGo()
    {
        _0x250 = true;
        setState(0);
    };
    bool isAbleStart(); /* {
         return true;
     }*/
    bool isCallThree()
    {
        return !mJugemRod ? false : mJugemRod->isCallThree();
    }

    int getSignalState();

    u8 _0x158[0x250 - 0x158];
    bool _0x250;
    u8 _0x251[0x26c - 0x251];
    TJugemRodSignal *mJugemRod;
};

#endif
