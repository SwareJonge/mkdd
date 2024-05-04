#ifndef KARTENUM_H
#define KARTENUM_H

// TODO: consistency.

enum ECourseID
{
    TEST_1 = 0x1,
    TEST_2 = 0x2,
    TEST_3 = 0x3,
    TEST_4 = 0x4,
    TEST_5 = 0x5,
    TEST_6 = 0x6,
    TEST_7 = 0x7,
    TEST_8 = 0x8,
    TEST_9 = 0x9,
    TEST_10 = 0xa,
    TEST_11 = 0xb,
    TEST_12 = 0xc,
    TEST_13 = 0xd,
    TEST_14 = 0xe,
    TEST_15 = 0xf,
    TEST_16 = 0x10,
    TEST_17 = 0x11,
    TEST_18 = 0x12,
    TEST_19 = 0x13,
    TEST_20 = 0x14,
    TEST_21 = 0x15,
    TEST_22 = 0x16,
    TEST_23 = 0x17,
    TEST_24 = 0x18,
    TEST_25 = 0x19,
    TEST_26 = 0x1a,
    TEST_27 = 0x1b,
    COURSE_UNK0 = 0x1c,
    DEBOKOKO = 0x1d,
    A3 = 0x1e,
    E3 = 0x1f,
    COURSE_UNK1_20 = 0x20,
    BABY_PARK = 0x21,
    PEACH_BEACH = 0x22,
    DAISY_CRUISER = 0x23,
    LUIGI_CIRCUIT = 0x24,
    MARIO_CIRCUIT = 0x25,
    YOSHI_CIRCUIT = 0x26,
    COURSE_UNK2_27 = 0x27,
    MUSHROOM_BRIDGE = 0x28,
    MUSHROOM_CITY = 0x29,
    WALUIGI_STADIUM = 0x2a,
    WARIO_COLOSSEUM = 0x2b,
    DINO_DINO_JUNGLE = 0x2c,
    DK_MOUNTAIN = 0x2d,
    COURSE_UNK3_2e = 0x2e,
    BOWSER_CASTLE = 0x2f,
    COURSE_UNK4_30 = 0x30,
    RAINBOW_ROAD = 0x31,
    DRY_DRY_DESERT = 0x32,
    SHERBET_LAND = 0x33,
    MINI_1 = 0x34,
    MINI_2 = 0x35,
    MINI_3 = 0x36,
    MINI_4 = 0x37,
    MINI_5 = 0x38,
    MINI_6 = 0x39,
    MINI_7 = 0x3a,
    MINI_8 = 0x3b,
    REVERSE_1 = 0x3c,
    REVERSE_2 = 0x3d,
    REVERSE_3 = 0x3e,
    REVERSE_4 = 0x3f,
    REVERSE_5 = 0x40,
    REVERSE_6 = 0x41,
    REVERSE_7 = 0x42,
    REVERSE_8 = 0x43,
    AWARD = 0x44,
    ENDING = 0x45
};

enum ECharID
{
    cCharIDNone = 0,
    BABY_MARIO = 1,
    BABY_LUIGI = 2,
    PARATROOPA = 3,
    KOOPATROOPA = 4,
    PEACH = 5,
    DAISY = 6,
    MARIO = 7,
    LUIGI = 8,
    WARIO = 9,
    WALUIGI = 10,
    YOSHI = 11,
    BIRDO = 12,
    DONKEYKONG = 13,
    DONKEYJR = 14,
    BOWSER = 15,
    BOWSERJR = 16,
    TOAD = 17,
    TOADETTE = 18,
    KING_BOO = 19,
    PETEY = 20,
    cCharIDMax = 21
};

enum EGhostKind
{
    KIND_0,
    KIND_1,
    KIND_2,
};

enum EKartID
{
    cKartIDNone = 0xff,
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
    PARADE_KART = 20,
    cKartIDMax = 21
};

enum EKartWeight
{
    LIGHT = 0,
    NORMAL = 1,
    HEAVY = 2,
    UNK_3 // huh?
};

enum ERaceGpCup
{
    INV_CUP = -1,
    MUSHROOM_CUP = 0,
    FLOWER_CUP = 1,
    STAR_CUP = 2,
    SPECIAL_CUP = 3,
    ALL_CUP_TOUR = 4,
    CUP_MAX
};

enum ERaceLevel
{ // unsure of this
    LVL_INV = -1,
    LVL_50CC = 0,
    LVL_100CC = 1,
    LVL_150CC = 2,
    LVL_MIRROR = 3
};

enum ERaceMode
{
    INV_MODE = 0,
    TIME_ATTACK = 0x1,
    GRAND_PRIX = 0x2,
    VERSUS_RACE = 0x3,
    BALLOON_BATTLE = 0x4,
    ROBBERY_BATTLE = 0x5,
    BOMB_BATTLE = 0x6,
    ESCAPE_BATTLE = 0x7,
    AWARD_DEMO = 0x8,
    STAFF_ROLL = 0x9,
};

enum ERacePhase
{ // idk how many phases there are
    PHASE_CRS_DEMO = 1,
    PHASE_2 = 2,
    PHASE_3 = 3,
    PHASE_4 = 4,
    PHASE_5 = 5,
    PHASE_6 = 6,
    PHASE_7 = 7,
    PHASE_8 = 8,
    PHASE_9 = 9,
    PHASE_10 = 10,
    PHASE_11 = 11,
    PHASE_12 = 12,
    PHASE_13 = 13,
    PHASE_14 = 14,
    PHASE_15 = 15,
};

#endif