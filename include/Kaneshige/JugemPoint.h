#ifndef JUGEMPOINT_H
#define JUGEMPOINT_H

#include "Kaneshige/Course/CrsData.h"

class CLPoint;

class JugemPoint
{
public:
    JugemPoint();
    short getSectorID() const { return mPointData->mSectorID; }
    void setCLPoint(CLPoint *clPoint) { mCLPoint = clPoint; }
    void setPointData(const CrsData::SJugemPoint *pointData) { mPointData = pointData; }
    bool isSameCPointID(u16 id) const { return mPointData->mCPointID == id; }
    bool isSameJPointID(u16 id) const { return mPointData->mID == id; }

    u16 getID() const { return mPointData->mID; }

    void getPosition(JGeometry::TVec3f *pos)
    {
        if(mPointData) {
            pos->set(mPointData->mPosition);
            return;
        }
        pos->zero();
    }

    void getFrDirection(JGeometry::TVec3f *dir) const
    {
        if(mPointData) {
            dir->x = 0.0001f * mPointData->dirX;
            dir->y = 0.0001f * mPointData->dirY;
            dir->z = 0.0001f * mPointData->dirZ;
        }
    }

private:
    const CrsData::SJugemPoint *mPointData;
    CLPoint *mCLPoint;
};

#endif