#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JUtility/JUTDbg.h"

JPAResourceManager::JPAResourceManager(u16, u16, JKRHeap *)
{
    // UNUSED
}

JPAResourceManager::JPAResourceManager(const void *p_jpc, JKRHeap *heap)
{
    pResAry = nullptr;
    pTexAry = nullptr;
    resMaxNum = 0;
    resRegNum = 0;
    texMaxNum = 0;
    texRegNum = 0;
    pHeap = heap;
#line 49
    JUT_ASSERT((p_jpc != 0) && (pHeap != 0));
    JPAResourceLoader loader((const u8 *)p_jpc, this);
}

void JPAResourceManager::load(const char *, u16)
{
    // UNUSED
}

void JPAResourceManager::load(const void *, u16)
{
    // UNUSED
}

JPAResource *JPAResourceManager::getResource(u16 idx) const
{
    for (u16 res_idx = 0; res_idx < resRegNum; res_idx++)
    {
        JPAResource *res = pResAry[res_idx];
        if (idx == res->getUsrIdx())
            return res;
    }

    return nullptr;
}

bool JPAResourceManager::checkUserIndexDuplication(u16) const
{
    // UNUSED, but exists in TP
}

ResTIMG *JPAResourceManager::swapTexture(const ResTIMG *, const char *)
{
    // UNUSED
}

void JPAResourceManager::registRes(JPAResource *res)
{
#line 151
    JUT_ASSERT(resRegNum < resMaxNum);
    pResAry[resRegNum] = res;
    resRegNum++;
}

void JPAResourceManager::registTex(JPATexture *res)
{
#line 166
    JUT_ASSERT(texRegNum < texMaxNum);
    pTexAry[texRegNum] = res;
    texRegNum++;
}

void JPAResourceManager::registTexDupCheck(const unsigned char *, JKRHeap *)
{
    // UNUSED
}

void *JPAResourceManager::getResUserWork(u16) const
{
    // UNUSED
}