#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include "Kameda/Scene.h"
#include "Kameda/BattleName2D.h"

// TODO
class CourseName2D;
class Cup2D;

class SceneFactory
{
public:
    SceneFactory();                                  // 0x801509c4
    ~SceneFactory();                                 // 0x801509f4
    void loadData(Scene::SceneType, JKRHeap *);      // 0x80150a7c
    Scene *createScene(Scene::SceneType, JKRHeap *); // 0x801510f0
    
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

    JKRArchive *getArchive(int idx) { return mArchives[idx]; }

    static SceneFactory *ptr() { return mspSceneFactory; }
    
private:
    static SceneFactory *mspSceneFactory; // 0x804162f8

    JKRArchive *mArchives[15];
    CourseName2D *mCourseName;
    BattleName2D *mBattleName;
    Cup2D *mCup;
}; // size: 0x48;

#endif // SCENEFACTORY_H
