#include <dolphin/os.h>

#include "JSystem/JAudio/System/JASAudioThread.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASCmdStack.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASLfo.h"
#include "JSystem/JAudio/System/JASProbe.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASReport.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace JASDriver {

static void mixMonoTrack(s16 *, u32, JASMixCallBack);
static void mixMonoTrackWide(s16 *, u32, JASMixCallBack);
static void mixExtraTrack(s16 *, u32, JASMixCallBack);
static void mixInterleaveTrack(s16 *, u32, JASMixCallBack);

static u32 getDacSize();
static u32 getFrameSamples();

static s16 *sDmaDacBuffer[3];

static const JASMixFunc sMixFuncs[4] = {
    &mixMonoTrack,
    &mixMonoTrackWide,
    &mixExtraTrack,
    &mixInterleaveTrack
};

static s16 **sDspDacBuffer;
static int sDspDacWriteBuffer;
static int sDspDacReadBuffer;
static int sDspStatus;
static JASDACCallBack sDspDacCallback;

static s16 *lastRspMadep;

static JASDACCallBack dacCallbackFunc;
static JASMixCallBack extMixCallback;

static u32 sOutputRate;
static u32 sSubFrameCounter;

static u8 sDspDacBufferCount = 3;
static JASMixMode sMixMode   = MixMode_Extra;
static f32 sDacRate          = 32000.0f * 1.000890625f;
static u32 sSubFrames        = 7;

void initAI(AIDCallback cb) {
    setOutputRate(OutputRate_32kHz);
    u32 size = getDacSize() * 2;
    for (int i = 0; i < 3; i++) {
        sDmaDacBuffer[i] = new (JASDram, 0x20) s16[size / 2];
#line 98
        JUT_ASSERT(sDmaDacBuffer[i]);
        JASCalc::bzero(sDmaDacBuffer[i], size);
        DCStoreRange(sDmaDacBuffer[i], size);
    }

    sDspDacBuffer = new (JASDram, 0) s16*[sDspDacBufferCount];
#line 109
    JUT_ASSERT(sDspDacBuffer);
    for (int i = 0; i < sDspDacBufferCount; i++) {
        sDspDacBuffer[i] = new (JASDram, 0x20) s16[getDacSize()];
#line 115
        JUT_ASSERT(sDspDacBuffer[i]);
        JASCalc::bzero(sDspDacBuffer[i], size);
        DCStoreRange(sDspDacBuffer[i], size);
    }
    sDspDacWriteBuffer = sDspDacBufferCount - 1;
    sDspDacReadBuffer  = 0;
    sDspStatus         = 0;

    JASChannel::initBankDisposeMsgQueue();
    AIInit(NULL);
    AIInitDMA((u32)sDmaDacBuffer[2], size);
    AISetDSPSampleRate(sOutputRate != 0);
    AIRegisterDMACallback(cb);
}

void startDMA() {
    AIStartDMA();
}

void stopDMA() {
    AIStopDMA();
}

void setOutputRate(JASOutputRate outputRate) {
    sOutputRate = outputRate;
    if (outputRate == OutputRate_32kHz) {
        sSubFrames = 7;
        sDacRate = 32000.0f;
    }
    else {
        sSubFrames = 10;
        sDacRate = 48000.0f; // but i want this
    }
    sDacRate *= 1.0008897f; // ?
}

void updateDac() {
    static u32 dacp = 0;
    {
        s16 *rsp     = lastRspMadep;
        lastRspMadep = NULL;
        if (rsp) {
            AIInitDMA((u32)rsp, getDacSize() * 2);
        }
    }

    u32 frameSamples = getFrameSamples();
    readDspBuffer(sDmaDacBuffer[dacp], frameSamples);
    if (sDspStatus == 0) {
        finishDSPFrame();
    }

    if (extMixCallback) {
        sMixFuncs[sMixMode](sDmaDacBuffer[dacp], frameSamples, extMixCallback);
    }

    {
        JASCriticalSection cs;
        DCStoreRange(sDmaDacBuffer[dacp], getDacSize() * 2);
    }

    lastRspMadep = sDmaDacBuffer[dacp];
    dacp++;
    if (dacp >= 3) {
        dacp = 0;
    }

    if (dacCallbackFunc) {
        (*dacCallbackFunc)(lastRspMadep, getFrameSamples());
    }
}

void updateDSP() {
    static u32 history[10] = { 1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    JASProbe::start(3, "SFR-UPDATE");
    JASDsp::invalChannelAll();
    JASPortCmd::execAllCommand();
    DSPSyncCallback();
    static OSTick old_time     = 0;
    OSTick new_time            = OSGetTick();
    OSTick delta               = new_time - old_time;
    old_time                   = new_time;
    u32 subFrame               = getSubFrames();
    u32 count                  = JASAudioThread::getDSPSyncCount();
#line 250
    JUT_ASSERT(subFrame <= 10);
    history[subFrame - count] = delta;
    if (subFrame != count && ((history[0] / (f32)delta) < 1.1f)) {
        JASReport("kill DSP channel");
        JASDSPChannel::killActiveChannel();
    }
    JASChannel::receiveBankDisposeMsg();
    JASDSPChannel::updateAll();
    subframeCallback();
    JASLfo::updateFreeRun(32028.5f / getDacRate());
    JASProbe::stop(3);
    sSubFrameCounter++;
}

void readDspBuffer(s16 *pDmaDac, u32 samples) {
    int nbuf = sDspDacReadBuffer + 1;
    if (nbuf == sDspDacBufferCount) {
        nbuf = 0;
    }
    if (nbuf == sDspDacWriteBuffer && sDspDacBufferCount >= 3) {
        s16 l = sDspDacBuffer[sDspDacReadBuffer][(samples / 2) - 1];
        s16 r = sDspDacBuffer[sDspDacReadBuffer][samples - 1];
        for (int i = 0; i < samples; i++) {
            sDspDacBuffer[sDspDacReadBuffer][i] = l;
        }
        for (u32 i = samples; i < samples * 2; i++) {
            sDspDacBuffer[sDspDacReadBuffer][i] = r;
        }
        //JASReport("readDspBuffer nbuf:%d sWBuf:%d BCount:%d stat:%d", nbuf, sDspDacWriteBuffer, sDspDacBufferCount, sDspStatus);
    } else {
        sDspDacReadBuffer = nbuf;
        DCInvalidateRange(sDspDacBuffer[nbuf], samples * 4);
    }
    JASCalc::imixcopy(sDspDacBuffer[sDspDacReadBuffer] + samples, sDspDacBuffer[sDspDacReadBuffer], pDmaDac, samples); // Think this requires more temps for TP debug
}

void finishDSPFrame() {
    int dacIdx = sDspDacWriteBuffer + 1;
    if (dacIdx == sDspDacBufferCount) {
        dacIdx = 0;
    }
    if (dacIdx == sDspDacReadBuffer) {
        sDspStatus = 0;
        return;
    }
    sDspDacWriteBuffer         = dacIdx;
    JASAudioThread::setDSPSyncCount(getSubFrames());
    JASProbe::start(7, "DSP-MAIN");
    u32 frameSamples = getFrameSamples();
    JASDsp::syncFrame(getSubFrames(), (u32)sDspDacBuffer[sDspDacWriteBuffer], (u32)(sDspDacBuffer[sDspDacWriteBuffer] + frameSamples));
    sDspStatus = 1;
    updateDSP();
    if (sDspDacCallback) {
        (*sDspDacCallback)(sDspDacBuffer[sDspDacWriteBuffer], frameSamples);
    }
}

void setSubFrames(u32 frames) { // UNUSED
    sSubFrames = frames;
}

void setNumDSPBuffer(u8 num) {
    // UNUSED
    JUT_ASSERT(num >= 2);
    JUT_ASSERT(sDspDacBuffer == 0);
    sDspDacBufferCount = num;
}

void registerMixCallback(JASMixCallBack cb, JASMixMode mixMode) {
    extMixCallback = cb;
    sMixMode = mixMode;
}

void registerDacCallback(JASDACCallBack cb) { // UNUSED
    dacCallbackFunc = cb;
}

void registDSPBufCallback(JASDACCallBack cb) { // UNUSED
    sDspDacCallback = cb;
}

f32 getDacRate() { 
    return sDacRate; 
}

u32 getSubFrames() { 
    return sSubFrames; 
}

u32 getDacSize() {
    return sSubFrames * 0x50 * 2;
}

u32 getFrameSamples() {
    return sSubFrames * 0x50;
}

void mixMonoTrack(s16* dest, u32 size, JASMixCallBack callback) {
    JASProbe::start(5, "MONO-MIX");
    s16* src = callback(size);
    if (!src) {
        return;
    }
    JASProbe::stop(5);
    s16* destPtr = dest;
    for (u32 i = size; i != 0; i--) {
        destPtr[0] = JASCalc::clamp<s16, s32>(destPtr[0] + *src);
        destPtr[1] = JASCalc::clamp<s16, s32>(destPtr[1] + *src);
        destPtr += 2;
        src++;
    }    
}

void mixMonoTrackWide(s16* dest, u32 size, JASMixCallBack callback) {
    JASProbe::start(5, "MONO(W)-MIX");
    s16* src = callback(size);
    if (!src) {
        return;
    }
    JASProbe::stop(5);
    s16* destPtr = dest;
    for (u32 i = size; i != 0; i--) {
        s32 val    = destPtr[0] + src[0];
        destPtr[0] = JASCalc::clamp<s16, s32>(val);
        val        = destPtr[1];
        val -= src[0];
        destPtr[1] = JASCalc::clamp<s16, s32>(val);
        destPtr += 2;
        src++;
    }
}

void mixExtraTrack(s16* dest, u32 size, JASMixCallBack callback) {
    JASProbe::start(5, "DSPMIX");
    s16* src1 = callback(size);
    if (!src1) {
        return;
    }
    JASProbe::stop(5);

    JASProbe::start(6, "MIXING");
    s16* destPtr = dest;
    s16* src2    = src1 + getFrameSamples();
    for (u32 i = size; i != 0; i--) {
        destPtr[0] = JASCalc::clamp<s16, s32>(destPtr[0] + src2[0]);
        destPtr[1] = JASCalc::clamp<s16, s32>(destPtr[1] + src1[0]);
        destPtr += 2;
        src2++;
        src1++;
    }
    JASProbe::stop(6);
}

void mixInterleaveTrack(s16* dest, u32 size, JASMixCallBack callback)
{
    s16* src = callback(size);
    if (!src) {
        return;
    }
    s16* destPtr = dest;
    s16* srcPtr  = src;
    for (u32 i = size * 2; i != 0; i--) {
        destPtr[0] = JASCalc::clamp<s16, s32>(destPtr[0] + srcPtr[0]);
        destPtr += 1;
        srcPtr++;
    }
}

u32 getSubFrameCounter() { 
    return sSubFrameCounter; 
}

}

#ifdef MATCHING

template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

#endif
