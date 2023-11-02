#ifndef SCENE_H
#define SCENE_H

#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "types.h"

class J2DOrthoGraph;

class Scene
{
    JKRArchive *mArchive;
    J2DOrthoGraph *mOrtho;

public:
    Scene(JKRArchive *archive, JKRHeap *heap);
    virtual ~Scene();
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void calc() = 0;

    enum SceneType
    {
        SCENE_0,
        SCENE_1,
        SCENE_2,
        SCENE_3,
        SCENE_4,
        SCENE_5,
        SCENE_6,
        SCENE_7,
        SCENE_8,
        SCENE_9,
        SCENE_10,
        SCENE_11,
        SCENE_ERROR
    };
};

#endif