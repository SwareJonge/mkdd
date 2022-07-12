#ifndef RACETIME_H
#define RACETIME_H

#include "types.h"

#define MAX_TIME 5999999
#define MAX_MS 999

class RaceTime
{
public:
    RaceTime() {
        reset();
    }
    // this constructor might not be needed
    /*RaceTime::RaceTime(RaceTime const & racetime) {
        value = racetime.value;
    }*/

    //inline void operator=(const RaceTime &);
    void get(int *, int *, int *) const;

    int get() const {
        return value;
    }

    int getUpwardMSec() const {
        return value + MAX_MS;
    }

    bool isAvailable() {
        return value != MAX_TIME;
    }

    bool isLittle(RaceTime const & raceTime) const {
        return value < raceTime.value;
    }

    void reset() {
        value = MAX_TIME;
    }

    void set(int frame) {
        value = frame; // no idea why framecount is used here, 
        if (value > MAX_TIME)
            value = MAX_TIME;
    }

    /*void set(RaceTime const & raceTime) {
        *this = raceTime;
    }*/

    void setFrame(int frame) {
        value = (frame * 1000) / 60;
        if (value > MAX_TIME)
            value = MAX_TIME;
    }

    void sub(const RaceTime & split1, const RaceTime & split2) {
        value = split1.value - split2.value;
    }

    void sub(int p1) {
        value = value - p1;
    }

    void zero() {
        value = 0;
    }

private:
    int value;
};

#endif