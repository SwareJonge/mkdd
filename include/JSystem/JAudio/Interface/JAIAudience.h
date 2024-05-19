#ifndef JAUDIO_JAIAUDIENCE_H
#define JAUDIO_JAIAUDIENCE_H

#include "JSystem/JAudio/Interface/JAIAudible.h"

class JAISoundID;

class JAIAudience
{
public:
    virtual ~JAIAudience();
    virtual JAIAudible *newAudible(const JGeometry::TVec3f &, JAISoundID, const JGeometry::TVec3f *, u32) = 0;
    virtual int getMaxChannels() = 0;
    virtual void deleteAudible(JAIAudible *) = 0;
    virtual u32 calcPriority(JAIAudible *) = 0;
    virtual void mixChannelOut(const JASSoundParams &, JAIAudible *, int) = 0;
};

# endif