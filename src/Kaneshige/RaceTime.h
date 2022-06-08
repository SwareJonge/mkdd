#ifndef RACETIME_H
#define RACETIME_H

class RaceTime
{
public:
    inline RaceTime();
    inline RaceTime(RaceTime const &);

private:
    u32 time;
};

#endif