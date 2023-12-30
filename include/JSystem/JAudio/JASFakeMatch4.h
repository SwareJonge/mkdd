#ifndef JAUDIO_JASFAKEMATCH4_H
#define JAUDIO_JASFAKEMATCH4_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAISe;
class JAISeq;

#include "JSystem/JAudio/JASFakeMatch.h"
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif