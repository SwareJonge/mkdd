#ifndef JASMUTEX_H
#define JASMUTEX_H

#include <dolphin/os.h>

class JASMutexLock
{
public:
    JASMutexLock(OSMutex *mutex)
    {
        mMutex = mutex;
        OSLockMutex(mutex);
    }

    ~JASMutexLock()
    {
        OSUnlockMutex(mMutex);
    }

private:
    OSMutex *mMutex;
};

#endif
