#ifndef JAUDIO_JASFAKEMATCH3_H
#define JAUDIO_JASFAKEMATCH3_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASTrack.h"
class JAIStream;
class JAISoundChild;
class JAISeq;
class JAISe;

#include "JSystem/JAudio/JASFakeMatch.h"
JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif