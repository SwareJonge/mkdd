#ifndef SYSTEMRECORD_H
#define SYSTEMRECORD_H

#include "kartEnums.h"
#include "Kaneshige/RaceInfo.h"
#include "Osako/GPRecord.h"
#include "Osako/TARecord.h"
#include "types.h"

struct SystemRecord
{
    SystemRecord() {}
    void init();
    void crypt(u16 seed);

    void applyAudioSetting();

    TARecord *getTARecord(ECourseID, int rank);
    TARecord *getBestLap(ECourseID crsID);
    GPRecord *getGPRecord(ERaceGpCup cup, ERaceLevel level);

    int rankTARecord(ECourseID crsId, TARecord &record);

    void setBestLap(ECourseID crsID, TARecord &newBestLap);
    void setTARecord(ECourseID crsID, int rank, TARecord &newRecord); // Unused
    void setGPRecord(ERaceGpCup cup, ERaceLevel level, GPRecord &newRecord);

    static int convCourseID(ECourseID crsId);
    static int convKartID(EKartID kartId);
    static bool tstSecretKart(EKartID kartId, u16 flags);

    void setDefaultName(const char *name);

    /*union
    {
    struct
    {*/
    int mOptions;
    u16 mGameFlag;   // unlockable characters, cups and maps
    u16 mSecretKart; // Unlockable karts
    s8 mVolume;
    u8 mItemSlotType;
    u8 mVsLapNum;
    char mDefaultName[4];            // AAA
    GPRecord mGPRecordData[5][4];    // stores all levels(50cc to mirror) of all 5 cups
    TARecord mBestFinalTimes[16][5]; // course idx, rank
    TARecord mBestLapTimes[16];
    u32 mTimesFetched;
    /*};
    u32 data[0x185];
    };*/
};

extern SystemRecord gSystemRecord; // bss size: 0x614(bss bug)

#endif