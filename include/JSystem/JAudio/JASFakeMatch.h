#ifndef JAUDIO_JASFAKEMATCH_H
#define JAUDIO_JASFAKEMATCH_H

// NOTE it's not really a fakematch since this is probably what they've done, however with seperate headers it's easier to control what TU gets it
// should've been declared in a .cpp file

#ifdef MATCHING
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASTrack.h"

template <>
JASMemPool_MultiThreaded<JASTrack::TChannelMgr> JASPoolAllocObject_MultiThreaded<JASTrack::TChannelMgr>::memPool_;
template <>
JASMemPool_MultiThreaded<JASTrack> JASPoolAllocObject_MultiThreaded<JASTrack>::memPool_;
template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

#endif



#endif
