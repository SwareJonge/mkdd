#ifndef JAUDIO_JASDRIVER_H
#define JAUDIO_JASDRIVER_H

#include <dolphin/ai.h>

enum JASMixMode
{
    MixMode_Mono,
    MixMode_Mono_Wide,
    MixMode_Extra,
    MixMode_InterLeave,
    MixMode_Max, // 4
};

enum JASOutputRate
{
    OutputRate_32kHz,
    OutputRate_48kHz
};

typedef s16 *(*JASMixCallBack)(s32);
typedef void (*JASDACCallBack)(s16 *, u32);
typedef void (*JASMixFunc)(s16 *, u32, JASMixCallBack);

namespace JASDriver
{
    // JASAiCtrl
    void initAI(AIDCallback);
    void setDSPLevel(f32);
    void startDMA();
    void stopDMA();
    void setOutputRate(JASOutputRate);
    void updateDac();
    void updateDSP();
    void readDspBuffer(s16 *, u32);
    void finishDSPFrame();
    void registerMixCallback(JASMixCallBack, JASMixMode);
    f32 getDacRate();
    u32 getSubFrames();
    u32 getSubFrameCounter();
    void waitSubFrame();
    void setSubFrames(u32);
    void setNumDSPBuffer(u8);
    void registerDacCallback(void (*)(s16 *, u32));
    void registDSPBufCallback(void (*)(s16 *, u32));

    // JASDriverIF
    void setDSPLevel(f32);
    void getChannelLevel_dsp();
    f32 getDSPLevel();
    void setOutputMode(u32);
    u32 getOutputMode();
    void waitSubFrame();
    void rejectCallback(s32 (*) (void *), void *);
    bool registerDspSyncCallback(s32 (*) (void *), void *);
    bool registerSubFrameCallback(s32 (*) (void *), void *);
    void subframeCallback();
    void DSPSyncCallback();
    void updateDacCallback();
    void setChannelLevel(f32);
    f32 getChannelLevel();
    void registerUpdateDacCallback(s32 (*) (void *), void *);
};

#endif
