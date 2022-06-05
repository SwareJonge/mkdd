#ifndef ITEMSHUFFLEMGR_H
#define ITEMSHUFFLEMGR_H

#include <types.h>

class ItemShuffleMgr {
private:
    struct SlotTable {
        u8 chance[27];
        u8 total; // i don't know wheter or not this is used but 1 byte is not always enough to store this
    };

    class KartSlotList {
    public:
        KartSlotList();
        SlotTable * slotTable[2];
    };

    struct KartSlotData {
        u8 kartCount;
        u8 useableSlots;
        u8 totalSlots;
        KartSlotList* slotList;
    };

    static u8 sSlotNormalItemNum; // 9
    static u8 sSlotSpecialItemNum; // 9
    static u32 *sSlotKindIndexArray;
    struct KartSlotRankDataSet {
        KartSlotData* slotData;
        u32 specialItemOffset;
        u32 specialItemChance;
        s32 rank;
        s32 total;
        s32 _0x14;
        s32 _0x18;
    };
public:
    virtual ~ItemShuffleMgr();
    virtual void calcRaceUseNormalItem(u32 *, ItemShuffleMgr::KartSlotRankDataSet *, s32);
    virtual void calcSpecialItemNum(u32 *, ItemShuffleMgr::KartSlotRankDataSet *, s32, bool);
    virtual s32 calcRank(ItemShuffleMgr::KartSlotRankDataSet);
};

#endif // !ITEMSHUFFLEMGR_H
