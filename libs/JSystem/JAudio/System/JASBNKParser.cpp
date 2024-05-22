#include "JSystem/JAudio/System/JASBNKParser.h"
#include "JSystem/JAudio/System/JASDrumSet.h"
#include "JSystem/JAudio/System/JASCalc.h"

JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

namespace JASBNKParser
{
    u32 sUsedHeapSize;

    static JASOscillator::Data *findOscPtr(JASBasicBank *pBank, const THeader *pHeader, const TOsc *pOsc);
    static const JASOscillator::Point *getOscTableEndPtr(const JASOscillator::Point *pPoint);

    JASBank *createBank(const void *p, JKRHeap *pHeap) { return createBasicBank(p, pHeap); }

    JASBasicBank *createBasicBank(const void *p, JKRHeap *pHeap)
    {
        if (pHeap == NULL)
            pHeap = JASDram;        
        
        u32 oldSize = pHeap->getFreeSize();
        const THeader *headerp = (const THeader *)p;
        JASBasicBank *bank = new (pHeap, 0) JASBasicBank();

        if (bank == NULL)
            return NULL;

        for (int i = 0; i < 0x80; i++)
        {
            const TInst *instrument = headerp->bank.instOffsets[i].ptr(headerp);

            if (instrument == NULL)
                continue;
            
            JASBasicInst *instp = new (pHeap, 0) JASBasicInst();
#line 56
            JUT_ASSERT(instp != 0);
            instp->setVolume(instrument->volume);
            instp->setPitch(instrument->pitch);

            for (int osc_no = 0, j = 0; j < 2; j++)
            {
                TOsc *oscp = instrument->oscOffsets[j].ptr(headerp);
                if (oscp == NULL)
                    continue;

                JASOscillator::Data *osc = findOscPtr(bank, headerp, oscp);
                if (osc == NULL)
                {
                    osc = new (pHeap, 0) JASOscillator::Data;
#line 71
                    JUT_ASSERT(osc != 0);
                    osc->mTarget = oscp->target;
                    osc->_04 = oscp->_4;
                    const JASOscillator::Point *start = oscp->oscPointOffset.ptr(headerp);
                    if (start != NULL)
                    {
                        const JASOscillator::Point *end = getOscTableEndPtr(start);
                        JASOscillator::Point *table = new (pHeap, 0) JASOscillator::Point[(end - start)];
#line 81
                        JUT_ASSERT(table != 0);
                        JASCalc::bcopy(start, table, (end - start) * sizeof(JASOscillator::Point));
                        osc->mTable = table;
                    }
                    else
                    {
                        osc->mTable = nullptr;
                    }
                    start = oscp->oscPointOffset2.ptr(headerp);
                    if (start != NULL)
                    {
                        const JASOscillator::Point *end = getOscTableEndPtr(start);
                        JASOscillator::Point *table = new (pHeap, 0) JASOscillator::Point[(end - start)];
                        JUT_ASSERT(table != 0);
                        JASCalc::bcopy(start, table, (end - start) * sizeof(JASOscillator::Point));
                        osc->mTable2 = table;
                    }
                    else
                    {
                        osc->mTable2 = nullptr;
                    }

                    osc->mScale = oscp->scale;
                    osc->_14 = oscp->_14;                   
                }
                instp->setOsc(osc_no, osc);
                osc_no++;
            }
            for(int j = 0, effectNo = 0; j < 2; j++) 
            {
                TRand *rand = instrument->randOffsets[j].ptr(headerp);
                if (rand == NULL)
                    continue;

                JASInstRand *randp = new (pHeap, 0) JASInstRand();
#line 120
                JUT_ASSERT(randp != 0);
                randp->setType(rand->type);
                randp->setFloor(rand->floor);
                randp->setCeiling(rand->ceiling);
                instp->setEffect(j, randp);
            }
            for (int j = 0; j < 2; j++)
            {
                const TSense *sense = instrument->senseOffsets[j].ptr(headerp);
                if (sense == NULL)
                    continue;

                JASInstSense *sensep = new (pHeap, 0) JASInstSense();               
                JUT_ASSERT(sensep != 0);                
                sensep->setType(sense->type);
                sensep->setParams(sense->trigger, sense->centerkey, sense->_4, sense->_8);
                instp->setEffect(j + 2, sensep);
            }

            instp->setKeyRegionCount(instrument->keyRegionCount, pHeap);

            for (int j = 0; j < instrument->keyRegionCount; j++)
            {
                JASBasicInst::TKeymap *keyregionp = instp->getKeyRegion(j);
                TKeymap *key = instrument->keyRegionOffsets[j].ptr(headerp);
                keyregionp->setHighKey(key->high);
                keyregionp->setVeloRegionCount(key->veloRegionCount, pHeap);

                for (int k = 0; k < key->veloRegionCount; k++) 
                {
                    JASVelo *velop = keyregionp->getVeloRegion(k);
                    TVmap *vmap = key->vmapOffsets[k].ptr(headerp);

                    velop->_0 = vmap->_0;
                    velop->_4 = JSULoHalf(vmap->_4);
                    velop->_8 = vmap->_8;
                    velop->_c = vmap->_c;
                }
            }
            bank->setInst(i, instp);
        }

        for (int i = 0; i < 12; i++)
        {
            TPerc *perc = headerp->bank.percOffsets[i].ptr(headerp);
            if (perc == NULL)
                continue;
#line 182
            JASDrumSet *setp = new (pHeap, 0) JASDrumSet();
            JUT_ASSERT(setp != 0);

            for (int j = 0; j < 0x80; j++)
            {
                TPmap *pmapp = perc->pmapOffsets[j].ptr(headerp);
                if (pmapp == NULL)
                    continue;
#line 189
                JASDrumSet::TPerc *percp = new (pHeap, 0) JASDrumSet::TPerc();
                JUT_ASSERT(percp);
                percp->setVolume(pmapp->volume);
                percp->setPitch(pmapp->pitch);

                if (perc->magic == 'PER2') 
                {
                    percp->setPan(perc->panTable[j] / 127.0f);
                    percp->setRelease(perc->releaseTable[j]);
                }

                for (int effectNo = 0, k = 0; k < 2; k++)
                {
                    TRand *rand = pmapp->randOffsets[k].ptr(headerp);
                    if (rand == NULL)
                        continue;

                    JASInstRand *randp = new (pHeap, 0) JASInstRand();
                    JUT_ASSERT(randp != 0);
                    randp->setType(rand->type);
                    randp->setFloor(rand->floor);
                    randp->setCeiling(rand->ceiling);
                    percp->setEffect(effectNo, randp);
                    effectNo++;
                }

                percp->setVeloRegionCount(pmapp->veloRegionCount, pHeap);

                for (int k = 0; k < pmapp->veloRegionCount; k++)
                {
                    JASVelo *velop = percp->getVeloRegion(k);
                    TVmap *vmap = pmapp->veloRegionOffsets[k].ptr(headerp);

                    velop->_0 = vmap->_0;
                    velop->_4 = JSULoHalf(vmap->_4);
                    velop->_8 = vmap->_8;
                    velop->_c = vmap->_c;
                }
                setp->setPerc(j, percp);
            }

            bank->setInst(i + 228, setp);
        }
        sUsedHeapSize += (oldSize - pHeap->getFreeSize());
        return bank;
    }

    JASOscillator::Data *findOscPtr(JASBasicBank *pBank, const THeader *pHeader, const TOsc *pOsc)
    {
        const TBank *bankp = &pHeader->bank;

        for (int i = 0; i < MAX_INST; i++)
        {
            TInst *instp = bankp->instOffsets[i].ptr(pHeader);
            if (instp == NULL)
                continue;

            for (int j = 0; j < MAX_OSC; j++)
            {
                if (instp->oscOffsets[j].ptr(pHeader) != pOsc)
                    continue;

                JASInst *bankInst = pBank->getInst(i);
                if (bankInst == NULL)
                    continue;

                JASInstParam instParam;
                bankInst->getParam(60, 127, &instParam);
                if (j < instParam._20)
                {
                    return instParam._1c[j];
                }
            }
        }        
        return NULL;
    }

    const JASOscillator::Point *getOscTableEndPtr(const JASOscillator::Point *pPoint) {
        int i;
        do
        {
            i = pPoint->_0;
            pPoint++;
        } while (i <= 10);
        return pPoint;
    }

    u32 getUsedHeapSize() { return sUsedHeapSize; }
}
