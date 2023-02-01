#ifndef SYSTEMRECORD_H
#define SYSTEMRECORD_H

#include "Kaneshige/Course/Course.h"
#include "Kaneshige/RaceInfo.h"
#include "Osako/GPRecord.h"
#include "Osako/TARecord.h"
#include "types.h"

struct SystemRecord {
    SystemRecord() {}
    void applyAudioSetting();
    TARecord * getBestLap(ECourseID);
    TARecord * getTARecord(ECourseID, int);

    u32 mOptions;
    u16 mUnlock1;
    u16 mUnlock2;
    u8 mVolume;
    u8 mItemSlotType;
    u8 mVsLapNum;
    char mDefaultName[4]; // AAA
    GPRecord mGPRecordData[4][5]; // stores all levels(50cc to mirror), and a top 5 of that
    TARecord mBestFinalTimes[16][5]; // course idx, rank
    TARecord mBestLapTimes[16];
    u32 _5d0;
    // not sure how many padding bytes there actually are, will become clear when SystemFile is decompiled
    u8 _5d4[0x614 - 0x5d4];
};

extern SystemRecord gSystemRecord; // bss size: 0x614

#endif