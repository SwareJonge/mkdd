#ifndef KARTPADRECORD_H
#define KARTPADRECORD_H

#include "Osako/TARecord.h"
#include "Osako/KartPadData.h"

// ghost files can only store up to 17864 frames, this is about 5 minutes(4:57.733 assumeing exact 60 fps, 4:58.031 assuming double ntsc/59.94) including countdown

#define MAX_INPUT_FRAME_COUNT 17864
#define NUM_LAP_SPLITS 6

class KartPadRecord
{
public:
    // fabricated
    void setFrame(int frame)
    {
        // clamp frame
        if (frame < MAX_INPUT_FRAME_COUNT)
            mFrame = frame;
        else
            mFrame = MAX_INPUT_FRAME_COUNT;
    }

    void getPadData(u32 frame, KartPadData *padData)
    {
        if (frame <= mFrame && frame <= MAX_INPUT_FRAME_COUNT)
        {
            *padData = mPadData[frame];
            return;
        }
        padData->mStickX = 0;
        padData->mStickY = 0;
        padData->mButtons = 0;
    }

    void setPadData(u32 frame, KartPadData *padData)
    {
        if (frame <= MAX_INPUT_FRAME_COUNT)
        {
            mPadData[frame] = *padData;
        }
    }

    TARecord &getTARecord()
    {
        return mRecord;
    };

private:
    TARecord mRecord;
    int mFrame;                          // Amount of input frames
    RaceTime mLapSplits[NUM_LAP_SPLITS]; // Stores the time when crossing the finish line, subtract with time from previous lap(except for lap 1 and 7) to get actual lap time
    KartPadData mPadData[MAX_INPUT_FRAME_COUNT];
}; // Size: 0x8bb8

#endif