#ifndef RESETAGENT_H
#define RESETAGENT_H

#include "JSystem/JUtility/JUTGamePad.h"

namespace ResetAgent
{ // Autogenerated
    // Global
    extern void callbackReset(int, void *); // 0x8020f4dc
    void reset(bool toMenu);                // 0x8020f508
    void draw();                            // 0x8020f530
    void calc();                            // 0x8020f534
    extern int msState;                     // 0x80416a78
    extern int msCount;                     // 0x80416a7c
    extern bool msForceMenu;                // 0x80416a80
    extern bool msInvalid;                  // 0x80416a81
    extern bool msAudioReset;               // 0x80416a82

}; // namespace ResetAgent
#endif // RESETAGENT_H