#include "JSystem/JAudio/System/JASProbe.h"

#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "dolphin/os.h"

JASProbe* JASProbe::sProbeTable[NUM_PROBES];

void JASProbe::start(const char *name) {
    JAS_CS_START
    mName          = name;
    mStartTime     = OSGetTime();
    JAS_CS_END
}

void JASProbe::stop() {
    JAS_CS_START
    _08  = (u32)OSGetTime() - mStartTime;
    _08 /= OS_TIMER_CLOCK / 59.94f;
    if (_10 < _08 && 100 < _1A8) {
        _10 = _08;
    }
    _0C             = _0C * 0.96f + _08 * 0.04f;
    u32 resultIndex = _1A8 % 100;
    _14 -= _18[resultIndex];
    _18[resultIndex] = _08;
    _14 += _08;
    _1A8++;
    JAS_CS_END
}

void JASProbe::start(s32 index, const char *name) {
    if (index < NUM_PROBES) {
        if (sProbeTable[index] != NULL) {
            sProbeTable[index]->start(name);
        }
    }
}

void JASProbe::stop(s32 index) {
    if (index < NUM_PROBES) {
        if (sProbeTable[index] != NULL) {
            sProbeTable[index]->stop();
        }
    }
}
