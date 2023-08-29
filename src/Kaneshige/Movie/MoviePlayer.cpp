#include <JSystem/JUtility/JUTVideo.h>

#include "Kaneshige/Movie/MoviePlayer.h"
#include "Kaneshige/THP/THPPlayer.h"

MoviePlayer *MoviePlayer::sPlayer;
int MoviePlayer::sMovieSelector;

const char *MoviePlayer::cMovieFileNameTable = "Movie/play1.thp";
const char *MoviePlayer::cMovie50FileNameTable = "Movie/play1_50.thp";

void MoviePlayer::create(JKRHeap *heap)
{
    new (heap, 0) MoviePlayer(KartLocale::getVideoMode(), KartLocale::getVideoFrameMode(), heap);
    sMovieSelector++;
}

MoviePlayer::MoviePlayer(VideoMode videoMode, VideoFrameMode frameMode, JKRHeap *heap)
{
    _40 = 0;
    sPlayer = this;

    const char *fileName = cMovieFileNameTable;
    if (videoMode == VideoMode_PAL && frameMode == VideoFrameMode_50Hz)
    {
        fileName = cMovie50FileNameTable;
    }

    mHeap = heap;
    if (mHeap == nullptr)
    {
        mHeap = JKRGetCurrentHeap();
    }

    mVideoMode = videoMode;
    mFrameMode = frameMode;
    mDrawFrame = -1;
    mIsDonePlaying = false;
    _2a = 0;
    mBuffer = nullptr;

    mBuf_30 = new (mHeap, 32) u8[64];

    THPPlayerInit();

#line 190
    BOOL open = THPPlayerOpen(fileName, FALSE);
    JUT_ASSERT_MSG(open, "THP OPEN FAIL");

    THPPlayerGetVideoInfo(&mVideoInfo);
    mBuffer = new (mHeap, 32) u8[THPPlayerCalcNeedMemory()];
    THPPlayerSetBuffer(mBuffer);
    reset();
}

MoviePlayer::~MoviePlayer()
{
    THPPlayerQuit();
    delete[] mBuf_30;
    delete[] mBuffer;
    sPlayer = nullptr;
}

void MoviePlayer::reset()
{
    mDrawFrame = -1;
    mIsDonePlaying = false;
    _2a = 0;
    mIsPlaying = false;
#line 251
    BOOL prepared = THPPlayerPrepare(0, 0, 0);
    JUT_ASSERT_MSG(prepared, "THP PRELOAD FAIL");
}

void MoviePlayer::calc()
{
    THPPlayerGetState(); // TODO: did this return state? MovieApp::calc didn't do anything with the return value if that is the case
}

void MoviePlayer::draw()
{
    if (mIsPlaying)
    {
        GXRenderModeObj *rmode = JUTVideo::getManager()->getRenderMode();
        mDrawFrame = THPPlayerDrawCurrentFrame(rmode, rmode->fbWidth - mVideoInfo.xSize >> 1, rmode->efbHeight - mVideoInfo.ySize >> 1, mVideoInfo.xSize, mVideoInfo.ySize);
    }
}

void MoviePlayer::drawDone()
{
    THPPlayerPostDrawDone();
}

void MoviePlayer::quit()
{
    THPPlayerStop();
    THPPlayerClose();
    THPPlayerQuit();
    mIsDonePlaying = true;
}

void MoviePlayer::play()
{
    if (mIsPlaying == false)
    {
        THPPlayerPlay();
        mIsPlaying = true;
    }
}

void MoviePlayer::replay()
{
    if (mIsPlaying)
        THPPlayerPlay();
}

void MoviePlayer::pause()
{
    if (mIsPlaying)
        THPPlayerPause();
}

void MoviePlayer::audioFadeOut(int time)
{
    THPPlayerSetVolume(0, (time * 1000) / 60);
}

int MoviePlayer::getFrameNumber()
{
    return THPPlayerGetTotalFrame();
}