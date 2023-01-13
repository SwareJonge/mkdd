#ifndef ITEMOBJMGR_H
#define ITEMOBJMGR_H

#include "types.h"

class ItemObjMgr {
public:
    ItemObjMgr();
    virtual ~ItemObjMgr();
    virtual void draw();
    virtual void callbackColGeoObj();

    int getRobberyItemNum(int, u8);

    static ItemObjMgr *getItemObjMgr()
    {
        return gItemMgr;
    }

    static ItemObjMgr * gItemMgr;

    static bool sTempSlotUseItem[18];
    static int sTempSpecialRatio[9];
private:
    unsigned char _0x4[0x828 - 0x4];
};

inline ItemObjMgr *GetItemObjMgr()
{
    return ItemObjMgr::getItemObjMgr();
}

class ItemShuffleMgr {
public:
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

    struct KartSlotRankDataSet {
        KartSlotData* data;
        u32 specialItemIndex;
        u32 specialItemChance;
        int rankIdx;
        int total;
        int _0x14;
    };
    static KartSlotData mSlotList;
    static KartSlotData mSlotListEnemy;

    static u8 sSlotNormalItemNum; // 9
    static u8 sSlotSpecialItemNum; // 9
    static u32 *sSlotKindIndexArray;

    ItemShuffleMgr() {
        _0x4 = 0;
        _0x5 = 0;
    }

    int calcSlot(KartSlotRankDataSet & slotRankData, int p2, int p3, bool p4);

    virtual ~ItemShuffleMgr();
    virtual void calcRaceUseNormalItem(u32 *, KartSlotRankDataSet *, int);
    virtual void calcSpecialItemNum(u32 *, KartSlotRankDataSet *, int, int, bool);
    virtual int calcRank(KartSlotRankDataSet);

private:
    u16 _0x4; // might be the amount of button presses to speed up the shuffle
    u8 _0x5;  //
    u8 _0x6;
    u8 _0x7;
    int idx;
};

class ItemRndSpecialShuffleMgr : public ItemShuffleMgr {
public:
    virtual ~ItemRndSpecialShuffleMgr();
    virtual void calcRaceUseNormalItem(u32 *, KartSlotRankDataSet *, int);
    virtual void calcSpecialItemNum(u32 *, KartSlotRankDataSet *, int, int, bool);
    virtual int calcRank(KartSlotRankDataSet);
};

inline void CreateItemObjMgr();

#endif // ITEMOBJMGR_H
