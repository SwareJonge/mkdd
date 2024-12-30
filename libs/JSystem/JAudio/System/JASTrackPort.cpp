#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JUtility/JUTAssert.h"

void JASTrackPort::init() {
    for(u32 i = 0; i < MAX_PORTS; i++) {
        mPortData[i] = 0;
    }
    mImportFlags = 0;
    mExportFlags = 0;
}

u16 JASTrackPort::readImport(u32 port_num) {
#line 27
    JUT_ASSERT(port_num < MAX_PORTS);
    mImportFlags &= ~(1 << port_num);
    return mPortData[port_num];
}

u16 JASTrackPort::readExport(u32 port_num) {
#line 34
    JUT_ASSERT(port_num < MAX_PORTS);
    mExportFlags &= ~(1 << port_num);
    return mPortData[port_num];
}

void JASTrackPort::writeImport(u32 port_num, u16 import) {
#line 41
    JUT_ASSERT(port_num < MAX_PORTS);
    mImportFlags |= (1 << port_num);
    mPortData[port_num] = import;
}

void JASTrackPort::writeExport(u32 port_num, u16 import) {
#line 47
    JUT_ASSERT(port_num < MAX_PORTS);
    mExportFlags |= (1 << port_num);
    mPortData[port_num] = import;
}

bool JASTrackPort::checkImport(u32 port_num) const { return mImportFlags & (1 << port_num); }

bool JASTrackPort::checkExport(u32 port_num) const { return mExportFlags & (1 << port_num); }
