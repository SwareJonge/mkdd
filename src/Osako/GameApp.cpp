#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"

#include "Kaneshige/SysDebug.h"
#include "Osako/system.h"
#include "Osako/GameApp.h"

GameApp::GameApp(u32 heapSize, const char *appName, void *otherHeap)
{
    mHeapMem = otherHeap;
    if (otherHeap != nullptr)
        mHeap = JKRExpHeap::create(otherHeap, heapSize, nullptr, false);
    else
    {
        if (heapSize == 0)
            mHeap = JKRAssertHeap::create(System::getAppHeap());
        else
            mHeap = JKRExpHeap::create(heapSize, System::getAppHeap(), false);
    }

    mHeap->becomeCurrentHeap();
    SYSDBG_CreateHeapInfo(mHeap, (char *)appName);
}

GameApp::~GameApp()
{
    SYSDBG_DestroyHeapInfo(mHeap);
    mHeap->destroy();
    delete mHeapMem;
}