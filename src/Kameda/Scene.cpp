#include "JSystem/JUtility/JUTDbg.h"
#include "Osako/system.h"
#include "Kameda/Scene.h"

Scene::Scene(JKRArchive * archive, JKRHeap* heap) {
    JUT_ASSERT(27, heap);
    mArchive = archive;
    mOrtho = System::getJ2DOrtho();
}

Scene::~Scene() { }