#include <JSystem/J3D/J3DModelLoader.h>

#include "Kaneshige/Movie/MovieApp.h"
#include "Kaneshige/Movie/MoviePlayer.h"
#include "Osako/AppMgr.h"
#include "Osako/kartPad.h"
#include "Osako/SequenceApp.h"
#include "Osako/system.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

MovieApp *MovieApp::sMovieApp;
void *MovieApp::mspHeapBuffer;

void MovieJugemActor::loadModelData(GeographyObj *obj) {
    void *bmd = MovieApp::getArchive(obj->getBmdFileName());
    J3DModelData *model = J3DModelLoaderDataBase::load(bmd, obj->getJ3DModelDataTevStageNum() | 0x19000010);
    obj->loadmodel(model);
    model->newSharedDisplayList(0x40000);
}

MovieJugemActor::MovieJugemActor() {
    mJugem = new TJugem();
    mVoidRod = new TJugemVoidRod();
    u32 pattern;
    
    u32 rnd = (u32)OSGetTime() % 10u;

    if (rnd <= 6) {
        pattern = 0;
    }
    else if (rnd <= 8) {
        pattern = 1;
    }
    else {
        pattern = 2;
    }

#ifdef DEBUG
    if (gGamePad1P.testButton(JUTGamePad::DPAD_LEFT))
        pattern = 0;
    else if (gGamePad1P.testButton(JUTGamePad::DPAD_UP))
        pattern = 1;
    else if (gGamePad1P.testButton(JUTGamePad::DPAD_RIGHT))
        pattern = 2;
#endif
    u32 id;
    switch(pattern) {
    case 0:
        id = 2;
        mRodItem = new TJugemRodSignal();
        break;
    case 1:
    case 2:
        id = 5;
        mRodItem = new TJugemRodPukuPuku();
        break;
    default:
        id = 0;
        mRodItem = nullptr;
        break;
    }
    mHear = new TJugemHeadHear();
    loadModelData(mJugem);
    loadModelData(mVoidRod);
    loadModelData(mRodItem);
    loadModelData(mHear);
    mJugem->loadAnimation();
    mVoidRod->loadAnimation();
    mRodItem->loadAnimation();
    mHear->loadAnimation();
    mJugem->reset();
    mJugem->setDemoPattern(pattern);
    mVoidRod->reset();
    mRodItem->reset();
    mHear->reset();

    Mtx m;
    PSMTXIdentity(m);
    j3dSys.setViewMtx(m);
    
    u32 size =  (int)JKRGetCurrentHeap()->getFreeSize();
    JKRSolidHeap *modelHeap = JKRCreateSolidHeap(size, JKRGetCurrentHeap(), false);
    JKRHeap *bak = modelHeap->becomeCurrentHeap();
    mJugem->createModel(modelHeap, 1);
    mVoidRod->createModel(modelHeap, 1);
    mRodItem->createModel(modelHeap, 1);
    mHear->createModel(modelHeap, 1);
    modelHeap->adjustSize();
    bak->becomeCurrentHeap();

    mVoidRod->setJugemRodItem(mRodItem, id);
    mJugem->setVoidRodPtr(mVoidRod);
    mJugem->setHearPtr(mHear);
    mJugem->setCameraNum(0);
    mJugem->startDemo();
}

void MovieJugemActor::setCurViewNo(u32 viewNo) {
    mJugem->setCurrentViewNo(viewNo);
    mVoidRod->setCurrentViewNo(viewNo);
    mRodItem->setCurrentViewNo(viewNo);
    mHear->setCurrentViewNo(viewNo);
}

void MovieJugemActor::calc() {
    mCalcCnt++;
    mJugem->calc();
    mVoidRod->calc();
    mRodItem->calc();
    mHear->calc();
}

void MovieJugemActor::update() {
    mJugem->update_norm();
    mVoidRod->update_norm();
    mRodItem->update_norm();
    mHear->update_norm();
}

void MovieJugemActor::viewCalc(u32 viewNo) {
    mJugem->viewCalc(viewNo);
    mVoidRod->viewCalc(viewNo);
    mRodItem->viewCalc(viewNo);
    mHear->viewCalc(viewNo);
}

void MovieJugemActor::endDemo() { mJugem->endDemo(); }

bool MovieJugemActor::isDemoEnd() { return mJugem->isDemoOut(); }

void *MovieApp::getArchive(const char *name) { return ResMgr::getPtr(ResMgr::mcArcOpening, name); }

MovieApp *MovieApp::create() {
    if(sMovieApp == nullptr) {
        if(SequenceApp::ptr()) {
            JKRHeap *heap =SequenceApp::ptr()->getHeap();
            void *mem = heap->alloc(0x200000, -4);
            sMovieApp = new (heap, -4) MovieApp(mem);
        }
        else {
            mspHeapBuffer = nullptr;
            sMovieApp = NEW_APP MovieApp(nullptr);
        }
        
    }
    return sMovieApp;
}

void MovieApp::call() { AppMgr::setNextApp(AppMgr::mcMovie); }

MovieApp::MovieApp(void *mem) : GameApp(0x200000, "Movie", mem) {
    mActor = nullptr;
    mState = 0;
    mRunningState = 0;
    _18 = 0;
    _1a = 0;
    mMdlViewer = new MdlViewer();
    _24 = 0;
    mWaitFrame = 0;
}

MovieApp::~MovieApp() { sMovieApp = nullptr; }

void MovieApp::draw() {
    if(MoviePlayer::getPlayer()) {
        MoviePlayer::getPlayer()->draw();
    }
    j3dSys.drawInit();
    mMdlViewer->draw();
    j3dSys.reinitGX();
}

void MovieApp::calc() {

    if (MoviePlayer::getPlayer()) {
        switch (_24)
        {
        case 1:
            _24 = 2;
            mWaitFrame = 0;
            break;
        case 2:
        {
            mWaitFrame++;
            if(mWaitFrame >= 60) {
                MoviePlayer::getPlayer()->replay();
                _24 = 0;
                mWaitFrame = 0;
            }
            break;
        }
        }
        MoviePlayer::getPlayer()->calc();
    }
    mMdlViewer->calc();


    switch(mState)
    {
    case 0:
    {
        if(MoviePlayer::getPlayer()) {
            mState = 1;
            _18 = 0;
            mRunningState = 0;
            _1a = 0;
            System::changeMovieRenderMode();   
        }
        break;
    }
    case 1:
    {
        if(ResMgr::isFinishLoadingArc(ResMgr::mcArcOpening)) {
            System::getDisplay()->startFadeIn(15);
            mActor = new MovieJugemActor();
            mMdlViewer->append(mActor);
            mState = 2;
        }
        break;
    }
    case 2:
        doRunning();
        break;
    case 3:
        doEnding();
        break;
    default:
#line 647
        JUT_ASSERT_MSG(0, "UNKNOWN STATE");
    }
}

bool MovieApp::isEnd() {
    bool ret = false;
    if(MoviePlayer::getPlayer()) {
        int endFrame = MoviePlayer::getPlayer()->getFrameNumber() - 170;
        if (MoviePlayer::getPlayer()->getDrawFrame() >= endFrame)
            ret = true;

        if(gGamePad1P.testTrigger(JUTGamePad::A | JUTGamePad::START)) {
            ret = true;
        }
    }
    return ret;
}

void MovieApp::reset() {
  if (AppMgr::getPrevApp() == AppMgr::mcErrorView) {
    _24 = 1;
    mWaitFrame = 0;
  }
}

void MovieApp::loadOpeningData() {
    ResMgr::loadOpeningData(getHeap());
}

void MovieApp::doRunning() {
    switch(mRunningState) {
    case 0:
    {
        bool arcLoaded = ResMgr::isFinishLoadingArc(ResMgr::mcArcARAM);
        if(++_1a >= 180 && arcLoaded) {
            mRunningState = 1;
            _1a = 0;
            if(mActor)
                mActor->endDemo();
        }
        break;
    }
    case 1: 
    {
        bool demoEnd = true;
        if(mActor)
            demoEnd = mActor->isDemoEnd();
        if(demoEnd) {
            _1a = 0;
            mRunningState = 2;       
        }
        break;
    }
    default:
    {
        if(MoviePlayer::getPlayer()) {
            if(!MoviePlayer::getPlayer()->isPlaying()) {
                ++_1a;
                if(_1a >= 25)
                    MoviePlayer::getPlayer()->play();
            }
        }
        if(isEnd()) {
            JUTColor color(0xff, 0xff, 0xff, 0xff);
            SYSTEM_GetFader()->setColor(color);
            System::getDisplay()->startFadeOut(90);
            mState = 3;
            _18 = 0;
            if(MoviePlayer::getPlayer()) {
                MoviePlayer::getPlayer()->audioFadeOut(90);
            }
        }
        break;
    }
    }
}

void MovieApp::doEnding() {
    if(++_18 >= 120) {
        System::changeNormalRenderMode();
        MoviePlayer::quitMovie();
        AppMgr::deleteCurrentApp();
        SequenceApp::call(Scene::SCENE_TITLE);
    }
}
