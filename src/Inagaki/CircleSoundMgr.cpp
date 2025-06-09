
#include "Inagaki/GameSoundMgr.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

namespace GameAudio {

CircleSoundMgr::CircleSoundMgr(Vec *pos, f32, f32, f32, JKRHeap *heap) : SoundMgr<CircleSoundMgr>(pos, heap, 4) {
    
}

CircleSoundMgr::~CircleSoundMgr() {}

void CircleSoundMgr::loop() {}

void CircleSoundMgr::frameWork() {}

void CircleSoundMgr::calcPlayPos(u32) {}

void CircleSoundMgr::setSe(u32) {}

}
