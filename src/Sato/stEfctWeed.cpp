#include "Sato/stEfctWeed.h"
#include "Yamamoto/kartCtrl.h"

namespace
{
    static f32 sSpdMin = 15.0f;
    static f32 sSpdMax = 45.0f;
    static f32 sOffsetScale = 20.0f;
    static f32 sSpdRatio = 0.5f;
    static f32 sSpdDriftRatio = 0.5f; // unknown
    static f32 sSpdDif = 1.0f / (sSpdMax - sSpdMin);
}

#include <JSystem/JAudio/JASFakeMatch2.h>

stEfctWeedBase::stEfctWeedBase() {}

int stEfctWeedBase::getGeneratePos(JGeometry::TVec3f *pos, Mtx m)
{
    getTirePos(pos);

    bool ret; // bool to check if front tires are toching the ground? also, why is this a bool when the return is int?
    bool flip = false;
    if (_22 == 1)
    {
        ret = GetKartCtrl()->GetRightFTirePos(kartNum, (Vec *)pos);
        flip = true;
    }
    else
    {
        ret = GetKartCtrl()->GetLeftFTirePos(kartNum, (Vec *)pos);
    }

    if (!ret)
    {
        return ret;
    }
    else
    {
        JGeometry::TVec3f offset; // just use the ctor or setter?
        offset.x = m[0][0];
        offset.y = m[1][0];
        offset.z = m[2][0];

        if (flip)
            offset.negate();

        offset.scale(sOffsetScale);
        *pos += offset;
        return ret;
    }
}

stEfctWeed::stEfctWeed()
{
    ipData.Init(4, nullptr, nullptr);
    ipData.AddFunc(stEmitterParamLineIP::Life);
    ipData.AddFunc(stEmitterParamLineIP::CreateRate);
    ipData.AddFunc(stEmitterParamLineIP::RateStep);
    ipData.AddFunc(stEmitterParamLineIP::FirstVel);
}

void stEfctWeed::calc()
{
    strategy(sSpdRatio, sSpdMax, sSpdMin, sSpdDif, false, 0.95f);
}

// Unused functions

stEfctWeedDrift::stEfctWeedDrift()
{
    ipData.Init(4, nullptr, nullptr);
    ipData.AddFunc(stEmitterParamLineIP::Life);
    ipData.AddFunc(stEmitterParamLineIP::CreateRate);
    ipData.AddFunc(stEmitterParamLineIP::RateStep);
    ipData.AddFunc(stEmitterParamLineIP::FirstVel);
}

void stEfctWeedDrift::calc()
{
    strategy(sSpdDriftRatio, sSpdMax, sSpdMin, sSpdDif, false, 0.95f);
}
