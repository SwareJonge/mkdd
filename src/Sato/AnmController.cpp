#include "Sato/AnmController.h"
#include "JSystem/JSupport/JSUList.h"

void AnmController::Reset() // 0x8026f950
{
    ResetAnm();
    ResetMat();
}

void AnmController::ResetMat() // 0x8026f99c
{
    if (_08 != nullptr) {
        for (JSUListIterator<AnmControlBase> it(mCtrls->getFirst()); it != mCtrls->getEnd(); ++it)
        {
            it->resetMatAnm();
        }
    }
}
