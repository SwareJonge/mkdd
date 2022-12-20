#ifndef SCENE_H
#define SCENE_H

#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "types.h"

class J2DOrthoGraph;

class Scene {    
    JKRArchive * mArchive;
    J2DOrthoGraph * mOrtho;
public:
    Scene(JKRArchive * archive, JKRHeap* heap);
    virtual ~Scene();
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void calc() = 0;
};

#endif