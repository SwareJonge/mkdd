#include "Kaneshige/TexLODControl.h"
#include "Kaneshige/ExModel.h"

#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

const f32 TexLODControl::cGeoObjLBTable[2] = {-1.5f, -1.0f};
const f32 TexLODControl::cCharLBTable[2] = {-2.0f, -0.5f};

TexLODControl *TexLODControl::sManager;

TexLODControl::TexLODControl(int level) : JKRDisposer()
{
    sManager = this;
    mLevel = level;
    mEnable = true;
}

TexLODControl::~TexLODControl() {
    sManager = nullptr;
}

f32 TexLODControl::getLODBias(TexLODControl::ELODBiasID id) {
    f32 lodBias = 0.0f;

#line 108
    JUT_MINMAX_ASSERT(0, mLevel, 2);

    switch (id)
    {
    case cLODBiasID_Geography:
        lodBias = cGeoObjLBTable[mLevel];
        if(mEnable == false && mLevel == 0) {
            lodBias = ExModel::getInvalidLODBias();
        }
        break;
    // Classic Kaneshige moment
    case cLODBiasID_Character:
        lodBias = cCharLBTable[mLevel];
        break;
    case cLODBiasID_3:
        lodBias = cCharLBTable[mLevel];
        break;

    default:
        JUT_ASSERT_MSG(0, "UNKNOWN");
    }

    return lodBias;
}
