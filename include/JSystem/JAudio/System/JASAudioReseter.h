#ifndef JAUDIO_JASAUDIORESETER_H
#define JAUDIO_JASAUDIORESETER_H

#include "types.h"

struct JASAudioReseter {
    JASAudioReseter();
    ~JASAudioReseter();
    bool start(u32, bool);
    void resume();
    int checkDone() const;
    int calc();
    static s32 callback(void*);

    u32 _0;               // 0
    f32 mDSPLevel;        // 04
    int mDoneFlag;        // 08
    bool mThreadStopFlag; // 0C
};  // Size: 0x10

#endif
