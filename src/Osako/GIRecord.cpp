#include <dolphin/os.h>
#include "Osako/GIRecord.h"

GIRecord::GIRecord(TARecord &record) : mRecord(record) {
    mTick = OSGetTick();
    mSignature[0] = 'G';
    mSignature[1] = 'I';
    mSignature[2] = 'D';

    mChecksum = 0;
    for (u8 *data = (u8 *)this; data != &mChecksum; data++)
        mChecksum += *data;
}

bool GIRecord::isValid() {
    u8 checksum = 0;
    for (u8 *data = (u8 *)this; data != &mChecksum; data++)
        checksum += *data;
    
    if (checksum == mChecksum && 
        (mSignature[0] == 'G' && mSignature[1] == 'I' && mSignature[2] == 'D'))
        return true;

    return false;
}