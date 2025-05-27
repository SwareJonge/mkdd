#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/stMath.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

CrsArea::CrsArea() : mPos()
{
    reset();
}

void CrsArea::reset()
{
    mPos.zero();
    mArea = nullptr;
    mRate = 0.0f;
}

void CrsArea::search(u8 id, const JGeometry::TVec3f &pos)
{
    mPos.set(pos);
    mArea = nullptr;
    mRate = 0.0f;

    if (RaceMgr::getManager() && RCMGetCourse())
    {
        int areaCnt = RCMGetCourse()->getAreaNumber();
        f32 prev = 0.0f;
        for (int i = 0; i < areaCnt; i++)
        {
            Course::Area *area = RCMGetCourse()->getArea(i);
            u8 kind = area->getKind();
            if (kind == id)
            {
                JGeometry::TVec3f areaPos;
                if (area->check(&areaPos, pos))
                {
                    if (id == 0)
                    {
                        f32 rate = area->getRate(areaPos);
                        if (rate > prev)
                        {
                            prev = rate;
                            mArea = area;
                            mRate = area->getRate(areaPos);
                        }
                    }
                    else
                    {
                        mArea = area;
                        mRate = area->getRate(areaPos);
                        return;
                    }
                }
            }
        }
    }
}

int CrsArea::getCameraID() const
{
    int cameraId = -1;

    if (isInside() && mArea->getKind() == 1)
    {
        cameraId = mArea->mData->cameraId;
    }
    return cameraId;
}

void CrsArea::getRoofPosition(JGeometry::TVec3f *out)
{
    if (isInside())
    {
        stPlaneParam plane;
        JGeometry::TVec3f vec;

        vec.scaleAdd(mArea->mScale.y, mArea->mYNorm, mArea->mPos);
        stMakePlaneParam(plane, mArea->mYNorm, vec);

        if (plane.y != 0.0f)
        {
            out->x = mPos.x;
            out->y = -(plane.d + (plane.x * mPos.x + plane.z * mPos.z)) / plane.y; // todo: get better understand of planeparam
            out->z = mPos.z;
        }
        else
        {
            out->zero();
        }
    }
    else
    {
        out->zero();
    }
}

f32 CrsArea::getEchoRate()
{
    f32 rate = 0.0f;

    if (isInside())
        rate = mRate * mArea->getEchoScale();

    return rate;
}

bool CrsArea::enableLightParam()
{
    bool ret = false;
    switch (mArea->getKind())
    {
    case cCrsAreaKindShadow:
    case 7:
        ret = true;
        break;
    }
    return ret;
}

s16 CrsArea::getLightParamID()
{
    s16 id = -1;
    if (isInside())
    {
#line 325
        JUT_ASSERT(enableLightParam());
        id = mArea->getLightParamID();
    }
    return id;
}

s16 CrsArea::getShadowID() const
{
    s16 id = 0;
    if (isInside())
    {
#line 347
        JUT_ASSERT(mArea->getKind() == cCrsAreaKindShadow);
        id = mArea->getShadowID();
    }
    return id;
}
