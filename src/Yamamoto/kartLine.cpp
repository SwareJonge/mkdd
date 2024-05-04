#include <JSystem/JAudio/JASFakeMatch2.h>

#include "Yamamoto/kartLine.h"

void Spline::getBezierPoint(JGeometry::TVec3f *out, f32 t)
{
    const f32 invT = (1.0f - t);
    f32 invT2 = (invT * invT);
    f32 t2 = t * t;
    f32 invT3 = invT2 * invT;
    f32 bezierCoefficient3 = 3.0f * t * invT2;
    f32 bezierCoefficient2 = 3.0f * t2 * invT;
    f32 t3 = t2 * t;

    JGeometry::TVec3f *controlPoints[4];
    for (int i = 0; i < 4; i++)
        controlPoints[i] = &mPoints[i];

    out->x = controlPoints[0]->x * invT3 + controlPoints[1]->x * bezierCoefficient3 + controlPoints[2]->x * bezierCoefficient2 + controlPoints[3]->x * t3;
    out->y = controlPoints[0]->y * invT3 + controlPoints[1]->y * bezierCoefficient3 + controlPoints[2]->y * bezierCoefficient2 + controlPoints[3]->y * t3;
    out->z = controlPoints[0]->z * invT3 + controlPoints[1]->z * bezierCoefficient3 + controlPoints[2]->z * bezierCoefficient2 + controlPoints[3]->z * t3;
}