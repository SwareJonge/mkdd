#ifndef JAUDIO_JAISOUNDINFO_H
#define JAUDIO_JAISOUNDINFO_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASGadget.h"

struct JAISoundInfo : public JASGlobalInstance<JAISoundInfo>
{
    JAISoundInfo(bool);
    virtual u32 getSoundType(JAISoundID) const = 0;
    virtual int getCategory(JAISoundID) const = 0;
    virtual u32 getPriority(JAISoundID) const = 0;
    virtual void getSeInfo(JAISoundID, JAISe *) const = 0;
    virtual void getSeqInfo(JAISoundID, JAISeq *) const = 0;
    virtual void getStreamInfo(JAISoundID, JAIStream *) const = 0;
    virtual ~JAISoundInfo();
};

#endif /* JAUDIO_JAISOUNDINFO_H */