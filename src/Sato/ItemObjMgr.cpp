#include <JSystem/JUT/JUTDbPrint.h>

#include "ItemShuffleMgr.h"
#include "ItemObjMgr.h"
#include "stMath.h"

bool sTempSlotUseItem[18];
int sTempSpecialRatio[9];

void ItemObjMgr::draw() {
    JUTReport(20, 60, "SlotTable");
    // i have a feeling this is inlined
    ItemShuffleMgr::KartSlotData * slotData = &ItemShuffleMgr::mSlotList;
    JUTReport(100, 80, "Player");

    for (u8 kartNum = 0; kartNum < slotData->kartCount; kartNum++) {
        for (u8 slotNum = 0; slotNum < slotData->totalSlots; slotNum++) {
            JUTReport(kartNum * 33 + 20, slotNum * 12 + 100, "%u", slotData->mList[kartNum].slotTable[0]->chance[slotNum]);
        }
    }
    // might need to use some inline functions to get this because of private members
    if (RaceMgr::sRaceManager->raceInfo->raceMode == GRAND_PRIX) {
        ItemShuffleMgr::KartSlotData * slotData = &ItemShuffleMgr::mSlotListEnemy;
        JUTReport(400, 80, "Enemy");
        for (u8 kartNum = 0; kartNum < slotData->kartCount; kartNum++) {
            for (u8 slotNum = 0; slotNum < slotData->totalSlots; slotNum++) {
                JUTReport(kartNum * 33 + 320, slotNum * 12 + 100, "%u", slotData->mList[kartNum].slotTable[0]->chance[slotNum]);
            }
        }
    }
}


s32 ItemShuffleMgr::calcRank(KartSlotRankDataSet slotRankData) {

    s32 ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    s32 uVar3 = 0;
    s32 uVar2 = 0;

    for (s32 i = 0; i < sSlotNormalItemNum + 1; i++, uVar3 = uVar2)
    {
        s32 itemChance = slotRankData.data->mList[slotRankData.rankIdx].slotTable[1]->chance[i];
        s32 itemIdx = i;
        if (i >= (s32)sSlotNormalItemNum) {
            itemIdx = slotRankData.specialItemIndex;
            itemChance = slotRankData.specialItemChance;
        }

        uVar2 = uVar2 + (itemChance & (-(sTempSlotUseItem[i] != false)));
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[itemIdx];
            break;
        }
    }

    if (ret == -1)
        ret = 3;

    return ret;

}

s32 ItemShuffleMgr::calcSlot(KartSlotRankDataSet & slotRankData, s32 p2, s32 p3, bool p4) {

    u32 total = 0;

    calcRaceUseNormalItem(&total, &slotRankData, p3);
    calcSpecialItemNum(&total, &slotRankData, p2, p3, p4);

    slotRankData.total = total;

    return calcRank(slotRankData);
}

s32 ItemRndSpecialShuffleMgr::calcRank(KartSlotRankDataSet slotRankData) {
    s32 ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    s32 uVar3 = 0;
    s32 uVar2 = 0;

    for (s32 idx = 0; idx < (s32)slotRankData.data->totalSlots; idx++, uVar3 = uVar2) {
        u32 chance;
        if (idx >= (s32)sSlotNormalItemNum) {
            chance = sTempSpecialRatio[idx - sSlotNormalItemNum];
        }
        else {
            chance = slotRankData.data->mList[slotRankData.rankIdx].slotTable[1]->chance[idx];
        }

        uVar2 = uVar2 + (chance & (-(sTempSlotUseItem[idx] != false)));
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[idx];
            break;
        }
    }

    if (ret == -1)
        ret = 3;

    return ret;
}
