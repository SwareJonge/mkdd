#ifndef JAUDIO_JASFAKEMATCH_H
#define JAUDIO_JASFAKEMATCH_H

#ifdef MATCHING
#include "JSystem/JAudio/System/JASMemPool.h"
#include "JSystem/JAudio/System/JASTrack.h"

JASMemPool_MultiThreaded<JASTrack::TChannelMgr> JASPoolAllocObject_MultiThreaded<JASTrack::TChannelMgr>::memPool_;
JASMemPool_MultiThreaded<JASTrack> JASPoolAllocObject_MultiThreaded<JASTrack>::memPool_;
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

#endif



#endif