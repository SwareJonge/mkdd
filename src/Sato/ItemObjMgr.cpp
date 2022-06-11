#include "Sato/ItemObjMgr.h"
#include "Sato/stMath.h"

#include "JSystem/JUtility/JUTDbPrint.h"
#include "Kaneshige/RaceMgr.h"


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

int ItemShuffleMgr::calcRank(KartSlotRankDataSet slotRankData) {
    int ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    int uVar3 = 0;
    int uVar2 = 0;

    for (int i = 0; i < sSlotNormalItemNum + 1; i++, uVar3 = uVar2)
    {
        int itemChance = slotRankData.data->mList[slotRankData.rankIdx].slotTable[1]->chance[i];
        int itemIdx = i;
        if (i >= (int)sSlotNormalItemNum) {
            itemIdx = slotRankData.specialItemIndex;
            itemChance = slotRankData.specialItemChance;
        }

        uVar2 = uVar2 + (itemChance & (-(ItemObjMgr::sTempSlotUseItem[i] != false)));
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[itemIdx];
            break;
        }
    }

    if (ret == -1)
        ret = 3;

    return ret;

}

int ItemShuffleMgr::calcSlot(KartSlotRankDataSet & slotRankData, int p2, int p3, bool p4) {

    u32 total = 0;

    calcRaceUseNormalItem(&total, &slotRankData, p3);
    calcSpecialItemNum(&total, &slotRankData, p2, p3, p4);

    slotRankData.total = total;

    return calcRank(slotRankData);
}

int ItemRndSpecialShuffleMgr::calcRank(KartSlotRankDataSet slotRankData) {
    int ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    int uVar3 = 0;
    int uVar2 = 0;

    for (int idx = 0; idx < (int)slotRankData.data->totalSlots; idx++, uVar3 = uVar2) {
        u32 chance;
        if (idx >= (int)sSlotNormalItemNum) {
            chance = ItemObjMgr::sTempSpecialRatio[idx - sSlotNormalItemNum];
        }
        else {
            chance = slotRankData.data->mList[slotRankData.rankIdx].slotTable[1]->chance[idx];
        }

        uVar2 = uVar2 + (chance & (-(ItemObjMgr::sTempSlotUseItem[idx] != false)));
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[idx];
            break;
        }
    }

    if (ret == -1)
        ret = 3;

    return ret;
}
