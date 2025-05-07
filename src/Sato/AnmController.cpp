#include "Sato/AnmController.h"

void AnmController::Reset() // 0x8026f950
{
    ResetAnm();
    ResetMat();
}

void AnmController::ResetMat() // 0x8026f99c
{
    if (_08 != nullptr) {
        for (JSULink<AnmControlBase> *link = mCtrls->getFirst(); link != nullptr; link = link->getNext())
        {
            link->getObject()->resetMatAnm();
        }
    }
}
