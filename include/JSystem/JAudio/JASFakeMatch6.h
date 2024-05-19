#ifndef JAUDIO_JASFAKEMATCH6_H
#define JAUDIO_JASFAKEMATCH6_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAISoundChild;
class JAISeq;

#include "JSystem/JAudio/JASFakeMatch.h"

JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;

#endif

#endif