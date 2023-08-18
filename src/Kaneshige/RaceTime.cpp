#include "Kaneshige/RaceTime.h"

void RaceTime::get(int *minutes, int * seconds, int *miliseconds) const {
    int absTime = mTime;
    if (absTime < 0) {
        absTime = -absTime;
    }

    *miliseconds = absTime % 1000;
    *seconds = (absTime / 1000) % 60;
    *minutes = (absTime / 1000) / 60;

    if (mTime < 0)
        *minutes = -*minutes;

}