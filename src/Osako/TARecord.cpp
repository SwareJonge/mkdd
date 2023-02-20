#include <dolphin/string.h>
#include "Osako/TARecord.h"
#include "types.h"

TARecord::TARecord(u8 charId1, u8 charId2, u8 kartId, u8 courseId) {
    setStatus(charId1, charId2, kartId, courseId);
}

void TARecord::setName(const char * name) {
    strncpy(mName, name, sizeof(mName)); // No buffer overflow!
}

void TARecord::setStatus(u8 charId1, u8 charId2, u8 kartId, u8 courseId) {
    mCharID1 = charId1;
    mCharID2 = charId2;
    mKartID = kartId;
    mCourseID = courseId;
}

void TARecord::setRaceTime(RaceTime raceTime) {
    mTime = raceTime;
}

void TARecord::set(u8 charId1, u8 charId2, u8 kartId, u8 courseId, const char * name, RaceTime raceTime) {
    mCharID1 = charId1;
    mCharID2 = charId2;
    mKartID = kartId;
    mCourseID = courseId;
    strncpy(mName, name, sizeof(mName));
    setRaceTime(raceTime);
}

bool TARecord::less(const TARecord & mRec) {
    if(mKartID == 0xff)
        return true;

    return mRec.getRaceTime().isLittle(mTime);
}
