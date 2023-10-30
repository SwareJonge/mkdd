#include "Kaneshige/ShapePktControl.h"

ShapePktControl::ShapePktControl() {
    mMagic = SHAPEPKT_MAGIC;
    mShapePkt = nullptr;
    mFlags = 0;
    _10 = 0;
}

void ShapePktControl::ctrlPacket(u32 idx)
{
    if (mFlags & 1 << idx)
        mShapePkt->hide();
    else
        mShapePkt->show();
}