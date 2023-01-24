#include "Osako/password.h"

const ECourseID caRaceCrsID[] = {
    LUIGI_CIRCUIT, PEACH_BEACH, BABY_PARK, DRY_DRY_DESERT,
    MUSHROOM_BRIDGE, MARIO_CIRCUIT, DAISY_CRUISER,  WALUIGI_STADIUM,
    SHERBET_LAND,  MUSHROOM_CITY,  YOSHI_CIRCUIT,  DK_MOUNTAIN,
    WARIO_COLOSSEUM, DINO_DINO_JUNGLE,  BOWSER_CASTLE,RAINBOW_ROAD
};

static const char scaPasswordTable[] = {
    'G', '6', 'E', 'Q',
    'T', 'X', 'Y', 'N',
    '4', 'W', 'R', 'H',
    'B', 'F', 'K', 'O',
    'I', 'J', 'A', 'P',
    'C', 'D', '5', 'S',
    '8', 'V', '7', 'U',
    'Z', '3', 'L', 'M'
};

// WIP
u8 Password::convCrsNo(ECourseID crsID) {
    for (u8 crsIdx = 0; crsIdx < 16; crsIdx++)
    {
        if (caRaceCrsID[crsIdx] == crsID)
            return crsIdx;
    }
}