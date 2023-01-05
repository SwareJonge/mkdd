#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTProcBar.h"

/*
 Mostly copied from https://github.com/zeldaret/tp/blob/master/libs/JSystem/JUtility/JUTProcBar.cpp
 Modified a bit to what i believe is correct and also give slightly more descriptive names
*/

JUTProcBar *JUTProcBar::sManager;

f32 oneFrameRate = 8.0f;
f32 oneFrameRateUser = 10.0f;

JUTProcBar::JUTProcBar() {
    mVisible = true;
    mHeapBarVisible = true;
    _108 = 0;
    int height = JUTVideo::getManager()->getEfbHeight();
    if (height > 400)
    {
        mParams.mBarWidth = 2;
        mParams.mPosX = 0x27;
        mParams.mPosY = height - 0x28;
        mParams.mWidth = 0x232;
        mParams.mUserPosition = height - 0x46;
    }
    else
    {
        mParams.mBarWidth = 1;
        mParams.mPosX = 0x27;
        mParams.mPosY = height - 0x14;
        mParams.mWidth = 0x232;
        mParams.mUserPosition = height - 0x23;
    }
    _110 = 1;
    _128 = 0;
    mWatchHeap = nullptr;
}

JUTProcBar::~JUTProcBar() {
    sManager = nullptr;
}

JUTProcBar *JUTProcBar::create() {
    if (!sManager) {
        sManager = new JUTProcBar();
    }
    return sManager;
}

void JUTProcBar::destroy() {
    if (sManager) {
        delete sManager;
    }
    sManager = nullptr;
}

void JUTProcBar::clear() {
    sManager->idleStart(255, 129, 30);
    sManager->cpuStart(255, 129, 30);
    sManager->gpStart(255, 129, 30);
    sManager->wholeLoopStart(255, 129, 30);
    sManager->mCostFrame = 0;
    oneFrameRate = 8.0f;
    oneFrameRateUser = 10.0f;
}

// Matches
void JUTProcBar::bar_subroutine(int param_0, int param_1, int param_2, int param_3, int param_4,
                                int param_5, int param_6, JUtility::TColor param_7,
                                JUtility::TColor param_8)
{
    int var1 = param_6 * param_3 / param_4;
    J2DFillBox(param_0, param_1, param_5 * param_3 / param_4, param_2, param_7);
    if (var1 >= 0) {
        if (var1 < 6)
            J2DFillBox(param_0, param_1, var1, param_2, param_8);
        else
            J2DFillBox(param_0 + var1 - 6, param_1, 6.0f, param_2, param_8);
    }
}

// Matched 
// perhaps rewrite this function, kinda annoying to read
void JUTProcBar::adjustMeterLength(u32 param_0, f32 *param_1, f32 param_2, f32 param_3,
                                   int *param_4) {
    BOOL var2 = false;
    float var1 = *param_1;
    while (var1 > param_2)
    {
        if (param_0 * var1 * 20.0f / 16666.0f <= mParams.mWidth - 30.0f)
            break;

        var1 -= 0.1f;
        var2 = true;
    }

    if (var1 >= param_3)
        *param_4 = 0;
    if (var1 > param_3 - 0.2f)
        var1 = param_3;

    while (!var2 && var1 < param_3)
    {
        (*param_4)++;
        if (*param_4 < 0x1e)
            break;
        if ((param_0 * var1 * 20.0f / 16666.0f) < (mParams.mWidth - 60.0f))
            var1 += 0.2f;
        break;
    }
    *param_1 = var1;
}

void JUTProcBar::draw() {
    drawProcessBar();
    drawHeapBar();
}

// Matches the one from TP, size mismatch for MKDD https://decomp.me/scratch/YKjcF
void JUTProcBar::drawProcessBar() {
    if (mVisible)
    {
        int frameDuration = 16666; // duration in miliseconds? for how long a frame takes,
        if (JUTVideo::getManager() && ((JUTVideo::getManager()->getRenderMode()->tvMode >> 2) & 0x0f) == 1) // possibly a define
            frameDuration = 20000; // duration for PAL        

        static int cnt = 0;
        adjustMeterLength(mWholeLoop.cost, &oneFrameRate, 1.0f, 10.0f, &cnt);
        int r28 = oneFrameRate * 20.0f;
        int r27 = mParams.mBarWidth * 8;
        int r26 = mParams.mBarWidth * 2;
        int r25 = mParams.mBarWidth * 10;
        int r24 = (mParams.mWidth - 4 + r28) / r28;

        mIdle.accumePeek();
        mGp.accumePeek();
        mCpu.accumePeek();

        u32 totalTime = calcGPUTime() - mCpu.cost; // unsure of types
        u32 gpuTime = calcGPUTime();
        J2DFillBox(mParams.mPosX, mParams.mPosY, mParams.mWidth, r27, JUtility::TColor(0, 0, 50, 200));
        J2DDrawFrame(mParams.mPosX, mParams.mPosY, mParams.mWidth, r27, JUtility::TColor(50, 50, 150, 255), 6);
        if (mCostFrame > r24)
            J2DFillBox(mParams.mPosX, mParams.mPosY + r27 + 1, mParams.mWidth, 1.0f, JUtility::TColor(250, 0, 0, 200));
        else
            J2DFillBox(mParams.mPosX, mParams.mPosY + r27 + 1, mCostFrame * r28 + 2, 1.0f, JUtility::TColor(0, 250, 250, 200));
        
        int stack92 = mWholeLoop.cost * r28 / frameDuration;
        if (stack92 > mParams.mWidth)
            J2DFillBox(mParams.mPosX, mParams.mPosY, mParams.mWidth, 1.0f, JUtility::TColor(255, 100, 0, 255));
        else
            J2DFillBox(mParams.mPosX, mParams.mPosY, stack92, 1.0f, JUtility::TColor(50, 255, 0, 255));

        if (_110 == 0)
        {
            int r23 = mParams.mPosY + mParams.mBarWidth;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mGp.cost, mGp._08, JUtility::TColor(80, 255, 80, 255), JUtility::TColor(100, 255, 120, 255));
            r23 += mParams.mBarWidth * 2;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mCpu.cost, mCpu._08, JUtility::TColor(255, 80, 80, 255), JUtility::TColor(255, 100, 100, 255));
            r23 += mParams.mBarWidth * 2;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mIdle.cost, mIdle._08, JUtility::TColor(180, 180, 160, 255), JUtility::TColor(200, 200, 200, 255));
        }
        else
        {
            int r22 = mParams.mPosY + mParams.mBarWidth;
            int r21 = mParams.mPosX + 1;
            bar_subroutine(r21, r22, r26, r28, frameDuration, gpuTime, -1, JUtility::TColor(80, 255, 80, 255), JUtility::TColor(80, 255, 80, 255));
            int thingy1 = gpuTime * r28 / frameDuration + r21; // inline?
            J2DFillBox(thingy1, r22, calcBarSize(r28, frameDuration), r26, JUtility::TColor(0, 255, 0, 255));
            int r30 = mGp.cost * r28 / frameDuration + r21;
            r21 += totalTime * r28 / frameDuration;
            r22 += mParams.mBarWidth * 2;
            bar_subroutine(r21, r22, r26, r28, frameDuration, mCpu.cost, -1, JUtility::TColor(255, 80, 80, 255), JUtility::TColor(255, 80, 80, 255));
            r22 += mParams.mBarWidth * 2;
            bar_subroutine(r30, r22, r26, r28, frameDuration, mIdle.cost, -1, JUtility::TColor(180, 180, 160, 255), JUtility::TColor(180, 180, 160, 255));
        }
        for (int i = 1; i < r24; i++)
        {
            int temp2 = mParams.mPosX + i * r28 + 1;
            J2DDrawLine(temp2, mParams.mPosY + mParams.mBarWidth, temp2, mParams.mPosY + r27 - mParams.mBarWidth, (i % 5) != 0 ? JUtility::TColor(100, 100, 255, 255) : JUtility::TColor(180, 255, 255, 255), 12);
        }
        u32 temp3 = 0;
        for (int i = 0; i < 8; i++)
        {
            CTime *time = &mUsers[i];
            time->accumePeek();
            if (time->_08 > temp3)
                temp3 = time->_08;
        }
        if ((bool)temp3 == true)
        {
            static int cntUser = 0;
            adjustMeterLength(temp3, &oneFrameRateUser, 1.0f, 10.0f, &cntUser);
            int r21 = oneFrameRateUser * 20.0f;
            J2DFillBox(mParams.mPosX, mParams.mUserPosition, mParams.mWidth, r25, JUtility::TColor(0, 0, 50, 200));
            J2DDrawFrame(mParams.mPosX, mParams.mUserPosition, mParams.mWidth, r25, JUtility::TColor(50, 50, 150, 255), 6);
            for (int i = 0; i < 8; i++)
            {
                CTime *time = &mUsers[i];
                time->accumePeek();
                if (time->cost != 0 || time->_08 != 0)
                {
                    int temp4 = time->cost * r21 / frameDuration;
                    int temp5 = time->_08 * r21 / frameDuration;
                    time->cost = 0;
                    J2DFillBox(mParams.mPosX + 1, mParams.mUserPosition + mParams.mBarWidth + i * mParams.mBarWidth, temp4, mParams.mBarWidth, JUtility::TColor(time->r, time->g, time->b, 255));
                    
                    if (temp5 < 3u)
                        J2DFillBox(mParams.mPosX, mParams.mUserPosition + mParams.mBarWidth + i * mParams.mBarWidth, temp5, mParams.mBarWidth, JUtility::TColor(255, 200, 50, 255));
                    else
                        J2DFillBox(mParams.mPosX + temp5 - 3, mParams.mUserPosition + mParams.mBarWidth + i * mParams.mBarWidth, 3.0f, mParams.mBarWidth, JUtility::TColor(255, 200, 50, 255));
                }
            }

            int r22 = (mParams.mWidth - 4 + r21) / r21;

            for (int i = 1; i < r22; i++)
            {
                int temp6 = mParams.mPosX + i * r21 + 1;
                J2DDrawLine(temp6, mParams.mUserPosition + mParams.mBarWidth, temp6, mParams.mUserPosition + r25 - mParams.mBarWidth, (i % 5) != 0 ? JUtility::TColor(100, 100, 255, 255) : JUtility::TColor(180, 255, 255, 255), 12);
            }
        }
        _108 = 0;
    }
}

int addrToXPos(void *param_0, int param_1) {
    return param_1 * (((u32)param_0 - 0x80000000) / (float)JKRHeap::mMemorySize);
}

int byteToXLen(int param_0, int param_1) {
    return param_1 * (param_0 / (float)JKRHeap::mMemorySize);
}

// Size mismatch for MKDD
void heapBar(JKRHeap *param_0, int param_1, int param_2, int param_3, int param_4, int param_5) {
    int stack52 = param_1 + addrToXPos(param_0->getStartAddr(), param_4);
    int var1 = param_1 + addrToXPos(param_0->getEndAddr(), param_4);
    int stack36 = byteToXLen(param_0->getTotalFreeSize(), param_4);
    J2DFillBox(stack52, param_2 - param_5 * 2 + param_5 / 2, var1 - stack52, param_5 / 2, JUtility::TColor(255, 0, 200, 255));
    J2DFillBox(stack52, param_2 - param_5 * 2 + param_5 / 2, stack36, param_5 / 2, JUtility::TColor(255, 180, 250, 255));
}

// https://decomp.me/scratch/Mi52V
// Size Mismatch for MKDD
void JUTProcBar::drawHeapBar() {
    int var1, barWidth, userRamStart, userRamEnd, posX, posY, width; // this is probably incorrect, however it fixed the regswap for TP
    if (mHeapBarVisible) {
        posX = mParams.mPosX; // could this possibly be getUnuseUserBar__10JUTProcBarFv?
        posY = mParams.mPosY;
        barWidth = mParams.mBarWidth;
        var1 = calcBarHeight();
        width = mParams.mWidth;
        J2DFillBox(posX, posY - barWidth * 4, width, var1, JUtility::TColor(100, 0, 50, 200));
        J2DDrawFrame(posX, posY - barWidth * 4, width, var1, JUtility::TColor(100, 50, 150, 255), 6);
        int codeStart = posX + addrToXPos(JKRHeap::mCodeStart, width);
        int codeEnd = posX + addrToXPos(JKRHeap::mCodeEnd, width);
        J2DFillBox(codeStart, posY - barWidth * 4, codeEnd - codeStart, var1, JUtility::TColor(255, 50, 150, 255));
        userRamStart = posX + addrToXPos(JKRHeap::mUserRamStart, width);
        userRamEnd = posX + addrToXPos(JKRHeap::mUserRamEnd, width);
        J2DFillBox(userRamStart, posY - barWidth * 4, userRamEnd - userRamStart, var1, JUtility::TColor(0, 50, 150, 255));
        int totalFreeSize = byteToXLen(JKRHeap::sRootHeap->getTotalFreeSize(), width);
        J2DFillBox(userRamStart, posY - barWidth * 4, totalFreeSize, var1 / 2, JUtility::TColor(0, 250, 250, 255));
        if (_128 == 0)
        {
            JKRHeap *heap = mWatchHeap ? mWatchHeap : JKRHeap::sCurrentHeap;
            if (heap != JKRHeap::sSystemHeap)
                heapBar(heap, posX, posY, var1, width, var1);
        }

    }
}
