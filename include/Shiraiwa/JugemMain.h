#ifndef JUGEMMAIN_H
#define JUGEMMAIN_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"
#include "Shiraiwa/JugemRodItem.h"
#include "Shiraiwa/AnmPlayer.h"

class TJugem : public TMapObjHioNode, public StateObserver
{
public:
    TJugem();

    void signalGo()
    {
        mIsAbleStart = true;
        setState(0);
    };
    bool isAbleStart(); /* { return mIsAbleStart; }*/
    bool isCallThree() { return !mJugemRod ? false : mJugemRod->isCallThree(); }

    const JGeometry::TVec3f &getPosition() const { return mPos; }

    int getSignalState();

    u8 _0x158[0x250 - 0x158];   //
    bool mIsAbleStart;          // 250
    u8 _0x251[0x26c - 0x251];   //
    TJugemRodSignal *mJugemRod; // 270
    u8 _274[0x27c - 0x274];     //
}; // Size: 0x27c

#endif
