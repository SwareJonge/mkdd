#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"

#include "Kaneshige/SysDebug.h"
#include "Osako/system.h"
#include "Osako/GameApp.h"

GameApp::GameApp(u32 heapSize, const char *appName, void *otherHeap)
{
    mSequenceHeap = otherHeap;
    if (otherHeap != nullptr)
        mHeap = JKRExpHeap::create(otherHeap, heapSize, nullptr, false);
    else
    {
        if (heapSize == null)
            mHeap = JKRAssertHeap::create(System::mspAppHeap);
        else
            mHeap = JKRExpHeap::create(heapSize, System::mspAppHeap, false);
    }

    mHeap->becomeCurrentHeap();
    SysDebug::getManager()->createHeapInfo(mHeap, (char *)appName);
}

GameApp::~GameApp()
{
    SysDebug::getManager()->destroyHeapInfo(mHeap);
    mHeap->destroy();
    delete mSequenceHeap;
}