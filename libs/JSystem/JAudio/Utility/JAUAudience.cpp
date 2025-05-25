#include "JSystem/JAUdio/JAUAudience.h"
#include "JSystem/JUtility/JUTAssert.h"

void JAUAudience_withSetting::updateSetting() {
#line 30
    JUT_ASSERT(setting_.minVolume >= 0.0f);
    setting_._3c = (setting_._20 - setting_._1c) / setting_._10;
    setting_._40 = (setting_._18 - setting_._1c) / setting_._14;
    setting_._44 = (setting_.minVolume - 1.0f) / (setting_._04 - setting_._08);
    setting_._48 = (f32)setting_._38 / (setting_._04 - setting_._08);
    setting_._4c = (setting_._28 - setting_._24) / (setting_._04 - setting_._08);
}

namespace {

    void testInstanciate() {
        JAUGenericAudible_<4, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos> *audible = new JAUGenericAudible_<4, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos>[4];
        JAUDopplerAudience<4> *audience = new JAUDopplerAudience<4>[4];
    }
}
