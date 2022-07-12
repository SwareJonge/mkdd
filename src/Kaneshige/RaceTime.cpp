#include "Kaneshige/RaceTime.h"

void RaceTime::get(int *minutes, int * seconds, int *miliseconds) const {
    int _time = value;
    if (_time < 0) {
        _time = -_time;
    }

    *miliseconds = _time % 1000;
    *seconds = (_time / 1000) % 60;
    *minutes = (_time / 1000) / 60;

    if (value < 0)
        *minutes = -*minutes;

}