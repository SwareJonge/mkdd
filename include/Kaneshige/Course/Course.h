#ifndef COURSE_H
#define COURSE_H

#include "types.h"

class Course
{
public:
    class Sector
    {
    public:
        s32 isMainSector();
        u8 getGeneration();
    };

    f32 getTrackSectorDist()
    {
        return mTrackSectorDist;
    }

private:
    u8 _0[0x514];
    f32 mTrackSectorDist;
};

#endif