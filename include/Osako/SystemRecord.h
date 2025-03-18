#ifndef SYSTEMRECORD_H
#define SYSTEMRECORD_H

#include "kartEnums.h"
#include "Osako/GPRecord.h"
#include "Osako/TARecord.h"
#include "types.h"

struct SystemRecord
{
    enum GameFlag {
        KinopioCombi = 0,  // Toad and Toadette
        BossCombi = 1,     // Petey Piranha and King Boo
        Mirror = 2,        // Mirror Mode
        SpecialCup = 3,    // Special Cup
        AllCupTour = 4,    // All Cup Tour
        Ending = 5,        // Ending
        SpecialEnding = 6, // Special Ending
        MiniLuigi = 7,     // Luigi's Mansion
        MiniMario = 8,     // Tilt-A-Kart
    };

    enum SecretKartID {
        BabyLuigi = 0, // Rattle Buggy
        Patapata = 1,  // Para Wing
        Diddy = 2,     // Barrel Train
        KoopaJr = 3,   // Bullet Blaster
        Kinopio = 4,   // Toad Kart
        Kinopico = 5,  // Toadette Kart
        Luigi = 6,     // Green Fire
        Daisy = 7,     // Bloom Coach
        Catherine = 8, // Turbo Birdo
        Waluigi = 9,   // Waluigi Racer
        Pakkun = 10,   // Piranha Pipes
        Teressa = 11,  // Boo Pipes
        Extra = 12,    // Parade Kart
    };

    SystemRecord() {}
    void init();
    void crypt(u16 seed);

    void applyAudioSetting();

    TARecord *getTARecord(ECourseID, int rank);
    TARecord *getBestLap(ECourseID crsID);
    GPRecord &getGPRecord(ERaceGpCup cup, ERaceLevel level);

    int rankTARecord(ECourseID crsId, TARecord &record);

    void setBestLap(ECourseID crsID, TARecord &newBestLap);
    void setTARecord(ECourseID crsID, int rank, TARecord &newRecord); // Unused
    void setGPRecord(ERaceGpCup cup, ERaceLevel level, GPRecord &newRecord);

    static int convCourseID(ECourseID crsId);
    static int convKartID(EKartID kartId);
    static bool tstSecretKart(EKartID kartId, u16 flags);

    void setDefaultName(const char *name);

    void unlockGame(GameFlag flag) {
        mGameFlag ^= (1 << flag);
    }

    void unlockKart(int kart) {
        mSecretKart ^= (1 << kart);
    }

    bool isRumble() { return !(mOptions & 4); } 
    /*union
    {
    struct
    {*/
    int mOptions;
    u16 mSecretKart; // Unlockable karts
    u16 mGameFlag;   // unlockable characters, cups and maps
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
