#ifndef JAUDIO_JASFAKEMATCH5_H
#define JAUDIO_JASFAKEMATCH5_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAIStream;
class JAISe;
class JAISeq;

#include "JSystem/JAudio/JASFakeMatch.h"

JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif