//#include "dolphin/vec.h"
#include "JSystem/JMath/JMath.h"
#include "types.h"

void JMAEulerToQuat(s16 x, s16 y, s16 z, Quaternion *quat)
{
    f32 cosX = JMASCos(x / 2);
    f32 cosY = JMASCos(y / 2);
    f32 cosZ = JMASCos(z / 2);
    f32 sinX = JMASSin(x / 2);
    f32 sinY = JMASSin(y / 2);
    f32 sinZ = JMASSin(z / 2);

    quat->w = cosX * (cosY * cosZ) + sinX * (sinY * sinZ);
    quat->x = sinX * (cosY * cosZ) - cosX * (sinY * sinZ);
    quat->y = cosZ * (cosX * sinY) + sinZ * (sinX * cosY);
    quat->z = sinZ * (cosX * cosY) - cosZ * (sinX * sinY);
    
}