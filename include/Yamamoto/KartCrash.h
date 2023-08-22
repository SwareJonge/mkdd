#ifndef KARTCRASH_H
#define KARTCRASH_H

#include "Sato/ItemObj.h"

class KartCrash
{ // Autogenerated
public:
    KartCrash() {}

    void Init(int);                // 0x80308560
    void DoRabbitMark();           // 0x803085b0
    void DoMatchlessTimer();       // 0x80308610
    void CheckMatchlessTimer();    // 0x803087d8
    void SetMatchlessTimer();      // 0x803087ec
    void ClearMatchlessTimer();    // 0x80308840
    void DoDecBalloon();           // 0x80308884
    void DoPointBomb(ItemObj *);   // 0x803088fc
    void DoDecPointBomb();         // 0x80308984
    void SaveDir();                // 0x80308a08
    void NonRescue();              // 0x80308b14
    void DoRollCrashCrl();         // 0x80308b98
    void DoPitchCrashCrl();        // 0x80308cdc
    void MakeBombCrash(ItemObj *); // 0x80308dd0
    void DoBombCrashCrl();         // 0x80308f64
    void MakeKameCrash(ItemObj *); // 0x80309018
    void MakeTornado();            // 0x80309154
    void DoTornadeCenter();        // 0x80309330
    void DotornadeCrashCrl();      // 0x803093e0
    void MakeSpin(ItemObj *);      // 0x803096d8
    void DoSpinCrashCrl();         // 0x80309878
    void MakeHalfSpin(ItemObj *);  // 0x803099ec
    void DoHalfSpinCrashCrl();     // 0x80309b80
    void MakeThunderSpin();        // 0x80309cfc
    void MakeBurn(ItemObj *);      // 0x80309d6c
    void MakeFreezeCrash();        // 0x80309e9c
    void DoFreezeCrashCrl();       // 0x80309fec
    // Inline/Unused
    // void MakeRollCrash();
private:
    // TODO
};
#endif // KARTCRASH_H