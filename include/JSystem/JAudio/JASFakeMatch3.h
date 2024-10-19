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
template <>
JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
template <>
JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;
template <>
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
template <>
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif
