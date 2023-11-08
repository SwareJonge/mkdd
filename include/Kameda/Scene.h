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
        SCENE_TITLE,
        SCENE_MENU,
        SCENE_OPTION,
        SCENE_COURSE,
        SCENE_MAP_SELECT,
        SCENE_RECORD,
        SCENE_GP_NEXT,
        SCENE_GHOST_LOAD_SAVE,
        SCENE_LAN_ENTRY,
        SCENE_SECRET,
        SCENE_MAX,
        SCENE_NONE, // Guess
        SCENE_12, // unknown
        SCENE_ERROR
    };
};

#endif