#ifndef JUGEMPOINT_H
#define JUGEMPOINT_H

#include "Kaneshige/Course/CrsData.h"

class CLPoint;

class JugemPoint
{
public:
    JugemPoint();
    short getSectorID() const { return mPointData->sectorID; }
    void setCLPoint(CLPoint *clPoint) { mCLPoint = clPoint; }
    void setPointData(const CrsData::SJugemPoint *pointData) { mPointData = pointData; }
    bool isSameCPointID(u16 id) const { return mPointData->cPointID == id; }
    bool isSameJPointID(u16 id) const { return mPointData->id == id; }

    u16 getID() const { return mPointData->id; }

    void getPosition(JGeometry::TVec3f *pos)
    {
        if(mPointData) {
            pos->set(mPointData->position);
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