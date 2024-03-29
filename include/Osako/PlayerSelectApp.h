#ifndef PLAYERSELECTAPP_H
#define PLAYERSELECTAPP_H

#include "Osako/GameApp.h"

class PlayerSelectApp : public GameApp
{ // Autogenerated
public:
    static PlayerSelectApp *create(); // 0x801f90cc
    static void call(u32);            // 0x801f911c
    virtual void reset();             // 0x801f9144, overide
    PlayerSelectApp();                // 0x801f9250
    virtual ~PlayerSelectApp();       // 0x801f92b0, overide
    virtual void draw();              // 0x801f9310, overide
    virtual void calc();              // 0x801f95c4, overide
    void doTag();                     // 0x801f9cc0
    void changePad(u8);               // 0x801f9d48

    // Inline/Unused
    // void decide();
    // void cancel();
    static PlayerSelectApp *ptr() { return mspPlayerSelectApp; }

private:
    static PlayerSelectApp *mspPlayerSelectApp; // 0x804168e8
    static u32 msCallFlag;                      // 0x804168ec
    // TODO
};

#endif // PLAYERSELECTAPP_H
