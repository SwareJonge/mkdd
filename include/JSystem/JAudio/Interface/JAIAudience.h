#ifndef JAUDIO_JAIAUDIENCE_H
#define JAUDIO_JAIAUDIENCE_H

#include "JSystem/JAudio/Interface/JAIAudible.h"

class JAISoundID;

class JAIAudience
{
public:
    virtual ~JAIAudience();
    virtual JAIAudible *newAudible(const JGeometry::TVec3f &, JAISoundID soundID, const JGeometry::TVec3f *, u32 channels) = 0;
    virtual int getMaxChannels() = 0;
    virtual void deleteAudible(JAIAudible *audible) = 0;
    virtual u32 calcPriority(JAIAudible *audible) = 0;
    virtual void mixChannelOut(const JASSoundParams &params, JAIAudible *audible, int channel) = 0;
};

# endif