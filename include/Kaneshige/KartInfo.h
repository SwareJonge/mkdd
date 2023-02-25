
#ifndef KARTINFO_H
#define KARTINFO_H


#include <dolphin/os.h>
#include "JSystem/JUtility/JUTDbg.h"
#include "Osako/kartPad.h"
#include "types.h"

extern "C" {
#include <ppcdis.h>
}

enum EGhostKind {
    KIND_0,
    KIND_1,
    KIND_2,
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
    LIGHT = 0,
    NORMAL = 1,
    HEAVY = 2
};

class KartInfo {
public:
    struct SCharDB
    {
        u16 id;
        u16 defaultPartnerID;
        u16 weight;
        u16 defaultKartID;
        u8 _8;
        u8 _9;
        //u8 _9[3]; // this is padding data most likely
    };

    struct SKartDB
    {
        EKartID id;
        EKartWeight weight;
        u16 wheelCount;
        s8 _a;
        u16 defaultDriverID;
        u16 _e;
    };

    class KartCharacter {
    public:
        KartCharacter() {
            reset();
        }
        ~KartCharacter() {};
        void reset()    {
            mKartGamePad = 0;
            mCharDB = 0;
        }

        bool isComPlayer() const {
            return mKartGamePad == nullptr;
        }

        KartGamePad *getPad() { return mKartGamePad; }
        void setPad(KartGamePad * gamepad);

        void setCharDB(const SCharDB  * charDB) {
            mCharDB = charDB;
        }
        ECharID getCharID() const {
            if (mCharDB != 0)
                return (ECharID)mCharDB->id;
            return cCharIDNone;
        }
        ECharID getPartnerID() const {
            if (mCharDB != 0)
                return (ECharID)mCharDB->defaultPartnerID;
            return cCharIDNone;
        }
        bool isAvailable() const;
        static s32 convPlayerKind(KartGamePad *);
        
        s32 getPlayerKind() const {
            return convPlayerKind(mKartGamePad);
        }

    private:
        KartGamePad* mKartGamePad; // inherited from JUTGamePad
        const SCharDB* mCharDB;
    };
    KartInfo();
    ~KartInfo();

    void reset();

    static const SCharDB * getCharDB(ECharID charID);
    void setDriver(int driverNo, ECharID charID, KartGamePad * gamePad);
    static const SKartDB * getKartDB(EKartID kartID);
    static EKartWeight getKartWeight(EKartID);
    static ECharID getDefaultDriver(EKartID);
    static ECharID getDefaultPartner(ECharID);
    static s32 getDriverWeight(ECharID);
    static EKartID getDefaultKartID(ECharID);
    static EKartID getPartnerKartID(ECharID);
    bool isDefaultCharCombi();
    KartGamePad * getYoungestPad();

    bool isComDriver(int driverNo) const {
        JUT_MINMAX_ASSERT(113, 0, driverNo, 2);
        return mKartCharacters[driverNo].isComPlayer();
    }

    // Enum?
    s32 getPlayerKind(int driverNo) const { 
        JUT_MINMAX_ASSERT(119, 0, driverNo, 2);
        return mKartCharacters[driverNo].getPlayerKind();
    }

    KartGamePad* getPad(int driverNo) {
        JUT_MINMAX_ASSERT(126, 0, driverNo, 2);
        return mKartCharacters[driverNo].getPad();
    }

    bool isComKart() const {
        return isComDriver(0);
    }

    void setKartID(EKartID kartID) {
        mKartDB = getKartDB(kartID);
    }

    bool isRealPlayerKart() const { return getPlayerKind(0) == 1; }
    bool isGhostKart() const { return getPlayerKind(0) == 4; }
    bool isPlayerKart() const { return !isComKart(); }

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
    const SKartDB * mKartDB;
    KartCharacter mKartCharacters[2]; // one for the driver, other for the one doing nothing
    EGhostKind mGhostKind;            // if this is set to 1 this means the driver is a ghost, 2 is also used for ghost but for the pad that gets recorded, so that means 2 is invisible?
};

#endif // !KARTINFO_H
