#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JAudio/System/JASWaveArcLoader.h"

bool JASWaveBank::isLoading() {
    for( u32 i = 0; i < getArcCount(); i++ ) {
        JASWaveArc *arc = getWaveArc(i);
        if (arc && arc->isLoading()) {
            return true;
        }
    }
    return false;
}
