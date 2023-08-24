#ifndef JAUDIO_JASDRIVER_H
#define JAUDIO_JASDRIVER_H

#include "types.h"

enum JASMixMode
{
    MixMode_Mono,
    MixMode_Mono_Wide,
    MixMode_Extra,
    MixMode_InterLeave
};

enum JASOutputRate
{
    // TODO
};

typedef s16 *(*JASMixCallBack)(s32);

class JASDriver
{
public:
    void startDMA();
    void stopDMA();
    void setOutputRate(JASOutputRate);
    void updateDac();
    void updateDSP();
    void readDspBuffer(s16 *, u32);
    void finishDSPFrame();
    static void registerMixCallback(JASMixCallBack, JASMixMode);
    void getDacRate();
    void getSubFrames();
    void getSubFrameCounter();
    // Inline/Unused
    void setSubFrames(u32);
    void setNumDSPBuffer(u8);
    void registerDacCallback(void (*)(s16 *, u32));
    void registDSPBufCallback(void (*)(s16 *, u32));
};

#endif
