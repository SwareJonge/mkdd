#include "Kaneshige/KartInfo.h"

void KartInfo::reset() {

    kartDB = 0;

    s32 charCount = 2;
    for (s32 i = 0; i < charCount; i++) {
        kartCharacter[i].kartGamePad = 0;
        kartCharacter[i].charDB = 0;
    }

    kartType = 0;
}