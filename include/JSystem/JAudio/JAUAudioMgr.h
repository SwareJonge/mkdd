#ifndef JAUDIO_JAUAUDIOMGR_H
#define JAUDIO_JAUAUDIOMGR_H

#include "JSystem/JAudio/JAUAudience.h"
#include "JSystem/JAudio/JAUSoundInfo.h"
#include "JSystem/JAudio/JAUSoundMgr.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/System/JASHeap.h"

// fabricated
class JAUAudioMgr : public JAUSoundMgr, JAUSoundInfo {
public:
    JAUAudioMgr(JAUSectionHeap *);
private:
    //JAUAudience<4> audience;
    JASHeap mHeaps[4];
};

#endif
