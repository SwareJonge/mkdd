
#include "Inagaki/GameMapSoundMgr.h"

#include "JSystem/JAudio/JASFakeMatch4.h"

namespace GameAudio {

MapSoundObjectMgr::MapSoundObjectMgr(Vec *pos, Type type, JKRHeap *heap) : SoundWithEchoMgr<MapSoundObjectMgr>(pos, heap, 2) {
    _68 = 0;
    mType = type;
}

MapSoundObjectMgr::~MapSoundObjectMgr() {}

void MapSoundObjectMgr::frameWork() {}

void MapSoundObjectMgr::getPlayerGoalRank() {}

}
