#ifndef JAUDIO_JAISOUND_H
#define JAUDIO_JAISOUND_H

#include <JSystem/JUtility/JUTDbg.h>

#include "types.h"

class JAISoundID {
    public:
    JAISoundID() {}
    JAISoundID(u32 id) {
        mID = id;
    }

    void setAnonymous() {
        mID = 0xffffffff;
    }

    bool isAnonymous() const {
        return mID != 0xffffffff;
    }

    private:
    u32 mID;
};

class JAISoundHandle;

class JAISound {
public:

private:
JAISoundHandle *mHandle;

};

class JAISoundHandle {
    public:
    JAISoundHandle() {
        sound_ = nullptr;
    }

    JAISound *getSound() {
#line 41
        JUT_ASSERT(sound_ != 0);
        return sound_;
    }
    
    private:
    JAISound *sound_;
};


#endif