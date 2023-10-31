#include <JSystem/JAudio/JASFakeMatch2.h>

#include "Yamamoto/kartLine.h"

void Spline::getBezierPoint(JGeometry::TVec3f *out, f32 t)
{
    const float invT = (1.0f - t);
    float invTT = (invT * invT);
    float tt = t * t;
    float invTTT = invTT * invT;
    float invT3 = 3.0f * t * invTT;
    float unk1 = 3.0f * tt * invT;
    float ttt = tt * t;

    JGeometry::TVec3f *controlPoints[4];
    for (int i = 0; i < 4; i++)
    {
        controlPoints[i] = &mPoints[i];
    }

    out->x = controlPoints[0]->x * invTTT + controlPoints[1]->x * invT3 + controlPoints[2]->x * unk1 + controlPoints[3]->x * ttt;
    out->y = controlPoints[0]->y * invTTT + controlPoints[1]->y * invT3 + controlPoints[2]->y * unk1 + controlPoints[3]->y * ttt;
    out->z = controlPoints[0]->z * invTTT + controlPoints[1]->z * invT3 + controlPoints[2]->z * unk1 + controlPoints[3]->z * ttt;
}