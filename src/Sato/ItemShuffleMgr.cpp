#include "ItemShuffleMgr.h"
#include "ItemObjMgr.h"
#include "stMath.h"

s32 ItemShuffleMgr::calcRank(ItemShuffleMgr::KartSlotRankDataSet slotRankData) {

    s32 ret = -1;
    u32 randomNum = stGetRnd(0)->getRandomMax(slotRankData.total - 1);

    s32 uVar3 = 0;
    s32 uVar2 = 0;

    for (s32 i = 0; i < sSlotNormalItemNum + 1; i++, uVar3 = uVar2)
    {
        s32 itemChance = slotRankData.slotData->slotList[slotRankData.rank].slotTable[1]->chance[i];
        s32 itemIdx = i;
        if (i >= (s32)sSlotNormalItemNum) {
            itemIdx = slotRankData.specialItemOffset;
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