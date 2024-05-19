#ifndef JAUDIO_JASFAKEMATCH7_H
#define JAUDIO_JASFAKEMATCH7_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAISoundChild;

#include "JSystem/JAudio/JASFakeMatch.h"

JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;

#endif

#endif