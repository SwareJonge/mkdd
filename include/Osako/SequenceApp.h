#ifndef SEQUENCEAPP_H
#define SEQUENCEAPP_H

#include "Osako/GameApp.h"
#include "Kameda/Scene.h"

class SequenceApp : public GameApp
{ // Autogenerated
public:
    // Global
    static SequenceApp *create();         // 0x80208aec
    static void call(Scene::SceneType);   // 0x80208b3c
    SequenceApp();                        // 0x80208b88
    virtual ~SequenceApp();               // 0x80208c58
    virtual void draw();                  // 0x80208e80
    virtual void calc();                  // 0x80208ec8
    virtual void reset();                 // 0x80209064
    void freeForMovieApp();               // 0x80208d6c
    static void loadTask(void *);         // 0x80209068
    bool ready(Scene::SceneType scene);   // 0x802090d4
    bool checkFinishAllLoading();         // 0x802091c8
    Scene *createScene(Scene::SceneType); // 0x802092c8

    // Inline/Unused
    int checkReady(Scene::SceneType);

    // Inline
    static SequenceApp *ptr() { return mspSequenceApp; }

private:
    static Scene::SceneType msScene;     // 0x80414868
    static Scene::SceneType msNextScene; // 0x8041486c
    static SequenceApp *mspSequenceApp;  // 0x80416a18

    Scene *mNextScene;
    Scene *mScenes[Scene::SCENE_MAX];
    u32 mLoadedScenes;
    u32 mActiveScenes;
    Scene::SceneType mPrevScene;
    u32 mState;
    int mDrawFlags;
}; // class SequenceApp

#define SEQUENCEAPP_isReady(scene) \
    SequenceApp::ptr()->ready(scene)

#endif