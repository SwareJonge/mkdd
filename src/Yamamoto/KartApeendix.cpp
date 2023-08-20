#include "Yamamoto/kartCtrl.h"

void KartAppendix::Init(int idx) {
    if (mKartLoader->getWheelNumber() == 6)
    {
        mModelWheel = mKartLoader->getExModelWheel(idx + 4);
    }
}