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
        KartSlotList* mList;
    };

    static KartSlotData mSlotList;
    static KartSlotData mSlotListEnemy;

    static u8 sSlotNormalItemNum; // 9
    static u8 sSlotSpecialItemNum; // 9
    static u32 *sSlotKindIndexArray;
    struct KartSlotRankDataSet {
        KartSlotData* data;
        u32 specialItemIndex;
        u32 specialItemChance;
        s32 rankIdx;
        s32 total;
        s32 _0x14;
    };
public:
    s32 calcSlot(KartSlotRankDataSet & slotRankData, s32 p2, s32 p3, bool p4);

    virtual ~ItemShuffleMgr();
    virtual void calcRaceUseNormalItem(u32 *, KartSlotRankDataSet *, s32);
    virtual void calcSpecialItemNum(u32 *, KartSlotRankDataSet *, s32, s32, bool);
    virtual s32 calcRank(KartSlotRankDataSet);
};

class ItemRndSpecialShuffleMgr : public ItemShuffleMgr {
public:
    virtual ~ItemRndSpecialShuffleMgr();
    virtual void calcRaceUseNormalItem(u32 *, KartSlotRankDataSet *, s32);
    virtual void calcSpecialItemNum(u32 *, KartSlotRankDataSet *, s32, s32, bool);
    virtual s32 calcRank(KartSlotRankDataSet);
};

#endif // !ITEMSHUFFLEMGR_H
