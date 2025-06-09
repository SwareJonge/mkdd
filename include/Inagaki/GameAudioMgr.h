#ifndef GAMEAUDIOMGR_H
#define GAMEAUDIOMGR_H

#include "JSystem/JAudio/JAUSoundMgr.h"

namespace GameAudio {

class CustomMgr : public JAUSoundMgr {
    CustomMgr();
    ~CustomMgr() {}
    bool startSound(JAISoundID soundID, JAISoundHandle *handlePtr, const JGeometry::TVec3f *pos);

private:
    u8 _80c;
    u8 _80d;
    u16 _80e;
};

}

#endif
