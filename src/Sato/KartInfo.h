#ifndef KARTINFO_H
#define KARTINFO_H

#include <types.h>

enum EKartID {
    RED_FIRE = 0,
    DK_JUMBO = 1,
    TURBO_YOSHI = 2,
    KOOPA_DASHER = 3,
    HEART_COACH = 4,
    GOO_GOO_BUGGY = 5,
    WARIO_CAR = 6,
    KOOPA_KING = 7,
    GREEN_FIRE = 8,
    BARREL_TRAIN = 9,
    TURBO_BIRDO = 10,
    PARA_WING = 11,
    BLOOM_COACH = 12,
    RATTLE_BUGGY = 13,
    WALUIGI_RACER = 14,
    BULLET_BLASTER = 15,
    TOAD_KART = 16,
    TOADETTE_KART = 17,
    BOO_PIPES = 18,
    PIRANHA_PIPES = 19,
    PARADE_KART = 20
};

class KartInfo {
public:
    KartInfo();
    ~KartInfo();
    struct SCharDB
    {
        s16 id;
        s16 defaultPartnerID;
        s16 weight;
        s16 defaultKartID;
        u8 _8;
        u8 _9[3]; // this is padding data most likely
    };

    struct SKartDB
    {
        EKartID id;
        s32 weight;
        s16 _08; // not sure if this is a short or 2 bytes
        s8 _09;
        s8 _0A; // probably padding
        s16 defaultDriverID;
        s8 _0E[2]; // padding bytes most likely
    };

    class KartCharacter {
    public:
        KartCharacter();
        ~KartCharacter();
    private:
        void* kartGamePad; // inherited from JUTGamePad
        SCharDB* charDB;
    };

private:
    SKartDB kartDB;
    KartCharacter kartCharacter[2]; // one for the driver, other for the one doing nothing
    s32 kartType; // if this is set to 1 this means the driver is a ghost, 2 is also used for ghost but for the pad that gets recorded, so that means 2 is invisible?

};

#endif // !KARTINFO_H
