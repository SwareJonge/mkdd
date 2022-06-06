#ifndef RACETIME_H
#define RACETIME_H

class RaceTime
{
public:
    RaceTime();
    RaceTime(RaceTime const &);
    ~RaceTime();

private:
    u32 time;
};

#endif