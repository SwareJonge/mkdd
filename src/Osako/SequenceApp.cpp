
#include "Inagaki/GameAudioMain.h"
#include "Kameda/CharacterSelect3D.h"
#include "Kameda/SceneFactory.h"
#include "Osako/AppMgr.h"
#include "Osako/CardAgent.h"
#include "Osako/kartPad.h"
#include "Osako/MainMenuApp.h"
#include "Osako/MenuBackground.h"
#include "Osako/MenuTitleLine.h"
#include "Osako/ResetAgent.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/system.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

#ifdef DEBUG
#define HEAP_SIZE 0x680000
static const float float_slack[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(float_slack)
#pragma pop
#else
#define HEAP_SIZE 0x65f400
#endif

Scene::SceneType SequenceApp::msScene = Scene::SCENE_NONE;
Scene::SceneType SequenceApp::msNextScene = Scene::SCENE_NONE;

SequenceApp *SequenceApp::mspSequenceApp;

SequenceApp *SequenceApp::create()
{
    if (mspSequenceApp == nullptr)
    {
        mspSequenceApp = new (SYSTEM_GetAppHeap(), 0) SequenceApp();
    }
    return mspSequenceApp;
}

void SequenceApp::call(Scene::SceneType scene)
{
    msNextScene = scene;
    if (mspSequenceApp)
    {
        mspSequenceApp->mState = 0;
        mspSequenceApp->mDrawFlags = 0;
        msScene = Scene::SCENE_NONE;
    }
    AppMgr::setNextApp(AppMgr::mcSequence);
}

SequenceApp::SequenceApp() : GameApp(HEAP_SIZE, "Sequence", nullptr)
{
    mNextScene = nullptr;
    SceneFactory::create();

    for (int i = 0; i < Scene::SCENE_MAX; i++)
        mScenes[i] = nullptr;

    mLoadedScenes = 0;
    mActiveScenes = 0;

    if (msNextScene != Scene::SCENE_NONE)
        ready(msNextScene);

    mState = 0;
    mDrawFlags = 0;
    msScene = Scene::SCENE_NONE;
}

SequenceApp::~SequenceApp()
{
    for (int i = 0; i < Scene::SCENE_MAX; i++)
        delete mScenes[i];

    // TODO: use inline to static instance or just the static instance?

    delete SceneFactory::getSceneFactory();
    delete MenuBackground::ptr();
    delete MenuTitleLine::ptr();
    delete CharacterSelect3D::getCharacterSelect3D();

    msNextScene = Scene::SCENE_NONE;
    msScene = Scene::SCENE_NONE;
    mspSequenceApp = nullptr;
}

void SequenceApp::freeForMovieApp()
{
    for (int i = 0; i < Scene::SCENE_MAX; i++)
    {
        delete mScenes[i];
        mScenes[i] = nullptr;
    }

    mLoadedScenes = 0;
    mActiveScenes = 0;
    mNextScene = nullptr;

    // TODO: use inline to static instance or just the static instance?

    delete SceneFactory::getSceneFactory();
    delete MenuBackground::ptr();
    delete MenuTitleLine::ptr();
    delete CharacterSelect3D::getCharacterSelect3D();

    mHeap->freeAll();

    SceneFactory::create();
}

void SequenceApp::draw()
{
    switch (mDrawFlags)
    {
    case 1:
        if (mNextScene == nullptr)
            return;
        mNextScene->draw();
    }
}

void SequenceApp::calc()
{
    switch (mState)
    {
    case 0:
        if (msNextScene == Scene::SCENE_NONE)
            return;
        if (!ready(msNextScene))
            return;

        mState = 1;
    case 1:
        if (msNextScene != Scene::SCENE_NONE)
        {
            if (ResetAgent::msState != 0)
            {
                mNextScene = nullptr;
            }
            else
            {
                mNextScene = createScene(msNextScene);
                mPrevScene = msScene;
                msScene = msNextScene;
                msNextScene = Scene::SCENE_NONE;
                mNextScene->init();
            }
        }

#ifdef DEBUG
        if (gGamePad1P.testButton(PAD_BUTTON_MENU) && gGamePad1P.testButton(PAD_TRIGGER_Z))
        {
            
            if ((CardAgent::msState != 0) && (CardAgent::msState != 0xf))
            {
                CardAgent::msFlags |= 4;
            }
            mState = 2;
        }
        else
#endif
        if (mNextScene)
        {
            mNextScene->calc();
        }

        mDrawFlags = 1;
        break;
    case 2:
        if ((CardAgent::msState == 0) || (CardAgent::msState == 0xf))
        {
            if (!checkFinishAllLoading())
                return;

            if (!ResMgr::isFinishLoadingArc(ResMgr::mcArcARAM))
                return;

            if (!GetGameAudioMain()->isWaveLoaded(GameAudioMain::STREAM3))
                return;

            AppMgr::deleteCurrentApp();
            MainMenuApp::call();
            mState = 1;
        }
    }
}

void SequenceApp::reset() {}

void SequenceApp::loadTask(void *arg)
{
    Scene::SceneType scene = (Scene::SceneType)arg;
    SceneFactory::getSceneFactory()->loadData(scene, ptr()->mHeap);

    BOOL enable = OSDisableInterrupts();
    ptr()->mLoadedScenes &= ~(1 << scene);
    ptr()->mActiveScenes |= 1 << scene;
    OSRestoreInterrupts(enable);
}

bool SequenceApp::ready(Scene::SceneType scene)
{
#line 265
    JUT_ASSERT(scene < 32)
    JUT_ASSERT(scene < Scene::SCENE_ERROR)

    if (!(mActiveScenes & 1 << scene))
    {
        if (!(mLoadedScenes & (1 << scene)))
        {
            SYSTEM_RequestTask(loadTask, (void *)scene, nullptr);
            mLoadedScenes |= 1 << scene;
        }
        return false;
    }

    return true;
}

int SequenceApp::checkReady(Scene::SceneType scene)
{
#line 286
    JUT_ASSERT(scene < 32)
    JUT_ASSERT(scene < Scene::SCENE_ERROR)

    int ret = 1;

    if (!(mActiveScenes & 1 << scene))
    {
        if ((mLoadedScenes & (1 << scene)))
            return 1;
        else
            return 0;
    }
    return 2;
}

bool SequenceApp::checkFinishAllLoading()
{
    bool finishedLoading = true;
    for (u8 i = 0; i < Scene::SCENE_MAX; i++)
    {
        if (checkReady((Scene::SceneType)i) == 1)
        {
            finishedLoading = false; // why not break or return here?
        }
    }
    return finishedLoading;
}

Scene *SequenceApp::createScene(Scene::SceneType scene)
{
    if (mScenes[scene] == nullptr)
        mScenes[scene] = SceneFactory::getSceneFactory()->createScene(scene, mHeap);
    return mScenes[scene];
}
