

#include "JSystem/JAudio/Interface/JAISound.h"

#ifdef DEBUG
#pragma sym on // TODO: it makes much more sense that this done through compilere flags, yet this screws up the order
#endif

JAISoundInfo::JAISoundInfo(bool p1) : JASGlobalInstance<JAISoundInfo>(p1)
{
}

JAISoundInfo::~JAISoundInfo()
{
#line 14
    JUT_ASSERT(false);
}