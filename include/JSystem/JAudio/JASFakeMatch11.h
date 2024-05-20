#ifndef JAUDIO_JASFAKEMATCH10_H
#define JAUDIO_JASFAKEMATCH10_H

#ifdef MATCHING

#include "JSystem/JAudio/JASFakeMatch.h"
class JAISoundChild;
class JAIStream;
class JAISeq;
class JAISe;

JASMemPool<JAISoundChild> JASPoolAllocObject<JAISoundChild>::memPool_;
JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif