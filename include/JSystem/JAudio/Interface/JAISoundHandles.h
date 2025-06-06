#ifndef JAUDIO_JAISOUNDHANDLES_H
#define JAUDIO_JAISOUNDHANDLES_H

#include "JSystem/JAudio/Interface/JAISound.h"

class JAISoundHandles
{
public:
    JAISoundHandles(JAISoundHandle *handles, int numHandles)
    {
        handles_ = handles;
        numHandles_ = numHandles;
    }

    // Global
    JAISoundHandle *getHandleSoundID(JAISoundID soundID); // 0x8008cff0
    JAISoundHandle *getHandleUserData(u32 id);            // 0x8008d0b4
    JAISoundHandle *getFreeHandle();                      // 0x8008d174
    void stopAllSounds();                                 // 0x8008d1bc
    void setPos(const JGeometry::TVec3f &pos);            // 0x8008d274
    // Inline/Unused
    // void getFreeHandleIndex() const;
    // void releaseAllSounds();
    bool isActive() const;

    int getNumHandles() const { return numHandles_; }

    JAISoundHandle &operator[](int index)
    {
#line 33
        JUT_ASSERT(index >= 0)
        JUT_ASSERT(index < numHandles_)
        return handles_[index];
    }

    JAISoundHandle &getHandle(int index)
    {
#line 42
        JUT_ASSERT(index >= 0)
        JUT_ASSERT(index < numHandles_)
        return handles_[index];
    }

    JAISoundHandle *handles_; // 00
    int numHandles_;          // 04
};

#endif // JAISOUNDHANDLES_H
