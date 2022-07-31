#ifndef COURSE_H
#define COURSE_H

#include "types.h"

class Course
{
public:
    class Sector
    {
    public:
        s32 isMainSector() const;
        u8 getGeneration() const;
    };

    f32 getTrackSectorDist() const
    {
        return mTrackSectorDist;
    }

private:
    u8 _0[0x514];
    f32 mTrackSectorDist;
};

#endif