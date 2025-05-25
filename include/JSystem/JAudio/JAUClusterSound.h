#ifndef JAUDIO_JAUCLUSTERSOUND_H
#define JAUDIO_JAUCLUSTERSOUND_H

#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JGeometry/Vec.h"
#include "types.h"

// Fabricated
template <int N>
class JAUClusterSound {
public:
    JAUClusterSound() {
        handles = new JAISoundHandle[N];
        pos = new JGeometry::TVec3f[N];
    }
    ~JAUClusterSound() {}

private:
    JAISoundHandle *handles;
    JGeometry::TVec3f *pos;
};

#endif
