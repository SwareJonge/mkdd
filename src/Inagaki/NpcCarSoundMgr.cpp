#include "Inagaki/GameAudioCommon.h"
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"

#include "JSystem/JAudio/JASFakeMatch4.h"

namespace GameAudio {

u32 EngineID[4] = {
    0x00040011, 0x00040010, 0x00040010, 0x00040012
};

u32 HornID[4] = {
    0x00040015, 0x00040013, 0x00040013, 0x0004001D
};

NpcCarSoundMgr::NpcCarSoundMgr(Vec *pos, u8 p3, JKRHeap *heap) : SoundWithEchoMgr(pos, heap, 8) {
    if (p3 >= 4) {
        _69 = 0;
    }
    else {
        _69 = p3;
    }
    _68 = 0;
    _6a = 0;
}

NpcCarSoundMgr::~NpcCarSoundMgr() {}

void NpcCarSoundMgr::frameWork(f32 p1, bool p2) {
    if (_48)
        return;

    setHornSound(p2);
    setEngineSound(p1);
    checkEcho();
}

void NpcCarSoundMgr::setEngineSound(f32 p1) {
    if (p1 < 10.0f)
        return;

    if (p1 > 80.0f)
        p1 = 80.0f;

    f32 pitch = (p1 / 80.0f) * 0.9f + 0.3f;

    JAISoundHandle *handle = startLevelSound(EngineID[_69]);
    if (handle && *handle) {
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }
}

void NpcCarSoundMgr::setHornSound(bool p1) {
    bool changeSound = false;
    if (_68 != p1) {
        _68 = p1;
        _6a = 0;
    }
    if (_68) {
        if (_6a == 0) {
            if (!((Random::getRandomU32() % 16384) / 4096)) {
                changeSound = true;
            }
            _6a = 100;
        }
        else {
            _6a--;
        }
    }
    if (changeSound == true) {
        u32 newSoundID = Common::changeRandomId(HornID[_69], 0);
        JAISoundHandle *handle = startSound(newSoundID);
        setInitialEcho(handle);
    }
}

void NpcCarSoundMgr::setSe(u32 soundID) {
    if (_48)
        return;

    if (getHandleSoundID(soundID))
        return;

    startSoundCustom(soundID, 0);
}

}
