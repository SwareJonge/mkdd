#include "Yamamoto/kartCtrl.h"

//#include "JSystem/JAudio/JASFakeMatch2.h"

void KartAppendix::Init(int idx) {
    if (mKartLoader->getWheelNumber() == 6)
    {
        mModelWheel = mKartLoader->getExModelWheel(idx + 4);
    }
}