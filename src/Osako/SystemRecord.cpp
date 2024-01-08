#include <string.h>
#include <dolphin/os.h>
#include <JSystem/JMath/JMath.h>
#include "Inagaki/GameAudioMain.h"
#include "Osako/SystemRecord.h"

SystemRecord gSystemRecord;

#include "JSystem/JAudio/JASFakeMatch2.h"

static const char scaName[] = "AAA";

void SystemRecord::init()
{
    mOptions = 0;
    if (OSGetSoundMode() == 0)
    {
        mOptions &= ~0x3;
        mOptions |= 1;
    }

    mGameFlag = 0;
    mSecretKart = 0;
    mVolume = 0;
    mVsLapNum = 0;
    mItemSlotType = 0;

    strncpy(mDefaultName, scaName, 4);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++) // typo? there are 4 levels
        {
            mGPRecordData[i][j].reset();
        }
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            mBestFinalTimes[i][j].reset();
        }
        mBestLapTimes[i].reset();
    }

    mTimesFetched = 0;
}

void SystemRecord::applyAudioSetting()
{
    if (OSGetSoundMode() == 0)
    {
        mOptions &= ~0x3;
        mOptions |= 1;
    }
    else if ((mOptions & 3) == 1)
    {
        mOptions &= ~0x3;
    }

    switch (mOptions & 3)
    {
    case 1:
        GameAudio::Main::getAudio()->setOutputMode(0);
        break;
    case 0:
        GameAudio::Main::getAudio()->setOutputMode(1);
        break;
    case 2:
        GameAudio::Main::getAudio()->setOutputMode(2);
        break;
    }

    GameAudio::Main::getAudio()->setMasterVolume(mVolume);
}

void SystemRecord::crypt(u16 seed)
{
    JMath::TRandom_fast_ rndm(0);
    rndm.setSeed(seed);

    u32 *data = (u32 *)this;

    for (int i = 0; i < sizeof(SystemRecord) / 4; i++)
    {
        data[i] ^= rndm.get();
    }
}

int SystemRecord::convCourseID(ECourseID crsId)
{
    switch (crsId)
    {
    case BABY_PARK:
        return 0;
    case PEACH_BEACH:
        return 1;
    case DAISY_CRUISER:
        return 2;
    case LUIGI_CIRCUIT:
        return 3;
    case MARIO_CIRCUIT:
        return 4;
    case YOSHI_CIRCUIT:
        return 5;
    case MUSHROOM_BRIDGE:
        return 6;
    case MUSHROOM_CITY:
        return 7;
    case WALUIGI_STADIUM:
        return 8;
    case WARIO_COLOSSEUM:
        return 9;
    case DINO_DINO_JUNGLE:
        return 10;
    case DK_MOUNTAIN:
        return 11;
    case BOWSER_CASTLE:
        return 12;
    case RAINBOW_ROAD:
        return 13;
    case DRY_DRY_DESERT:
        return 14;
    case SHERBET_LAND:
        return 15;
    default:
        return 0;
    }
}

int SystemRecord::convKartID(EKartID kartId)
{
    switch (kartId)
    {
    case GREEN_FIRE:
        return 6;
    case BARREL_TRAIN:
        return 2;
    case TURBO_BIRDO:
        return 8;
    case PARA_WING:
        return 1;
    case BLOOM_COACH:
        return 7;
    case RATTLE_BUGGY:
        return 0;
    case WALUIGI_RACER:
        return 9;
    case BULLET_BLASTER:
        return 3;
    case TOAD_KART:
        return 4;
    case TOADETTE_KART:
        return 5;
    case BOO_PIPES:
        return 11;
    case PIRANHA_PIPES:
        return 10;
    case PARADE_KART:
        return 12;
    default:
        return 13;
    }
}

TARecord *SystemRecord::getTARecord(ECourseID crsId, int rank)
{
#line 216
    JUT_MINMAX_ASSERT(0, rank, 5);
    return &mBestFinalTimes[convCourseID(crsId)][rank];
}

int SystemRecord::rankTARecord(ECourseID crsId, TARecord &record)
{
    int rank = 4;
    int crsNo = convCourseID(crsId);

    while (rank >= 0 && mBestFinalTimes[crsNo][rank].less(record))
    {
        if (rank < 4)
        {
            mBestFinalTimes[crsNo][rank + 1] = mBestFinalTimes[crsNo][rank];
        }

        mBestFinalTimes[crsNo][rank] = record;
        rank--;
    }
    if (rank == 4)
    {
        return 0;
    }

    return rank + 2;
}

TARecord *SystemRecord::getBestLap(ECourseID crsID)
{
    return &mBestLapTimes[convCourseID(crsID)];
}

void SystemRecord::setBestLap(ECourseID crsID, TARecord &newBestLap)
{
    mBestLapTimes[convCourseID(crsID)] = newBestLap;
}

GPRecord *SystemRecord::getGPRecord(ERaceGpCup cup, ERaceLevel level)
{
#line 284
    JUT_MINMAX_ASSERT(0, cup, 5);
    return &mGPRecordData[cup][level];
}

void SystemRecord::setGPRecord(ERaceGpCup cup, ERaceLevel level, GPRecord &record)
{
#line 294
    JUT_MINMAX_ASSERT(0, cup, 5);
    mGPRecordData[cup][level] = record;
}

bool SystemRecord::tstSecretKart(EKartID kartId, u16 flags)
{
    int kartNo = convKartID(kartId);

    return kartNo == 13 || (flags & 1 << kartNo);
}

void SystemRecord::setDefaultName(const char *name)
{
    strncpy(mDefaultName, name, 4);
}