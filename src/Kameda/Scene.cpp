#include "Osako/system.h"
#include "Kameda/Scene.h"
#include "JSystem/JUtility/JUTAssert.h"

Scene::Scene(JKRArchive * archive, JKRHeap* heap) {
    JUT_ASSERT_F(27, heap, "%s", "heap"); // There probably is a different type of assert for this, however if it works it works
    mArchive = archive;
    mOrtho = System::mspJ2DOrtho;
}

Scene::~Scene() { }