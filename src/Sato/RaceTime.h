#ifndef RACETIME_H
#define RACETIME_H

class RaceTime
{
public:
    RaceTime();
    RaceTime(RaceTime const &);

private:
    u32 time;
};

#endif