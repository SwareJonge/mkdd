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
    'Z', '3', 'L', 'M',
    '\0'
};

u8 Password::convCrsNo(ECourseID crsID)
{
    u8 crsNo = 0;
    for (; crsNo < 16; crsNo++)
    {
        if (caRaceCrsID[crsNo] == crsID)
            break;
    }
    return crsNo;
}

u8 Password::calcCheckData()
{
    u32 sum = 0;
    sum += mPWTable.bits.back;
    sum += mPWTable.bits.region;
    sum += mPWTable.bits.hasChecksum;
    sum += mPWTable.bits.totalTime;
    sum += mPWTable.bits.kart;
    sum += mPWTable.bits.driver;
    sum += mPWTable.bits.course;
    sum += mPWTable.bits.flapTime;

    u32 checkData = 0; // add up the 4 bytes of the checksum
    checkData += (u8)(sum >> 24);
    checkData += (u8)(sum >> 16);
    checkData += (u8)(sum >> 8);
    checkData += (u8)(sum);

    return checkData;
}

void Password::setCheckData()
{
    mPWTable.bits.hasChecksum = 1;
    mPWTable.bits.checkData = calcCheckData();
}

bool Password::pack(EKartID kartId, ECharID driverId, ECharID backId, ECourseID crsID, RaceTime total, RaceTime flap)
{
    if (kartId < cKartIDMax)
        mPWTable.bits.kart = kartId;
    else
        return false;
    
    if (driverId < cCharIDMax)
        mPWTable.bits.driver = driverId;
    else
        return false;

    if (backId < cCharIDMax)
        mPWTable.bits.back = backId;
    else
        return false;

    u8 raceNo = convCrsNo(crsID);
    if (raceNo < 16)
        mPWTable.bits.course = raceNo;
    else
        return false;

    u32 totalTime = total.get();
    if (totalTime < 524288) // check if totaltime is less than 19 bits/faster than 8:44.288
        mPWTable.bits.totalTime = totalTime;
    else
        return false;

    u32 flapTime = flap.get();
    if (flapTime < 262144) // check if fastest lap time is less than 18 bits/faster than 4:22.144
        mPWTable.bits.flapTime = flapTime;
    else
        return false;

    mPWTable.bits.region = KartLocale::getRegion();
    setCheckData();

    return true;
}

bool Password::unpack(EKartID *, ECharID *, ECharID *, ECourseID *, RaceTime *, RaceTime *) {

}

void Password::crypt(u16 rnd)
{
    int hash = rnd;
    for (u32 i = 0; i < 8; i++)
    {
        mPWTable.packed[i] ^= (u8)((u32)hash >> 8);
        hash = (hash * 0x40d583d + 1) % 100000000;
    }
}

void Password::encode()
{
    // more unions i don't know of?
    mPass[0] = scaPasswordTable[(mPWTable.packed[0] >> 3) & 31];
    mPass[1] = scaPasswordTable[((mPWTable.packed[0] << 2) | (mPWTable.packed[1] >> 6)) & 31];
    mPass[2] = scaPasswordTable[(mPWTable.packed[1] >> 1) & 31];
    mPass[3] = scaPasswordTable[((mPWTable.packed[1] << 4) | (mPWTable.packed[2] >> 4)) & 31];
    mPass[4] = scaPasswordTable[((mPWTable.packed[2] << 1) | (mPWTable.packed[3] >> 7)) & 31];
    mPass[5] = scaPasswordTable[(mPWTable.packed[3] >> 2) & 31];
    mPass[6] = scaPasswordTable[((mPWTable.packed[3] << 3) | ((mPWTable.packed[4] >> 5))) & 31];
    mPass[7] = scaPasswordTable[(u8)mPWTable.packed[4] & 31];

    mPass[8] = scaPasswordTable[(mPWTable.packed[5] >> 3) & 31];
    mPass[9] = scaPasswordTable[((mPWTable.packed[5] << 2) | (mPWTable.packed[6] >> 6)) & 31];
    mPass[10] = scaPasswordTable[(mPWTable.packed[6] >> 1) & 31];
    mPass[11] = scaPasswordTable[((mPWTable.packed[6] << 4) | (mPWTable.packed[7] >> 4)) & 31];
    mPass[12] = scaPasswordTable[((mPWTable.packed[7] << 1) | (mPWTable.packed[8] >> 7)) & 31];
    mPass[13] = scaPasswordTable[(mPWTable.packed[8] >> 2) & 31];
    mPass[14] = scaPasswordTable[((mPWTable.packed[8] << 3) | (mPWTable.packed[9] >> 5)) & 31];
    mPass[15] = scaPasswordTable[(mPWTable.packed[9]) & 31];
    mPass[16] = '\0'; // terminator
}

char Password::searchPasswordTable(char c) {
    //return scaPasswordTable[c];
}

void Password::decode() {

}

// 802195f0 debug
// 801e76a8 PAL
bool Password::make(EKartID kartId, ECharID driverId, ECharID backId, ECourseID crsID, RaceTime total, RaceTime flap)
{
    if (!mMake)
    {
        if (pack(kartId, driverId, backId, crsID, total, flap))
        {
            mPWTable.bits.rnd = OSTicksToMilliseconds(OSGetTime());
            crypt(mPWTable.bits.rnd);
            encode();
            
            mMake = true;
            return true;
        }
    }
    return false;
}