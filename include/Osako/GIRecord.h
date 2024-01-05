#ifndef GIRECORD_H
#define GIRECORD_H

#include "Osako/TARecord.h"

class GIRecord {
public:
    GIRecord(TARecord &record);
    bool isValid();

    //operator const u8*() const { return (const u8 *)&mRecord; }

private:
    TARecord mRecord;
    u32 mTick;
    char mSignature[3]; // GID
    u8 mChecksum;
};

#endif
