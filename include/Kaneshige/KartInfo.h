
#ifndef KARTINFO_H
#define KARTINFO_H

#include "types.h"
#include "Dolphin/OS.h"
#include "JSystem/JUtility/JUTAssert.h"

extern "C" {
#include <ppcdis.h>
}

class KartGamePad; // usee placeholder for now so i don't have to edit symbols

enum ECharID {
    NONE = 0,
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
    PETEY = 20
};

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

enum EKartWeight {

};

class KartInfo {
public:
    struct SCharDB
    {
        s16 id;
        s16 defaultPartnerID;
        s16 weight;
        s16 defaultKartID;
        u8 _8;
        u8 _9;
        //u8 _9[3]; // this is padding data most likely
    };

    struct SKartDB
    {
        EKartID id;
        s32 weight; // use enum
        s16 wheelCount;
        s8 _a;
        s16 defaultDriverID;
        s16 _e;
    };

    class KartCharacter {
    public:
        KartCharacter() {
            reset();
        }
        ~KartCharacter() {};
        void reset() {
            kartGamePad = 0;
            charDB = 0;
        }
        void setPad(KartGamePad * gamepad);
        void setCharDB(const SCharDB  * sCharDB) {
            charDB = sCharDB;
        }
        ECharID getCharID() const;
        ECharID getPartnerID() const;
        bool isAvailable() const;
        s32 convPlayerKind() const;

        KartGamePad* kartGamePad; // inherited from JUTGamePad
        const SCharDB* charDB;
    };
    KartInfo();
    ~KartInfo();

    void reset();

    static const SCharDB * getCharDB(ECharID charID);
    void setDriver(int driverNo, ECharID charID, KartGamePad * gamePad);
    static const SKartDB * getKartDB(EKartID charID);
    s32 getKartWeight(EKartID);
    ECharID getDefaultDriver(EKartID);
    ECharID getDefaultPartner(ECharID);
    s32 getDriverWeight(ECharID);
    EKartID getDefaultKartID(ECharID);
    ECharID getPartnerKartID(ECharID);
    bool isDefaultCharCombi();
    KartGamePad * getYoungestPad();
    bool isRealPlayerKart();
    s32 getPlayerKind();
    bool isGhostKart() const;
    bool isPlayerKart() const;

    static const SCharDB cBabyMarioCharDB;
    static const SCharDB cBabyLuigiCharDB;
    static const SCharDB cPatapataCharDB;
    static const SCharDB cNokonokoCharDB;
    static const SCharDB cPeachCharDB;
    static const SCharDB cDaisyCharDB;
    static const SCharDB cMarioCharDB;
    static const SCharDB cLuigiCharDB;
    static const SCharDB cWarioCharDB;
    static const SCharDB cWaluigiCharDB;
    static const SCharDB cDonkeyCharDB;
    static const SCharDB cDonkeyJrCharDB;
    static const SCharDB cYoshiCharDB;
    static const SCharDB cCatherineCharDB;
    static const SCharDB cKoopaCharDB;
    static const SCharDB cKoopaJrCharDB;
    static const SCharDB cKinopioCharDB;
    static const SCharDB cKinopicoCharDB;
    static const SCharDB cKingTeresaCharDB;
    static const SCharDB cBossPakkunCharDB;
    static const SKartDB cMustacheKartDB;
    static const SKartDB cMonkeyKartDB;
    static const SKartDB cLizardKartDB;
    static const SKartDB cSmallFishKartDB;
    static const SKartDB cPrincessKartDB;
    static const SKartDB cChildKartDB;
    static const SKartDB cBadKartDB;
    static const SKartDB cSatanKartDB;
    static const SKartDB cLuigiKartDB;
    static const SKartDB cDiddyKartDB;
    static const SKartDB cCatherineKartDB;
    static const SKartDB cPatapataKartDB;
    static const SKartDB cDaisyKartDB;
    static const SKartDB cBabyLuigiKartDB;
    static const SKartDB cWaluigiKartDB;
    static const SKartDB cKoopaJrKartDB;
    static const SKartDB cKinopioKartDB;
    static const SKartDB cKinopicoKartDB;
    static const SKartDB cKingTeresaKartDB;
    static const SKartDB cBossPakkunKartDB;
    static const SKartDB cBonusKartDB;

private:
    SKartDB * kartDB;
    KartCharacter kartCharacter[2]; // one for the driver, other for the one doing nothing
    s32 kartType; // if this is set to 1 this means the driver is a ghost, 2 is also used for ghost but for the pad that gets recorded, so that means 2 is invisible?
};

char lbl_80377338[] : 0x80377338;
//void order_strings_80377338();

#endif // !KARTINFO_H
