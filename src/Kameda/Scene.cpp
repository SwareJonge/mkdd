#include "JSystem/JUtility/JUTDbg.h"
#include "Osako/system.h"
#include "Kameda/Scene.h"

Scene::Scene(JKRArchive *archive, JKRHeap *heap)
{
#line 27
    JUT_ASSERT(heap);
    mArchive = archive;
    mOrtho = System::getJ2DOrtho();
}

Scene::~Scene() {}