#ifndef KARTTHUNDER_H
#define KARTTHUNDER_H

#include "types.h"

class KartBody;

class KartThunder
{ // Autogenerated
public:
    KartThunder() {}
    
    void Init(int);      // 0x80307ff4
    bool DoReturn();     // 0x80308058
    bool DoShake();      // 0x803080c4
    void ClearThunder(); // 0x80308144
    void MakeThunder();  // 0x803081d0
    void DoThunderCrl(); // 0x80308364
    // Inline/Unused
    // void SetThunder();

    // Inline
    f32 getScale() { return mScale; }

    KartBody *mBody; // 00
    u8 mFlags;       // 04
    u8 mState;       // 05 
    u16 mTimer;      // 06
    f32 mScale;      // 08
    f32 _c;          //
    f32 _10;         //
}; // 14

#endif // KARTTHUNDER_H
