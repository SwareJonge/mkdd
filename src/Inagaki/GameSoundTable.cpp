#include "Inagaki/GameSoundTable.h"
#include "JSystem/JAudio/Interface/JAISound.h"

#include "JSystem/JAudio/JASFakeMatch11.h"

namespace GameAudio {

u16 CustomSoundTable::getBgmSeqResourceID(JAISoundID soundID) const {}

u32 CustomSoundTable::getSoundType(JAISoundID soundID) const {}

int CustomSoundTable::getCategory(JAISoundID soundID) const {}

void CustomSoundTable::getAudibleSw(JAISoundID soundID) const {}

u8 CustomSoundTable::getPTrack(JAISoundID soundID) const {}

u32 CustomSoundTable::getSwBit(JAISoundID soundID) const {}

u8 CustomSoundTable::getLevelFlag(JAISoundID soundID) const {}

u32 CustomSoundTable::getPriority(JAISoundID soundID) const {}

void CustomSoundTable::getSeInfo(JAISoundID soundID, JAISe *se) const {}

void CustomSoundTable::getSeqInfo(JAISoundID soundID, JAISeq *seg) const {}

void CustomSoundTable::getStreamInfo(JAISoundID soundID, JAIStream *stream) const {}

JAISoundInfo *CustomSoundTable::getSoundInfo_(JAISoundID soundID, JAISound *sound) const {}

s32 CustomSoundTable::getStreamFileEntry(JAISoundID soundID) {}

void CustomSoundTable::initAudibleSw() {}

}
