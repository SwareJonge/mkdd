#ifndef JUGEMPOINT_H
#define JUGEMPOINT_H

#include "Kaneshige/Course/CrsData.h"

class CLPoint;

class JugemPoint
{
public:
    JugemPoint();
    short getSectorID() const {
        return mPointData->mSectorID;
    }

private:
    const CrsData::SJugemPoint *mPointData;
    const CLPoint *mCLPoint;
};

#endif