#include "Yamamoto/kartCtrl.h"

#ifndef DEBUG // TODO: fix
#include "JSystem/JAudio/JASFakeMatch2.h"
#endif

void KartAppendix::Init(int idx) {
    if (mKartLoader->getWheelNumber() == 6)
    {
        mModelWheel = mKartLoader->getExModelWheel(idx + 4);
    }
}