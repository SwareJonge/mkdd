#include "Kaneshige/RaceTime.h"

#define MAX_TIME 5999999
#define MAX_MS 999

RaceTime::RaceTime() {
    reset();
}

// this constructor might not be needed
/*RaceTime::RaceTime(RaceTime const & racetime) {
    time = racetime.time;
}*/ 

int RaceTime::get() const {
    return time;
}

void RaceTime::get(int *minutes, int * seconds, int *miliseconds) const {
    int _time = time;
    if (_time < 0) {
        _time = -_time;
    }

    *miliseconds = _time % 1000;
    *seconds = (_time / 1000) % 60;
    *minutes = (_time / 1000) / 60;

    if (time < 0)
        *minutes = -*minutes;

}

int RaceTime::getUpwardMSec() const {
    return time + MAX_MS;
}

void RaceTime::set(int frame) {
    time = frame; // no idea why framecount is used here, 
    if (time > MAX_TIME)
        time = MAX_TIME;
}

void RaceTime::set(RaceTime const & raceTime) {
    *this = raceTime;
}

void RaceTime::reset() {
    time = MAX_TIME;
}

void RaceTime::setFrame(int frame) {
    time = (frame * 1000) / 60;
    if (time > MAX_TIME)
        time = MAX_TIME;
}

bool RaceTime::isAvailable() {
    return time != MAX_TIME;
}

bool RaceTime::isLittle(RaceTime const & raceTime) const {
    return time < raceTime.time;
}

void RaceTime::sub(const RaceTime & split1, const RaceTime & split2) {
    time = split1.time - split2.time;
}

void RaceTime::sub(int p1) {
    time = time - p1;
}

void RaceTime::zero() {
    time = 0;
}