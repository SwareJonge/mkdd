#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "dolphin/os/OSThread.h"

namespace JASDriver {

static JASCallbackMgr sDspSyncCallback;
static JASCallbackMgr sSubFrameCallback;
static JASCallbackMgr sUpdateDacCallback;

static u16 MAX_MIXERLEVEL = 0x2ee0;
static u32 JAS_SYSTEM_OUTPUT_MODE = 1;

void setDSPLevel(f32 level) {
    JASDsp::setDSPMixerLevel(level);
}

u16 getChannelLevel_dsp() {
    return MAX_MIXERLEVEL;
}

f32 getDSPLevel() {
    return JASDsp::getDSPMixerLevel();
}

void setOutputMode(u32 outputMode) {
    JAS_SYSTEM_OUTPUT_MODE = outputMode;
}

u32 getOutputMode() {
    return JAS_SYSTEM_OUTPUT_MODE;
}

void waitSubFrame() {
    u32 sf_start = getSubFrameCounter();
    do {
        OSYieldThread();
    } while (sf_start == getSubFrameCounter());
}

int rejectCallback(JASDriverCallBack cb, void *arg) {
    int ret = sDspSyncCallback.reject(cb, arg);
    ret += sSubFrameCallback.reject(cb, arg);
    ret += sUpdateDacCallback.reject(cb, arg);
    return ret;
}

bool registerDspSyncCallback(JASDriverCallBack cb, void *arg) {
    return sDspSyncCallback.regist(cb, arg);
}

bool registerSubFrameCallback(JASDriverCallBack cb, void *arg) {
    return sSubFrameCallback.regist(cb, arg);
}

void subframeCallback() {
    sSubFrameCallback.callback();
}

void DSPSyncCallback() {
    sDspSyncCallback.callback();
}

void updateDacCallback() {
    sUpdateDacCallback.callback();
}



}
