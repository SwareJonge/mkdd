#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/KartInfo.h"
#include "Kameda/SceneCourseSelect.h"
#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/RaceTime.h"
#include "Osako/GPRecord.h"
#include "Osako/SystemRecord.h"
#include "Osako/kartPad.h"
#include "dolphin/os/OSTime.h"
#include "kartEnums.h"
#include "mathHelper.h"

SequenceInfo gSequenceInfo;

#include "JSystem/JAudio/JASFakeMatch2.h"


const int SequenceInfo::RANKPOINT[8] = {
    10, 8, 6, 4,
     3, 2, 1, 0
};

SequenceInfo::SequenceInfo() : mRnd(0) {
    for (int i = 0; i < 4; i++) {
        _80[i] = 0;
        for (int j = 0; j < 4; j++) {
            _40[i][j] = 0;
        }
    }
    init();
}

void SequenceInfo::init() {
    _28 = 0;
    _2c = 0;
    _30 = 0;

    for (int i = 0; i < 8; i++) {
        mStartNo[i] = i;
    }

    mDecidePad = nullptr;
    mMapInfo.cup = 0;
    mMapInfo.course = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
            _98[i][j] = 0;
        }
        mRankPoint[i] = 0;
        _2b8[i] = 0;
        _2d8[i] = 0;
        _2f8[i] = 0;
    }

    _350 = 0;
    _354 = 0;
    mLevel = LVL_INV;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            _318[i]._0[j] = cCharIDNone;
        }
        _318[i]._8 = cKartIDNone;
        _318[i].mTime.zero();
        _318[i]._14 = 0;
        _318[i]._18 = 0;
    }

    _35c = 0;
    _35d = 0;
    _35e = 0;
    _35f = 0;
    _360 = 0;
    _364 = 0;
    mGhostFlags = 1;
    mGameFlag = 0;
    mSecretKart = 0;
    rndDemo(0);
    setRandomSeed();
    rndAllCupCourse();
    SceneCourseSelect::mCup = 0;
    SceneCourseSelect::mCourse = 0;

}

void SequenceInfo::setRandomSeed() { mRnd.setSeed(OSGetTime()); }

u32 SequenceInfo::getRandom() { return mRnd.get(); }

KartGamePad *SequenceInfo::getDecidePad() {
    if (mDecidePad) {
        return mDecidePad;
    }
    return gpaGamePad[0];
}

void SequenceInfo::setGPRank(int kart, int rankIdx) {
#line 168
    JUT_MINMAX_ASSERT(0, kart, 8);
    JUT_MINMAX_ASSERT(0, mMapInfo.course, 16);
    _98[kart][mMapInfo.course] = rankIdx;
    if (rankIdx >= 0 && rankIdx < 8) {
        mRankPoint[kart] = mRankPoint[kart] + RANKPOINT[rankIdx];
    }
}

void SequenceInfo::setClrGPCourse() {
    for (int i = 0; i < 8; i++) {
        _2f8[i] = _2d8[i];
    }

    for (int i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        KartChecker *kartChecker = RCMGetManager()->getKartChecker(i);
        int rank = kartChecker->getRank();
        if (rank < 1 || rank > 8) continue;
        gSequenceInfo.setStartNo(i, rank-1);
        gSequenceInfo.setGPRank(i, rank-1);
        mRankPoint[rank + 7] = i;

        if (i >= gRaceInfo.getPlayerKartNumber()) continue;

        RaceTime time = get_318_RaceTime(i);
        time.add(kartChecker->getTotalTime());
        set_318_RaceTime(i, time);
    }

    int pointTable[8];
    for (int i = 0; i < 8; i++) {
        pointTable[i] =  (8 - i) + mRankPoint[i] * 100;
    }

    if (gRaceInfo.getPlayerKartNumber() == 2) {
        RaceTime player1Time = get_318_RaceTime(0);
        RaceTime player2Time = get_318_RaceTime(1);
        if (get_318_RaceTime(0).isLittle(player2Time)) { // blehhh ugly match
            pointTable[0] += 10;
        }
        else if (player1Time.isBig(player2Time)) {
            pointTable[1] += 10;
        }
    }

    int indexTable[8];
    for (int i = 0; i < 8; i++) {
        indexTable[i] = i;
    }

    for (int i = 0; i < RCMGetManager()->getKartNumber() - 1; i++) {
        for (int j = RCMGetManager()->getKartNumber() - 1; j > i; j--) {            
            if (pointTable[indexTable[i]] < pointTable[indexTable[j]]) {
                int old = indexTable[i];
                indexTable[i] = indexTable[j];
                indexTable[j] = old;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        _2d8[i] = indexTable[i];
    }

    for (int i = 0; i < gRaceInfo.getPlayerKartNumber(); i++) {
        for (int j = 0; j < 8; j++) {
            if (_2d8[j] == i) {
                set_318_14(i, j);
            }
        }
    }
    mMapInfo.course++;
}

bool SequenceInfo::isEndGP() {
    if (mMapInfo.cup == ALL_CUP_TOUR) {
        return mMapInfo.course >= 16;
    }
    return mMapInfo.course >= 4;
}

bool SequenceInfo::isGameClr() {
    int stack[8];
    for (int i = 0; i < 8; i++) {
        stack[i] = mRankPoint[i];
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < (7 - i); j++) {
            if (stack[j] > stack[j+1]) {
                int old = stack[j];
                stack[j] = stack[j+1];
                stack[j+1] = old;
            }
        }
    }

    int compVal = stack[5];
    bool ret = false;
    for(int i = 0; i < gRaceInfo.getPlayerKartNumber(); i++) {
#line 297
        JUT_MINMAX_ASSERT(0, i, 8);
        if (mRankPoint[i] >= compVal) {
            ret = true;
            break;
        }
    }
    return ret;
}

int SequenceInfo::getPlayerRankGP() {
    int rankIdx = 7;
    switch (gRaceInfo.getRaceMode()) {
    case GRAND_PRIX: {
        switch (gRaceInfo.getPlayerKartNumber()) {
        case 1: {
            for (int i = 0; i < 8; i++) {
                if (_2d8[i] == 0) {
                    rankIdx = i;
                    break;
                }
            }
            break;
        }
        case 2: {
            for (int i = 0; i < 8; i++) {
                if (_2d8[i] == 0 || _2d8[i] == 1) {
                    rankIdx = i;
                    break;
                }
            }
            break;
        }
        default: {
#line 336
            JUT_ASSERT(0);
            break;
        }
        }
        break;
    }
    default: {
#line 341
        JUT_ASSERT(0);
        break;
    }
    }
    return rankIdx;
}

int SequenceInfo::getPad2Player(KartGamePad *pad) {
    if (pad == nullptr) {
        return -1;
    }

    for (int i = 0; i < 8; i++) {
        if (gpaKartPad[i] == pad)
            return i;
    }

    return -1;
}

void SequenceInfo::setSecretFlag() {
    ERaceGpCup cup = SceneCourseSelect::mRaceGpCup[mMapInfo.cup];
    ERaceLevel level = mLevel;
    int rankIdx = getPlayerRankGP();

    #define unlockKart(kart, unlockCup, unlockLevel, unlockRank) \
        if (!(gSystemRecord.mSecretKart & (1 << kart)) && (cup == unlockCup) && (level == unlockLevel) && (rankIdx == unlockRank)) { \
            setSecretKartAppear(kart); \
            _35c = 1; \
            _35d = 1; \
        }

    #define unlockGame(gameID, unlockCup, unlockLevel, unlockRank) \
        if (!(gSystemRecord.mGameFlag & (1 << gameID)) && (cup == unlockCup) && (level == unlockLevel) && (rankIdx == unlockRank)) { \
            setSecretGameAppear(gameID); \
            _35c = 1; \
            _35d = 1; \
        } 

    unlockGame(SystemRecord::KinopioCombi, SPECIAL_CUP, LVL_100CC, 0)
    unlockGame(SystemRecord::BossCombi, STAR_CUP, LVL_MIRROR, 0)
    unlockKart(SystemRecord::Luigi, MUSHROOM_CUP, LVL_50CC, 0)
    unlockKart(SystemRecord::BabyLuigi, MUSHROOM_CUP, LVL_100CC, 0)
    unlockKart(SystemRecord::Daisy, FLOWER_CUP, LVL_50CC, 0)
    unlockKart(SystemRecord::Waluigi, FLOWER_CUP, LVL_100CC, 0)
    unlockKart(SystemRecord::Catherine, FLOWER_CUP, LVL_150CC, 0)
    unlockKart(SystemRecord::Patapata, STAR_CUP, LVL_50CC, 0)
    unlockKart(SystemRecord::Diddy, STAR_CUP, LVL_150CC, 0)
    unlockKart(SystemRecord::KoopaJr, SPECIAL_CUP, LVL_50CC, 0)
    unlockKart(SystemRecord::Kinopio, SPECIAL_CUP, LVL_100CC, 0)
    unlockKart(SystemRecord::Kinopico, MUSHROOM_CUP, LVL_MIRROR, 0)
    unlockKart(SystemRecord::Pakkun, STAR_CUP, LVL_MIRROR, 0)
    unlockKart(SystemRecord::Teressa, SPECIAL_CUP, LVL_MIRROR, 0)
    unlockKart(SystemRecord::Extra, ALL_CUP_TOUR, LVL_MIRROR, 0)
    unlockGame(SystemRecord::Mirror, ALL_CUP_TOUR, LVL_150CC, 0)
    unlockGame(SystemRecord::SpecialCup, STAR_CUP, LVL_100CC, 0)
    unlockGame(SystemRecord::AllCupTour, SPECIAL_CUP, LVL_150CC, 0)
    
    if (!(gSystemRecord.mGameFlag & (1 << SystemRecord::Ending)) && (cup == 3) && (rankIdx >= 0) && (rankIdx < 3)) {
        setSecretGameAppear(SystemRecord::Ending);
        _35c = 1;
    }

    if (!(gSystemRecord.mGameFlag & (1 << SystemRecord::SpecialEnding))) {
        bool playSpecialEnding = true; // this might be inline?
        for (int i = 0; i < CUP_MAX; i++) {
            for (int j = 0; j < LVL_MAX; j++) {
                if (i == cup && j == level) {
                    if (rankIdx == 0) {                        
                        continue;
                    }
                    playSpecialEnding = false;
                }
                GPRecord record = gSystemRecord.getGPRecord((ERaceGpCup)i, (ERaceLevel)j);
                if (record.isValid()) {
                    if (record.mRank != 0) {
                        playSpecialEnding = false;
                    }
                }
                else {
                    playSpecialEnding = false;
                }                
            }
        }
        if (playSpecialEnding) {
            setSecretGameAppear(SystemRecord::SpecialEnding);
            _35c = 1;
        }
    }

    unlockGame(SystemRecord::MiniLuigi, MUSHROOM_CUP, LVL_150CC, 0)
    unlockGame(SystemRecord::MiniMario, FLOWER_CUP, LVL_MIRROR, 0)
}

void SequenceInfo::setSecretGameAppear(SystemRecord::GameFlag flag) {
    mGameFlag |= 1 << flag;
    gSystemRecord.mGameFlag |= 1 << flag;
}

void SequenceInfo::setSecretKartAppear(SystemRecord::SecretKartID secretKart) {
    mSecretKart |= 1 << secretKart;
    gSystemRecord.mSecretKart |= 1 << secretKart;
}

void SequenceInfo::rndDemo(u32 seed) {
    JMARandFast rnd(0);
    int randChars[16];
    rnd.setSeed(seed);

    for (int i = 0; i < 16; i++) {
        randChars[i] = i;
    }
    
    for (int i = 0; i < 16; i++) {
        u32 rand = rnd.get();
        int randIdx = i + (rand % (16 - i));
        int old = randChars[i];
        randChars[i] = randChars[randIdx];
        randChars[randIdx] = old;
    }

    for (int i = 0; i < 8; i++) {
        mDemoKart[i].char1 = (ECharID)(randChars[i * 2] + 1);
        mDemoKart[i].char2 = (ECharID)(randChars[(i * 2) + 1] + 1);
    }

    for(int i = 0; i < 8; i++) {
        EKartID kartID1 = getDemoKart(mDemoKart[i].char1);
        EKartID kartID2 = getDemoKart(mDemoKart[i].char2);
        if (KartInfo::getKartWeight(kartID1) >= KartInfo::getKartWeight(kartID2)) {
            mDemoKart[i].kart = kartID1;
        }
        else {
            mDemoKart[i].kart = kartID2;
        }
    }
}

EKartID SequenceInfo::getDemoKart(ECharID charID) {
    EKartID demoKart = cKartIDNone;
    switch (charID) {
    case BABY_MARIO:
    case BABY_LUIGI:
    case DIDDY_KONG: {
        demoKart = GOO_GOO_BUGGY;
        break;
    }
    case PARATROOPA:
    case KOOPATROOPA:
    case BOWSER_JR: {
        demoKart = KOOPA_DASHER;
        break;
    }
    case PEACH:
    case DAISY: {
        demoKart = HEART_COACH;
        break;
    }
    case MARIO:
    case LUIGI: 
    case WALUIGI: {
        demoKart = RED_FIRE;
        break;   
    }
    case WARIO: {
        demoKart = WARIO_CAR;
        break;
    }
    case YOSHI:
    case BIRDO: {
        demoKart = TURBO_YOSHI;
        break;
    }
    case DONKEY_KONG: {
        demoKart = DK_JUMBO;
        break;        
    }
    case BOWSER: {
        demoKart = KOOPA_KING;
        break;
    }
    default:
#line 740
        JUT_ASSERT(0);
        break;
    }
    return demoKart;
}

void SequenceInfo::rndAllCupCourse() {
    for (int i = 0; i < 16; i++) {
        _3cc[i] = i;
    }

    for (int i = 1; i < 15; i++) {
        u32 rand = mRnd.get();
        int randIdx = i + (rand % (15 - i));
        int old = _3cc[i];
        _3cc[i] = _3cc[randIdx];
        _3cc[randIdx] = old;
    }
}
