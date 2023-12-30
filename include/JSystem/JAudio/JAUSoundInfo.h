#ifndef JAUDIO_JAUSOUNDINFO_H
#define JAUDIO_JAUSOUNDINFO_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASGadget.h"

class JAUSoundInfo : public JASGlobalInstance<JAUSoundInfo>
{
public:
    JAUSoundInfo(bool param_0) : JASGlobalInstance<JAUSoundInfo>(param_0) {}
    virtual void getAudibleSw(JAISoundID) const = 0;
    virtual u16 getBgmSeqResourceID(JAISoundID) const = 0;
};

#endif /* JAUDIO_JAUSOUNDINFO_H */