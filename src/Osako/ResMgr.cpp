#include <JSystem/JAudio/JASFakeMatch2.h>
#include <JSystem/JKernel/JKRDvdRipper.h>

#include "kartLocale.h"
#include "Kaneshige/SysDebug.h"
#include "Osako/Kart2DCommon.h"
#include "Osako/ResMgr.h"
#include "Osako/system.h"

namespace ResMgr
{
    JKRSolidHeap *mspKeepHeap;
    JKRSolidHeap *mspCourseHeap;
    JKRHeap *mspAwardHeap;
    JKRHeap *mspOpeningHeap;
    u32 msLoadFlag;
    u32 msLoadingFlag;
    bool msRequestLoadingKeepData;
    ECourseID msCourseID;
    CourseOrder msCourseOrder;
    ECourseID msMountCourseID;
    CourseOrder msMountCourseOrder;
    void *mspCourseName;
    void *mspStaffGhost;
    u8 msFreeArgArray;
    int msAramTaskNum;

    JKRArchive *mspaLoader[mcArcMax];
    AramAsyncState msaAramResArg[0x40];

    ERaceGpCup msCupID = INV_CUP;
    u8 msUsedArgArray = 0x40;

    const u32 mscKeepHeapSize = 0x780000;
    const u32 mscCourceHeapSize = 0x280000;

    char *getCrsArcName(ECourseID courseId)
    {
        switch (courseId)
        {
        case LUIGI_CIRCUIT:
            return "Luigi";
        case PEACH_BEACH:
            return "Peach";
        case BABY_PARK:
            return "BabyLuigi";
        case MUSHROOM_BRIDGE:
            return "Nokonoko";
        case DRY_DRY_DESERT:
            return "Desert";
        case DAISY_CRUISER:
            return "Daisy";
        case SHERBET_LAND:
            return "Snow";
        case MARIO_CIRCUIT:
            return "Mario";
        case MUSHROOM_CITY:
            return "Patapata";
        case WALUIGI_STADIUM:
            return "Waluigi";
        case DK_MOUNTAIN:
            return "Donkey";
        case BOWSER_CASTLE:
            return "Koopa";
        case DINO_DINO_JUNGLE:
            return "Diddy";
        case YOSHI_CIRCUIT:
            return "Yoshi";
        case WARIO_COLOSSEUM:
            return "Wario";
        case RAINBOW_ROAD:
            return "Rainbow";
        case TEST_1:
            return "Test1";
        case TEST_2:
            return "Test2";
        case TEST_3:
            return "Test3";
        case TEST_4:
            return "Test4";
        case TEST_5:
            return "Test5";
        case TEST_6:
            return "Test6";
        case TEST_7:
            return "Test7";
        case TEST_8:
            return "Test8";
        case TEST_9:
            return "Test9";
        case TEST_10:
            return "Test10";
        case TEST_11:
            return "Test11";
        case TEST_12:
            return "Test12";
        case E3:
            return "E3";
        case DEBOKOKO:
            return "Dekoboko";
        case A3:
            return "A3";
        case TEST_13:
            return "Test13";
        case TEST_14:
            return "Test14";
        case TEST_15:
            return "Test15";
        case TEST_16:
            return "Test16";
        case TEST_17:
            return "Test17";
        case TEST_18:
            return "Test18";
        case TEST_19:
            return "Test19";
        case TEST_20:
            return "Test20";
        case TEST_21:
            return "Test21";
        case TEST_22:
            return "Test22";
        case TEST_23:
            return "Test23";
        case TEST_24:
            return "Test24";
        case TEST_25:
            return "Test25";
        case TEST_26:
            return "Test26";
        case TEST_27:
            return "Test27";
        case MINI_1:
            return "Mini1";
        case MINI_2:
            return "Mini2";
        case MINI_3:
            return "Mini3";
        case MINI_4:
            return "Mini4";
        case MINI_5:
            return "Mini5";
        case MINI_6:
            return "Mini6";
        case MINI_7:
            return "Mini7";
        case MINI_8:
            return "Mini8";
        case REVERSE_1:
            return "Reverse1";
        case REVERSE_2:
            return "Reverse2";
        case REVERSE_3:
            return "Reverse3";
        case REVERSE_4:
            return "Reverse4";
        case REVERSE_5:
            return "Reverse5";
        case REVERSE_6:
            return "Reverse6";
        case REVERSE_7:
            return "Reverse7";
        case REVERSE_8:
            return "Reverse8";
        case AWARD:
            return "Award";
        case ENDING:
            return "Ending";
        default:
#line 118 // uhhh what?
            JUT_ASSERT_F(0, "Invalid Course Id:%d", courseId);
            return nullptr;
        }
    }

    char *getAwardArcName(ERaceGpCup cupId)
    {
        switch (cupId)
        {
        case MUSHROOM_CUP:
            return "Kinoko";
        case FLOWER_CUP:
            return "Flower";
        case STAR_CUP:
            return "Star";
        case SPECIAL_CUP:
            return "Special";
        case ALL_CUP_TOUR:
            return "AllTour";
        default:
#line 137
            JUT_ASSERT_F(0, "Invalid Cup Id:%d", cupId);
            return nullptr;
        }
    }

    char *getCrsDataName(CourseDataId crsDataId)
    {
        switch (crsDataId)
        {
        case mcCourseBmd:
            return "course.bmd";
        case mcCourseBco:
            return "course.bco";
        case mcCourseBol:
            return "course.bol";
        case mcCourseBtk1:
            return "course.btk";
        case mcCourseBtk2:
            return "course_02.btk";
        case mcCourseBtk3:
            return "course_03.btk";
        case mcCourseBtp:
            return "course.btp";
        case mcCourseBrk:
            return "course.brk";
        case mcCourseSkyBmd:
            return "sky.bmd";
        case mcCourseSkyBtk:
            return "sky.btk";
        case mcCourseSkyBrk:
            return "sky.brk";
        case mcCourseMapBti:
            return "map.bti";
        case mcCourseNameBti:
            return "name.bti";
        default:
#line 164
            JUT_ASSERT_F(0, "Invalid Course Data Id:%d", crsDataId);
            return nullptr;
        }
    }

    char *getKartName(EKartID kartId)
    {
        switch (kartId)
        {
        case GOO_GOO_BUGGY:
            return "baby";
        case RATTLE_BUGGY:
            return "bblui";
        case KOOPA_DASHER:
            return "noko";
        case PARA_WING:
            return "pata";
        case BARREL_TRAIN:
            return "diddy";
        case BULLET_BLASTER:
            return "jr";
        case TOAD_KART:
            return "kpio";
        case TOADETTE_KART:
            return "kpico";
        case RED_FIRE:
            return "mario";
        case GREEN_FIRE:
            return "luigi";
        case HEART_COACH:
            return "peach";
        case BLOOM_COACH:
            return "daisy";
        case TURBO_YOSHI:
            return "yoshi";
        case TURBO_BIRDO:
            return "cathy";
        case WALUIGI_RACER:
            return "walui";
        case WARIO_CAR:
            return "wario";
        case DK_JUMBO:
            return "dk";
        case KOOPA_KING:
            return "koopa";
        case PIRANHA_PIPES:
            return "pakku";
        case BOO_PIPES:
            return "teres";
        case PARADE_KART:
            return "extra";
        default:
#line 202
            JUT_ASSERT_F(0, "Invalid Kart Id:%d", kartId);
            return nullptr;
        }
    }

    char *getKartDataName(KartDataId kartDataId)
    {
        switch (kartDataId)
        {
        case mcKartDataBody:
            return "body.bmd";
        case mcKartDataArm:
            return "arm.bmd";
        case mcKartDataShock:
            return "shock.bmd";
        case mcKartTire:
            return "tire.bmd";
        case mcKartTireLeft:
            return "tire_l.bmd";
        case mcKartTireRight:
            return "tire_r.bmd";
        default:
#line 222
            JUT_ASSERT_F(0, "Invalid Kart Data Id:%d", kartDataId);
            return nullptr;
        }
    }

    char *getDriverName(DriverId driverId)
    {
        switch (driverId)
        {
        case mcDriverBabyMario:
            return "BabyMario";
        case mcDriverBabyLuigi:
            return "BabyLuigi";
        case mcDriverParaTroopa:
            return "Patapata";
        case mcDriverKoopaTroopa:
            return "Nokonoko";
        case mcDriverPeach:
            return "Peach";
        case mcDriverDaisy:
            return "Daisy";
        case mcDriverMario:
            return "Mario";
        case mcDriverLuigi:
            return "Luigi";
        case mcDriverWario:
            return "Wario";
        case mcDriverWaluigi:
            return "Waluigi";
        case mcDriverYoshi:
            return "Yoshi";
        case mcDriverBirdo:
            return "Catherine";
        case mcDriverDonkeyKong:
            return "DK";
        case mcDriverDiddyKong:
            return "DKjr";
        case mcDriverBowser:
            return "Koopa";
        case mcDriverBowserJr:
            return "KoopaJr";
        case mcDriverToad:
            return "Kinopio";
        case mcDriverToadette:
            return "Kinopico";
        case mcDriverKingBoo:
            return "KingTeresa";
        case mcDriverPetey:
            return "BossPakkun";
        default:
#line 256
            JUT_ASSERT_F(0, "Invalid Driver Id:%d", driverId);
            return nullptr;
        }
    }

    char *getDriverDataName(DriverDataId driverDataId)
    {
        switch (driverDataId)
        {
        case mcDriverDataBmd:
            return "driver.bmd";
        case mcDriverData200Bmd:
            return "driver_200.bmd";
        case mcDriverDataBrk:
            return "driver.brk";
        default:
#line 273
            JUT_ASSERT_F(0, "Invalid Driver Data Id:%d", driverDataId);
            return nullptr;
        }
    }

    char *getShadowDataName(ShadowDataId shadowDataId)
    {
        switch (shadowDataId)
        {
        case mcShadowDataBmd:
            return "bmd";
        case mcShadowDataBls:
            return "bls";
        case mcShadowDataBlk:
            return "blk";
        default:
#line 290
            JUT_ASSERT_F(0, "Invalid Shadow Data Id:%d", shadowDataId);
            return nullptr;
        }
    }

    char *getMsgDataName(MessageDataId msgDataId)
    {
        switch (msgDataId)
        {
        case mcColor:
            return "color.bmc";
        case mcDvd:
            return "dvd.bmg";
        case mcCard:
            return "card.bmg";
        case mcProgressiveScan:
            return "prog.bmg";
        case mcBattle:
            return "battle.bmg";
        case mcEuro60:
            return "euro60.bmg";
        case mcLan:
            return "lan.bmg";
        case mcIpl:
            return "ipl.bmg";
        case mcSecret:
            return "secret.bmg";
        case mcRace:
            return "race.bmg";
        case mcYesNo:
            return "yesno.bmg";
        case mcStaffRoll:
            return "staff.bmg";
        default:
#line 316
            JUT_ASSERT_F(0, "Invalid Message Data Id:%d", msgDataId);
            return nullptr;
        }
    }

    void create(JKRHeap *heap)
    {
        for (int i = 0; i < mcArcMax; i++)
        {
            mspaLoader[i] = nullptr;
        }

        for (int i = 0; i < 0x40; i++)
        {
            msaAramResArg[i]._10 = 2;
        }

        mspaLoader[mcArcSystem] = JKRMountArchive(_pSystemArc, heap, JKRArchive::MOUNT_DIRECTION_HEAD);
        mspKeepHeap = JKRSolidHeap::create(mscKeepHeapSize, heap, false);
        mspCourseHeap = JKRSolidHeap::create(mscCourceHeapSize, heap, false);
        SYSDBG_CreateHeapInfo(mspKeepHeap, "MRAM.Arc");
        SYSDBG_CreateHeapInfo(mspCourseHeap, "Crs.Arc");
    }

    void loadKeepTask(void *param)
    {
        if (FLAG_ON(msLoadFlag, 1))
        {
            msLoadingFlag |= 1;
            mspaLoader[mcArcMRAM] = JKRMountArchive("/MRAM.arc", JKRArchive::MOUNT_MEM, mspKeepHeap, JKRArchive::MOUNT_DIRECTION_HEAD);

            // clang-format off
            #line 400
            JUT_ASSERT(mspaLoader[ mcArcMRAM ]);
            // clang-format on
            msLoadingFlag &= ~1;
            msLoadFlag |= 1;
        }

        if (FLAG_ON(msLoadFlag, 2))
        {
            char path[0x40];
            snprintf(path, sizeof(path), "/MRAM_Locale/%s/MRAMLoc.arc", KartLocale::getLanguageName());
            msLoadingFlag |= 2;
            mspaLoader[mcArcMRAMLoc] = JKRMountArchive(path, JKRArchive::MOUNT_MEM, mspKeepHeap, JKRArchive::MOUNT_DIRECTION_HEAD);

            // clang-format off
            #line 412
            JUT_ASSERT(mspaLoader[ mcArcMRAMLoc ]);
            // clang-format on
            msLoadingFlag &= ~2;
            msLoadFlag |= 2;
        }

        if (FLAG_ON(msLoadFlag, 0x20))
        {
            JKRHeap *heap = mspKeepHeap; // Inline?
            msLoadingFlag |= 0x20;
            mspaLoader[mcArcRace2D] = JKRMountArchive(getPtr(mcArcMRAM, "Race2D.arc"), heap, JKRArchive::MOUNT_DIRECTION_HEAD);

// clang-format off
            #line 422
            JUT_ASSERT(mspaLoader[ mcArcRace2D ]);
            // clang-format on
            msLoadingFlag &= ~0x20;
            msLoadFlag |= 0x20;
            Kart2DCommon::create(mspKeepHeap);
        }

        if (FLAG_ON(msLoadFlag, 4))
        {
            msLoadingFlag |= 4;
            mspaLoader[mcArcARAM] = JKRMountArchive("/ARAM.arc", JKRArchive::MOUNT_ARAM, mspKeepHeap, JKRArchive::MOUNT_DIRECTION_HEAD);

// clang-format off
            #line 434
            JUT_ASSERT(mspaLoader[ mcArcARAM ]);
            // clang-format on
            msLoadingFlag &= ~4;
            msLoadFlag |= 4;
        }
    }

    void loadCourseTask(void *param)
    {
        msLoadingFlag |= 8;

        char path[0x40];
        snprintf(path, sizeof(path), "/CourseName/%s/%s_name.bti", KartLocale::getLanguageName(), getCrsArcName(msCourseID));
        mspCourseName = JKRDvdToMainRam(path, nullptr, EXPAND_SWITCH_DEFAULT, 0, mspCourseHeap, JKRDvdRipper::ALLOC_DIR_TOP, 0, nullptr, nullptr);

        snprintf(path, sizeof(path), "/StaffGhosts/%s.ght", getCrsArcName(msCourseID));
        mspStaffGhost = JKRDvdToMainRam(path, nullptr, EXPAND_SWITCH_DEFAULT, 0, mspCourseHeap, JKRDvdRipper::ALLOC_DIR_TOP, 0, nullptr, nullptr);

        if (msCourseID == LUIGI_CIRCUIT && gRaceInfo.getRaceLevel() != LVL_50CC)
        {
            snprintf(path, sizeof(path), "/Course/Luigi2%s.arc", msCourseOrder == mcOrder_1 ? "L" : "");
        }
        else
        {
            snprintf(path, sizeof(path), "/Course/%s%s.arc", getCrsArcName(msCourseID), msCourseOrder == mcOrder_1 ? "L" : "");
        }

        mspaLoader[mcArcCourse] = JKRMountArchive(path, JKRArchive::MOUNT_MEM, mspCourseHeap, JKRArchive::MOUNT_DIRECTION_HEAD);

        msMountCourseID = msCourseID;
        msMountCourseOrder = msCourseOrder;
        msLoadingFlag &= ~8;
        msLoadFlag |= 8;
    }

    void loadAwardTask(void *param)
    {
        char path[0x40];

        snprintf(path, sizeof(path), "/AwardData/%s/Award2D.arc", KartLocale::getLanguageName());
        msLoadingFlag |= 0x80;
        mspaLoader[mcArcAward2D] = JKRMountArchive(path, JKRArchive::MOUNT_MEM, mspAwardHeap, JKRArchive::MOUNT_DIRECTION_HEAD);
        msLoadingFlag &= ~0x80;
        msLoadFlag |= 0x80;

        snprintf(path, sizeof(path), "/AwardData/Award_%s.arc", getAwardArcName(msCupID));
        msLoadingFlag |= 0x40;
        mspaLoader[mcArcAward] = JKRMountArchive(path, JKRArchive::MOUNT_MEM, mspAwardHeap, JKRArchive::MOUNT_DIRECTION_HEAD);
        msLoadingFlag &= ~0x40;
        msLoadFlag |= 0x40;
    }

    void loadOpeningTask(void *param)
    {
        msLoadingFlag |= 0x100;
        mspaLoader[mcArcOpening] = JKRMountArchive("/Movie/Opening.arc", JKRArchive::MOUNT_MEM, mspOpeningHeap, JKRArchive::MOUNT_DIRECTION_HEAD);
        msLoadingFlag &= ~0x100;
        msLoadFlag |= 0x100;
    }

    void loadKeepData()
    {
        if (msRequestLoadingKeepData)
            return;
        if (!SYSTEM_RequestTask(loadKeepTask, nullptr, nullptr))
        {
#line 514
            JUT_PANIC("task full");
        }
        msRequestLoadingKeepData = true;
    }

    void loadCourseData(ECourseID courseID, CourseOrder courseOrder)
    {
        msCourseID = courseID;
        msCourseOrder = courseOrder;

        // Bug: compares courseOrder with msMountCourseID instead of msMountCourseOrder
        if (msMountCourseID == courseID && msMountCourseID == courseOrder && msMountCourseID != LUIGI_CIRCUIT)
            return;

        mspCourseHeap->freeAll();
        msLoadFlag &= ~8;
        mspaLoader[mcArcCourse] = nullptr;

        if (!SYSTEM_RequestTask(loadCourseTask, nullptr, nullptr))
        {
#line 536
            JUT_PANIC("task full");
        }
    }

    void loadAwardData(ERaceGpCup cupId, JKRHeap *heap)
    {
        msLoadFlag &= ~0x80;
        mspaLoader[mcArcAward2D] = nullptr;

        msLoadFlag &= ~0x40;
        mspaLoader[mcArcAward] = nullptr;

        msCupID = cupId;
        mspAwardHeap = heap;

        if (!SYSTEM_RequestTask(loadAwardTask, nullptr, nullptr))
        {
#line 555
            JUT_PANIC("task full");
        }
    }

    void loadOpeningData(JKRHeap *heap)
    {
        msLoadFlag &= ~0x100;
        mspaLoader[mcArcOpening] = nullptr;
        mspOpeningHeap = heap;
        if (!SYSTEM_RequestTask(loadOpeningTask, nullptr, nullptr))
        {
#line 570
            JUT_PANIC("task full");
        }
    }

    bool isFinishLoadingArc(ArchiveId id)
    {
        return (msLoadFlag & 1 << id) != 0;
    }

    bool isLoadingArc(ArchiveId id)
    {
        return (msLoadingFlag & 1 << id) != 0;
    }

    void *getPtr(KartDataId kartDataId, EKartID kartId)
    {
        char path[0x80];
        snprintf(path, sizeof(path), "Kart/%s_all/%s_k_%s", getKartName(kartId), getKartName(kartId), getKartDataName(kartDataId));
        return mspaLoader[mcArcMRAM]->getResource(path);
    }

    void *getPtr(DriverDataId driverDataId, DriverId driverId)
    {
        char path[0x40];
        snprintf(path, sizeof(path), "Driver/%s/%s", getDriverName(driverId), getDriverDataName(driverDataId));
        return mspaLoader[mcArcMRAM]->getResource(path);
    }

    void *getPtr(ShadowDataId shadowDataId, EKartID kartId)
    {
        if (shadowDataId == mcShadowDataBmd)
        {
            kartId = RED_FIRE;
        }
        else
        {
            // might be inline
            switch (kartId)
            {
            case GREEN_FIRE:
                kartId = RED_FIRE;
                break;
            case PARA_WING:
                kartId = KOOPA_DASHER;
                break;
            case BLOOM_COACH:
                kartId = HEART_COACH;
                break;
            case RATTLE_BUGGY:
                kartId = GOO_GOO_BUGGY;
                break;
            case TURBO_BIRDO:
                kartId = TURBO_YOSHI;
                break;
            case TOADETTE_KART:
                kartId = TOAD_KART;
                break;
            case BOO_PIPES:
                kartId = PIRANHA_PIPES;
                break;
            }
        }

        char path[0x40];
        snprintf(path, sizeof(path), "Shadow/shadow_%s.%s", getKartName(kartId), getShadowDataName(shadowDataId));
        return mspaLoader[mcArcMRAM]->getResource(path);
    }

    void *getPtr(CourseDataId crsDataId)
    {
        switch (crsDataId)
        {
        case mcCourseNameBti:
            return mspCourseName;
        case mcCourseDataGhost:
            return mspStaffGhost;
        }

        char path[0x40];
        snprintf(path, sizeof(path), "%s_%s", getCrsArcName(msCourseID), getCrsDataName(crsDataId));
        return mspaLoader[mcArcCourse]->getResource(path);
    }

    void *getPtr(MessageDataId msgDataId)
    {
        char path[0x40];
        snprintf(path, sizeof(path), "Message/%s/%s", KartLocale::getLanguageName(), getMsgDataName(msgDataId));
        return mspaLoader[mcArcSystem]->getResource(path);
    }

    void *getPtr(ArchiveId id, const char *path)
    {
#line 698
        JUT_MINMAX_ASSERT(mcArcMRAM, id, mcArcMax);
        return mspaLoader[id]->getResource(path);
    }

    long getResSize(ArchiveId id, const void *buf)
    {
#line 709
        JUT_MINMAX_ASSERT(mcArcMRAM, id, mcArcMax);
        return mspaLoader[id]->getResSize(buf);
    }

    bool readResourceAsync(void *p1, u32 p2, char *folder, char *fileName, AramAsyncState *state)
    {
        if (msFreeArgArray == msUsedArgArray)
            return false;

        state->_0 = nullptr;
        msaAramResArg[msFreeArgArray].setAll(p1, p2, folder, fileName, 0, state);
        state = &msaAramResArg[msFreeArgArray];

        if (System::getAramTask()->request(readAnmResTask, state, nullptr))
        {
            msAramTaskNum++;
            msFreeArgArray++;

            if (msFreeArgArray >= 0x40)
                msFreeArgArray = 0;
            return true;
        }

#line 737
        JUT_PANIC("task full");
        return false;
    }

    void readAnmResTask(void *param)
    {
        AramAsyncState *state = (AramAsyncState *)param;
        char path[0x40];
        snprintf(path, sizeof(path), "DriverAnm/%s/%s", state->folder, state->fileName);

        state->parent->_0 = (void *)1; // ?
        state->_10 = 1;
        mspaLoader[mcArcARAM]->readResource(state->_0, state->_4, path);

        state->parent->_0 = (void *)2; // ?
        state->_10 = 2;

        msAramTaskNum--;
        msUsedArgArray++;

        if (msUsedArgArray >= 0x40)
            msUsedArgArray = 0;
    }

    bool findResource(ArchiveId id, char *path)
    {
#line 768
        JUT_MINMAX_ASSERT(mcArcMRAM, id, mcArcMax)

        if (mspaLoader[id]->findFsResource(path, 0))
            return true;

        return false;
    }

    bool isFinishLoadingAram()
    {
        for (int i = 0; i < ARRAY_SIZE(msaAramResArg); i++)
        {
            if (msaAramResArg[i]._10 != 2)
                return false;
        }
        return true;
    }
}
