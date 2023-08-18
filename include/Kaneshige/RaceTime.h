#ifndef RACETIME_H
#define RACETIME_H

#include "types.h"

#define MAX_TIME 5999999
#define MAX_MS 999

class RaceTime
{
public:
    RaceTime()
    {
        reset();
    }
    // this constructor might not be needed
    /*RaceTime::RaceTime(RaceTime const & racetime) {
        mTime = racetime.mTime;
    }*/

    // inline void operator=(const RaceTime &);
    void get(int *, int *, int *) const;

    int get() const
    {
        return mTime;
    }

    int getUpwardMSec() const
    {
        return mTime + MAX_MS;
    }

    bool isAvailable() const
    {
        return mTime != MAX_TIME;
    }

    bool isLittle(RaceTime const &raceTime) const
    {
        return mTime < raceTime.mTime;
    }

    void reset()
    {
        mTime = MAX_TIME;
    }

    void set(int frame)
    {
        mTime = frame; // no idea why framecount is used here,
        if (mTime > MAX_TIME)
            mTime = MAX_TIME;
    }

    void set(RaceTime const & raceTime) {
        *this = raceTime;
    }

    void setFrame(int frame)
    {
        mTime = (frame * 1000) / 60;
        if (mTime > MAX_TIME)
            mTime = MAX_TIME;
    }

    void sub(const RaceTime &split1, const RaceTime &split2)
    {
        mTime = split1.mTime - split2.mTime;
    }

    void sub(int p1)
    {
        mTime = mTime - p1;
    }

    void zero()
    {
        mTime = 0;
    }

private:
    int mTime; // time in miliseconds
};

#endif