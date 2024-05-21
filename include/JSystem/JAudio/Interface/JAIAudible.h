#ifndef JAUDIO_JAIAUDIBLE_H
#define JAUDIO_JAIAUDIBLE_H

#include "JSystem/JGeometry.h"
#include "JSystem/JAudio/System/JASSoundParams.h"

class JAIAudible
{
public:
    virtual ~JAIAudible();
    virtual JASSoundParams *getOuterParams(int chan) = 0;
    virtual void calc() = 0;

    JGeometry::TVec3f &getPos() { return mPos; }
    void setPos(const JGeometry::TVec3f &other) { mPos.set(other); }

    JGeometry::TVec3f mPos;
};

#endif /* JAUDIO_JAIAUDIBLE_H */