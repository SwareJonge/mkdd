#ifndef AWARDAPP_H
#define AWARDAPP_H

#include "Osako/GameApp.h"

class AwardApp : public GameApp
{ // Autogenerated
public:
    AwardApp();                // 0x8020e1d0
    static AwardApp *create(); // 0x8020e15c
    static void call();        // 0x8020e1ac

    // Vtable
    virtual void calc();                           // 0x8020e224, overide
    virtual ~AwardApp() { mspAwardApp = nullptr; } // 0x801fbc60, overide
    virtual void draw() {}                         // 0x8020e29c, overide

    // Inline Functions
    static AwardApp *ptr() { return mspAwardApp; }

private:
    static AwardApp *mspAwardApp; // 0x80416a68
    int mState;
}; // Size: 0x10

#endif // AWARDAPP_H