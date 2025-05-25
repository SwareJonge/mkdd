#ifndef JAUDIO_JASFAKEMATCH10_H
#define JAUDIO_JASFAKEMATCH10_H

#ifdef MATCHING

#include "JSystem/JAudio/JASFakeMatch.h"

template <int N>
class JAUAudible;
class JAIStream;
class JAISeq;
class JAISe;

typedef JAUAudible<4> TAudible4;

JASMemPool<TAudible4> JASPoolAllocObject<TAudible4>::memPool_;
JASMemPool<JAIStream> JASPoolAllocObject<JAIStream>::memPool_;
JASMemPool<JAISeq> JASPoolAllocObject<JAISeq>::memPool_;
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

#endif
