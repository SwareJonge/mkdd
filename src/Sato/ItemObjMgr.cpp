#include "JSystem/JUtility/JUTDbPrint.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/stMath.h"
#include "Sato/ItemObjMgr.h"

// WIP

void ItemObjMgr::draw() {
    JUTReport(20, 60, "SlotTable");
    // i have a feeling this is inlined / has some sort of macro
    ItemShuffleMgr::KartSlotData * slotData = &ItemShuffleMgr::mSlotList;
    JUTReport(100, 80, "Player");

    for (u8 kartNum = 0; kartNum < slotData->kartCount; kartNum++) {
        for (u8 slotNum = 0; slotNum < slotData->totalSlots; slotNum++) {
            JUTReport(kartNum * 33 + 20, slotNum * 12 + 100, "%u", slotData->mList[kartNum].slotTable[0]->chance[slotNum]);
        }
    }
    
    if (RCMGetManager()->isRaceModeGp()) {
        ItemShuffleMgr::KartSlotData * slotData = &ItemShuffleMgr::mSlotListEnemy;
        JUTReport(400, 80, "Enemy");
        for (u8 kartNum = 0; kartNum < slotData->kartCount; kartNum++) {
            for (u8 slotNum = 0; slotNum < slotData->totalSlots; slotNum++) {
                JUTReport(kartNum * 33 + 320, slotNum * 12 + 100, "%u", slotData->mList[kartNum].slotTable[0]->chance[slotNum]);
            }
        }
    }
}

// TODO: make SlotTable or KartSlotData unnamed?
int ItemShuffleMgr::calcRank(KartSlotRankDataSet rdata) {
    int ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(rdata.total - 1);

    int uVar3 = 0;
    int uVar2 = 0;

    for (int i = 0; i < sSlotNormalItemNum + 1; i++, uVar3 = uVar2) {
        int itemChance = rdata.data->mList[rdata.kart_rank].slotTable[1]->chance[i];
        int itemIdx = i;
        if (i >= sSlotNormalItemNum) {
            itemIdx = rdata.specialItemIndex;
            itemChance = rdata.specialItemChance;
        }

        uVar2 += ItemObjMgr::sTempSlotUseItem[idx] ? itemChance : 0;
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[itemIdx];
            break;
        }
    }
    // if ret == -1 hang, in the release version, just give the player a banana as failsafe
#line 3995
    JUT_ASSERT(ret != -1); 

    if (ret == -1)
        ret = 3;

    return ret;

}

int ItemShuffleMgr::calcSlot(KartSlotRankDataSet &rdata, int p2, int p3, bool p4) {
#line 4017
    JUT_MINMAX_ASSERT(0, rdata.kart_rank, rdata.data->kartCount);
    u32 total = 0;

    calcRaceUseNormalItem(&total, &rdata, p3);
    calcSpecialItemNum(&total, &rdata, p2, p3, p4);

    rdata.total = total;

    return calcRank(rdata);
}

int ItemRndSpecialShuffleMgr::calcRank(KartSlotRankDataSet slotRankData) {
    int ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    int uVar3 = 0;
    int uVar2 = 0; // this value adds up the probabilty of each item unless a tempSlotItem is unavailable(eg lightning, blue shell etc.)

    for (int idx = 0; idx < slotRankData.data->totalSlots; idx++, uVar3 = uVar2) {
        u32 itemChance = idx >= sSlotNormalItemNum ? 
            ItemObjMgr::sTempSpecialRatio[idx - sSlotNormalItemNum] : 
            slotRankData.data->mList[slotRankData.kart_rank].slotTable[1]->chance[idx];

        uVar2 += ItemObjMgr::sTempSlotUseItem[idx] ? itemChance : 0;
        if ((uVar3 <= randomNum) && (uVar2 > randomNum)) {
            ret = sSlotKindIndexArray[idx];
            break;
        }
    }
#line 4189
    JUT_ASSERT(ret != -1);

    if (ret == -1)
        ret = 3;

    return ret;
}
