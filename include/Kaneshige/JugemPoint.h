#ifndef JUGEMPOINT_H
#define JUGEMPOINT_H

#include "Kaneshige/Course/CrsData.h"

class JugemPoint
{
public:
    short getSectorID() const {
        return mPointData->mSectorID;
    }

private:
    const CrsData::SJugemPoint *mPointData;
    const void *mCLPoint;
};

#endif