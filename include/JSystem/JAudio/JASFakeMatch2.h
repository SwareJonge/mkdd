#ifndef JAUDIO_JASFAKEMATCH2_H
#define JAUDIO_JASFAKEMATCH2_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAISe;

#include "JSystem/JAudio/JASFakeMatch.h"
template <>
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif
