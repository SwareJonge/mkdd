#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include "Kameda/Scene.h"

class SceneFactory
{
public:
    SceneFactory();                                  // 0x801509c4
    ~SceneFactory();                                 // 0x801509f4
    void loadData(Scene::SceneType, JKRHeap *);      // 0x80150a7c
    Scene *createScene(Scene::SceneType, JKRHeap *); // 0x801510f0
    static SceneFactory *mspSceneFactory;            // 0x804162f8
    // Inline/Unused
    // void mountSceneArc(SceneArc, JKRHeap *);

    // Inline
    static SceneFactory *create()
    {
        if (mspSceneFactory == nullptr)
        {
            mspSceneFactory = new SceneFactory();
        }
        return mspSceneFactory;
    }

    static SceneFactory *getSceneFactory() { return mspSceneFactory; }

private:
    // TODO
    u8 _0[0x48];
}; // size: 0x48;

#endif // SCENEFACTORY_H
