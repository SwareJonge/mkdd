#include <string.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h"
#include <JSystem/JKernel/JKRExpHeap.h>
#include <JSystem/JUtility/JUTDbPrint.h>
#include <JSystem/JUtility/JUTProcBar.h>

#include "Kaneshige/SysDebug.h"


const GXColor SysDebug::cUserBarColorTable[8] = {
    { 255, 255, 255, 255},
    { 255,   0,   0, 255},
    {   0, 255,   0, 255},
    {   0,   0, 255, 255},
    { 128,   0,   0, 255},
    {   0, 128,   0, 255},
    {   0,   0, 128, 255},
    { 128, 128, 128, 255}
};

SysDebug *SysDebug::sManager;

SysDebug *SysDebug::createManager() {
    if(sManager == nullptr)
        sManager = new SysDebug();
    return sManager;
}

SysDebug::SysDebug() : mGamePad(JUTGamePad::Port2) {
    mDrawPage = false;
    mPageNo = 0;

    if(OSGetConsoleSimulatedMemSize() >= 0x3000000) {
        void *mem = JKRHeap::getRootHeap()->alloc(0xd00090, -4);
        mDebugHeap = JKRExpHeap::create(mem, 0xd00090, nullptr, false);
    }
    else {
        mDebugHeap = nullptr;
    }
    mBackupHeap = nullptr;
    mSelectedHeap = JKRHeap::getRootHeap();
    mGroupIdx = 0;
    clrAllUserTimeLabel();
    mDrawPosX = 16;
    mDrawPosY = 40;
    mCharWidth = 12;
    mCharHeight = 12;
    mDrawUserTime = false;
    mProcTimeType = 0;
    mProcTimeFrames = 0;
    mCPUTime = 0.0f;
    mCPUTimeMax = 0.0f;
    mAllTime = 0.0f;
    mAllTimeMax = 0.0f;
    mBufferIdx = 0;
    mDrawProcTime = false;
    mCPUTimes = nullptr;
    mGPUTimes = nullptr;
    mUsrPageNo = 0;
    mOrtho = nullptr;

    if(mDebugHeap) {
        beginDebugHeap();
        JUTResFont *font = new JUTResFont(&JUTResFONT_Ascfont_fix12, mDebugHeap);
        JUTDbPrint::getManager()->changeFont(font);
        mOrtho = new J2DOrthoGraph(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
        JUTColor color(0, 0, 0, 128);
        mOrtho->setColor(color);
        mCPUTimes = new f32[60];
        mGPUTimes = new f32[60];

        for(int i  =0 ; i < 60; i++) {
            mCPUTimes[i] = 0.0f;
            mGPUTimes[i] = 0.0f;
        }
        endDebugHeap();
    }
}

void SysDebug::ctrlDebugMode() {
    if(mDrawPage) {
        JUTProcBar::getManager()->setVisible(true);
        f32 cpuFrameTime = JUTProcBar::getManager()->getCpuCost() / 16666.666f;
        f32 gpuFrameTime = JUTProcBar::getManager()->getGpCost() / 16666.666f;

        if(mCPUTimes) {
            mCPUTimes[mBufferIdx] = cpuFrameTime;
            mGPUTimes[mBufferIdx] = gpuFrameTime;
            mBufferIdx++;
            if (mBufferIdx >= 60)
                mDrawProcTime = true;
            
            mBufferIdx = mBufferIdx % 60;   
        }

        if(mPageNo >= 0 && mPageNo <= 2) {            
            if(mGamePad.testTrigger(JUTGamePad::L)) {
                if(mPageNo > 0)
                    mPageNo--;
                else
                    mPageNo = 2;
            }
            if(mGamePad.testTrigger(JUTGamePad::R)) {
                if(mPageNo < 2)
                    mPageNo++;
                else
                    mPageNo = 0;
            }
        }
        
        switch (mPageNo)
        {
        case 0:
        {
            if(mGamePad.testTrigger(JUTGamePad::Y)) {
                if(mDrawUserTime) 
                    mDrawUserTime = false;                
                else
                    mDrawUserTime = true;                
            }
            switch(mProcTimeType) {
                case 0:
                {
                    if (mGamePad.testTrigger(JUTGamePad::CSTICK_DOWN))
                        mProcTimeType = 1;     
                    break;               
                }
                case 1:
                {
                    mProcTimeFrames++;

                    if(cpuFrameTime > mCPUTimeMax)
                        mCPUTimeMax = cpuFrameTime;                    
                    mCPUTime += cpuFrameTime;

                    if(gpuFrameTime > mAllTimeMax)
                        mAllTimeMax = gpuFrameTime;
                    mAllTime += gpuFrameTime;

                    if (mGamePad.testTrigger(JUTGamePad::CSTICK_DOWN))
                        mProcTimeType = 2;
                    break;
                }
                case 2:
                {
                    if (mGamePad.testTrigger(JUTGamePad::CSTICK_DOWN)) {
                        mProcTimeType = 0;
                        mProcTimeFrames = 0;
                        mCPUTime = 0.0f;
                        mCPUTimeMax = 0.0f;
                        mAllTime = 0.0f;
                        mAllTimeMax = 0.0f;
                    }                        
                    break;
                }
            }
            break;
        }
        case 1:
        {
            // very optimal code incoming
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_UP)) {
                
                if (mSelectedHeap->getHeapTree().getPrevChild() != mSelectedHeap->getHeapTree().getEndChild()) {
                    if (mSelectedHeap->getHeapTree().getPrevChild()->getNumChildren() != 0)
                        mSelectedHeap = mSelectedHeap->getHeapTree().getPrevChild()->getLastChild()->getObject();
                    else
                        mSelectedHeap = mSelectedHeap->getHeapTree().getPrevChild()->getObject(); 
                }
                else if (mSelectedHeap->getHeapTree().getParent())
                    mSelectedHeap = mSelectedHeap->getHeapTree().getParent()->getObject();
            }
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_DOWN)) {
                if(mSelectedHeap->getHeapTree().getNumChildren() != 0)
                    mSelectedHeap = mSelectedHeap->getHeapTree().getFirstChild()->getObject();
                else if (mSelectedHeap->getHeapTree().getNextChild() != mSelectedHeap->getHeapTree().getEndChild())
                    mSelectedHeap = mSelectedHeap->getHeapTree().getNextChild()->getObject();
                else if (mSelectedHeap->getHeapTree().getParent()->getNextChild() != mSelectedHeap->getHeapTree().getParent()->getEndChild())
                    mSelectedHeap = mSelectedHeap->getHeapTree().getParent()->getNextChild()->getObject();
            }

            if(mGamePad.testTrigger(JUTGamePad::A))
                mPageNo = 3;

            if(mGamePad.testButton(JUTGamePad::START))
                JKRHeap::getSystemHeap()->dump();            
            break;
        }
        case 3:
        {
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_LEFT)) {
                mGroupIdx -= 10;
                if(mGroupIdx < 0)
                    mGroupIdx += 30;
            }
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_RIGHT)) {
                mGroupIdx += 10;
                if(mGroupIdx >= 30)
                    mGroupIdx -= 30;
            }

            if(mGamePad.testTrigger(JUTGamePad::B))
                mPageNo = 1;
            break;
        }
        case 2:
        {
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_LEFT) && mUsrPageNo > 0)
                mUsrPageNo--;
            if(mGamePad.testTrigger(JUTGamePad::CSTICK_RIGHT))
                mUsrPageNo++;
            break;
        }
        }
    }

    if (mGamePad.testButton(JUTGamePad::Z) && mGamePad.testTrigger(JUTGamePad::R)  ){
        if(!mDrawPage) {
            mDrawPage = true;
        }
        else {
            mDrawPage = false;
        }
    }
}

void SysDebug::setHeapGroup(const char *name, JKRHeap *pHeap) {
    if(pHeap == nullptr)
        pHeap = JKRGetCurrentHeap();

    if(pHeap && pHeap->getHeapType() == 'EXPH') {
        HeapInfo *info = findHeapInfo(pHeap);
        if(info)
            info->setGroup(name);
    }
}

void SysDebug::setDefaultHeapGroup(JKRHeap *pHeap) {
    if(pHeap == nullptr) {
        pHeap = JKRGetCurrentHeap();
    }

    if(pHeap && pHeap->getHeapType() == 'EXPH') {
        HeapInfo *info = findHeapInfo(pHeap);
        if(info)
            info->setDefaultGroup();
    }
}

void SysDebug::setUserTimeLabel(int idx, const char *label) {
    if(idx >= 0 && idx < 8)
        mUserTimeLabels[idx] = label;
}

void SysDebug::clrAllUserTimeLabel() {
    for(int i = 0; i < 8; i++)
        mUserTimeLabels[i] = nullptr;
}

void SysDebug::beginUserTime(int idx) {
    if(!mDrawUserTime)
        return;    
    
    if(idx >= 0 && idx < 8) {
        const GXColor &color = cUserBarColorTable[idx];
        JUTProcBar::getManager()->userStart(idx, color.r, color.g, color.b);
    }
}

void SysDebug::endUserTime(int idx) {
    if(!mDrawUserTime)
        return;    
    
    if(idx >= 0 && idx < 8)
        JUTProcBar::getManager()->userEnd(idx);
    //JUTProcBar::userEnd(int) {}
    //JUTProcBar::CTime::end() {}
}

void SysDebug::draw() {

    if(!mDrawPage)
        return;
    
    if(mOrtho) {
        mOrtho->setPort();
        mOrtho->fillBox(0.0f, 0.0f, 640.0f, 480.0f);
    }

    switch (mPageNo)
    {
    case 0:
        drawProcTime();
        break;
    case 1:
        drawMemMap();
        break;
    case 2:
        drawUsrPage();
        break;
    case 3:
        drawHeapSpec();
        break;    
    }
}

void SysDebug::drawProcTime() {
    f32 cpuFrameTime = JUTProcBar::getManager()->getCpuCost() / 16666.666f;
    f32 gpuFrameTime = JUTProcBar::getManager()->getGpCost() / 16666.666f;


    f32 cpuTimesSum = 0.0f;
    f32 allTimeSum = 0.0f;

    if(mCPUTimes && mDrawProcTime) {
        f32 cpuTimes = 0.0f;
        f32 allTimes = 0.0f;
        for(int i = 0; i < 60; i++) {
            cpuTimes += mCPUTimes[i];
            allTimes += mGPUTimes[i];
        }
        cpuTimesSum = cpuTimes / 60;
        allTimeSum = allTimes / 60;
    }

    JUTReport(mDrawPosX, mDrawPosY, "PROC TIME");
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 5, "CPU:%6.3f A%6.3f", cpuFrameTime, cpuTimesSum);
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 6, "ALL:%6.3f A%6.3f", gpuFrameTime, allTimeSum);

    for(int i = 0; i < 8; i++) {
        f32 userFrameTime = (JUTProcBar::getManager()->getUserCost(i) / 16666.666f);
        if(userFrameTime != 0.0f) {
            if(mUserTimeLabels[i]) {
                JUTReport(mDrawPosX, mDrawPosY + mCharHeight * (i + 8), "USR%1d:%6.3f[%s]", i, userFrameTime, mUserTimeLabels[i]);
            }
            else {
                JUTReport(mDrawPosX, mDrawPosY + mCharHeight * (i + 13),"USR%1d:%6.3f", i, userFrameTime);                
            }
        }
    }


    char *type = nullptr;
    switch(mProcTimeType) {
    case 0:
        type = "CLEAR  ";
        break;
    case 1:
        type = "MEASURE";
        break;
    case 2:
        type = "STOP   ";
        break;
    }
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 16, "WATCH %s:%d", type, mProcTimeFrames);
    f32 cpuTimeAvg = 0.0f;
    f32 allTimeAvg = 0.0f;
    if (mProcTimeFrames) {
        cpuTimeAvg = mCPUTime / mProcTimeFrames;
        allTimeAvg = mAllTime / mProcTimeFrames;
    }
    //JUTProcBar::getUserCost(int) {}
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 17, "       AVG    MAX");
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 18, "CPU: %6.3f %6.3f", cpuTimeAvg, mCPUTimeMax);
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 19, "ALL: %6.3f %6.3f", allTimeAvg, mAllTimeMax);
}

char *SysDebug::getHeapLabel(JKRHeap *pHeap) {
    char* label = nullptr;
    HeapInfo *info = findHeapInfo(pHeap);
    if(info)
        label = info->mHeapLabel;
    
    if(label == nullptr) {
        if(pHeap == JKRHeap::getRootHeap()) {
            return "ROOT    ";
        }
        else if(pHeap == JKRHeap::getSystemHeap()) {
            return "JSYSTEM ";
        }
        else if(pHeap == mDebugHeap) {
            return "DEBUG   ";
        }     
        label = "--------";   
    }

    return label;

}

char *SysDebug::getHeapTypeName(JKRHeap *pHeap) {
    switch (pHeap->getHeapType())
    {
    case 'EXPH':
        return "EXP";
    case 'ASTH':
        return "AST";
    case 'SLID':
        return "SLD";
    case 'STDH':
        return "STD";
    default:
        return "---";
    }
}

void SysDebug::printHeap(s32 x, s32 y, JKRHeap *pHeap) {
    char *label = getHeapLabel(pHeap);
    char *typeName = getHeapTypeName(pHeap);

    s32 size = (s32)pHeap->getEndAddr() - (s32)pHeap->getStartAddr();
    s32 free = pHeap->getTotalFreeSize();
   
    JUTReport(x, y, "%s[%s]:%08X-%08X:%5dK/%5dK", label, typeName, pHeap->getStartAddr(), pHeap->getEndAddr(), sizeKByte(free), sizeKByte(size));
}

void SysDebug::printHeapTree(JKRHeap *pHeap, s32 xOffset, s32 *pLineNo) {
    int w = mCharWidth * 2;
    int posX = mDrawPosX + xOffset * w;
    int posY = mDrawPosY + mCharHeight * *pLineNo;

    if(pHeap == mSelectedHeap)
        JUTReport(posX - mCharWidth, posY, "*");

    printHeap(posX, posY, pHeap);
    (*pLineNo)++;

    if(pHeap->getHeapTree().getNumChildren() != 0) {
        for (JSUTreeIterator<JKRHeap> it = pHeap->getHeapTree().getFirstChild(); it != pHeap->getHeapTree().getEndChild(); ++it)
            printHeapTree(it.getObject(), xOffset + 1, pLineNo);
    }
}

void SysDebug::drawMemMap() {
    s32 lineNo = 0;
    JUTReport(mDrawPosX, mDrawPosY, "MEMORY MAP(FREE/SIZE)");
    lineNo++;
    s32 size = (s32)JKRHeap::getCodeEnd() - (s32)JKRHeap::getCodeStart();
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * lineNo, "CODE         :%08X-%08X:      /%5dK", JKRHeap::getCodeStart(), JKRHeap::getCodeEnd(), sizeKByte(size));
    lineNo++;
    printHeapTree(JKRHeap::getRootHeap(), 0, &lineNo);
}

void SysDebug::drawUsrPage() {
    JUTReport(mDrawPosX, mDrawPosY, "USR PAGE:%d", mUsrPageNo);

    s16 pageIdx = 0;
    for(JSUListIterator<SysDbUsrPage> it(mUsrPageList.getFirst()); it != mUsrPageList.getEnd(); pageIdx++, ++it) {
        if(pageIdx == mUsrPageNo) {
            it->draw();
            break;
        }
    }
}

void SysDebug::drawHeapSpec() 
{
    JKRHeap *heap = mSelectedHeap;

    s32 size =  (s32)heap->getEndAddr() - (s32)heap->getStartAddr(); 
    s32 maxFree = heap->getFreeSize();
    s32 totalFree = heap->getTotalFreeSize();

    JUTReport(mDrawPosX, mDrawPosY, "HEAP SPEC");
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 2, "NAME      :%s", getHeapLabel(heap));
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 3, "TYPE      :%s", getHeapTypeName(heap));
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 4, "ADDRESS   :%08X-%08X", heap->getStartAddr(), heap->getEndAddr());
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 5, "SIZE      :%08X(%5dK)", size, sizeKByte(size));
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 6, "MAX   FREE:%08X(%5dK)", maxFree, sizeKByte(maxFree));
    JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 7, "TOTAL FREE:%08X(%5dK)", totalFree, sizeKByte(totalFree));

    if(heap && heap->getHeapType() == 'EXPH') {
        HeapInfo *info = findHeapInfo(heap);
        if(info) {
            JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 10, "EXP HEAP GROUP:");
            JKRExpHeap *expHeap = (JKRExpHeap *)heap;
            s32 usedSize = expHeap->getUsedSize(0xff);
            JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 11, "  :%08XH(%5dKB):%s", usedSize, sizeKByte(usedSize), "UNKNOWN");
        
            for(int i = 0; i < 10; i++) {
                int groupID = mGroupIdx + i;
                s32 usedSize = expHeap->getUsedSize(groupID);
                const char *name = info->mGroupName[groupID];
                if(name == nullptr)
                    name = "UNUSED";
                
                JUTReport(mDrawPosX, mDrawPosY + mCharHeight * (i + 12), "%2d:%08XH(%5dKB):%s",groupID, usedSize, sizeKByte(usedSize), name);
                
            }
        }
        else {
            JUTReport(mDrawPosX, mDrawPosY + mCharHeight * 10, "NO HEAP INFO");
        }
    } 

}

bool SysDebug::beginDebugHeap() {
    bool ret = false;
    if(mDebugHeap) {
        mBackupHeap = mDebugHeap->becomeCurrentHeap();;
        ret = true;
    }

    return ret;
}

void SysDebug::endDebugHeap() {
    if(mBackupHeap == nullptr)
        return;
    
    mBackupHeap->becomeCurrentHeap();
    mBackupHeap = nullptr;
}

void SysDebug::createHeapInfo(JKRHeap *pHeap, char *name) {
    if(mDebugHeap == nullptr)
        return;

    if(findHeapInfo(pHeap) == nullptr) {
        HeapInfo *info = new (mDebugHeap, 0) HeapInfo(pHeap, name);
        if(info)
            appendInfo(info);
    }
    mSelectedHeap = pHeap;
}

void SysDebug::destroyHeapInfo(JKRHeap *pHeap) {
    if(mSelectedHeap == pHeap)
        mSelectedHeap = JKRHeap::getRootHeap();

    HeapInfo *info = findHeapInfo(pHeap);
    if(info)
        delete info;
}

SysDebug::HeapInfo *SysDebug::findHeapInfo(JKRHeap *pHeap) {
    HeapInfo *ret = nullptr;
    for(JSUListIterator<HeapInfo> it(mInfoList.getFirst()); it != mInfoList.getEnd(); ++it) {
        if(it->mHeap == pHeap) {
            ret = it.getObject();
            break;
        }           
    }
    return ret;
}

SysDebug::HeapInfo::HeapInfo(JKRHeap *pHeap, char *label) : mLink(this) {
    mHeap = pHeap;
    clrGroup();


    for(int i = 0; i < 8; i++)
        mHeapLabel[i] = label[i];
    mTerminator = '\0';

    for(int i = strlen(mHeapLabel) ; i < 8; i++)
        mHeapLabel[i] = ' '; // if string has less than 8 characters, fill it up with spaces
}



SysDebug::HeapInfo::~HeapInfo() {
    if(GetSysDebug())
        GetSysDebug()->removeInfo(this);
}

void SysDebug::HeapInfo::clrGroup() {
    mGroupID = 0;
    for(s16 i = 0; i < 30; i++) {
        mGroupName[i] = nullptr;
    }
}

void SysDebug::HeapInfo::setGroup(const char *name) {
    int newId = searchSameName(name);
    if(newId < 0 && mGroupID < 30) {
        mGroupName[mGroupID] = name;
        newId = mGroupID;
        mGroupID++;
    }

    if(newId >= 0) {
        mHeap->changeGroupID(newId);
    }
}

int SysDebug::HeapInfo::searchSameName(const char *name) {
    int ret = -1;

    if(name == nullptr)
        return -1;
    
    for(int i = 0; i < 30; i++) {
        if(mGroupName[i] && strcmp(mGroupName[i], name) == 0) {
            ret = i;
            break;
        }
    }
    return ret;
}

bool SysDebug::appendPage(SysDbUsrPage *page) {
    mUsrPageList.append(&page->mLink);
    return true;
}

void SysDebug::removePage(SysDbUsrPage *page) { mUsrPageList.remove(&page->mLink); }

void SysDebug::checkInvalidFloat(f32 f, char *name) {
#line 1267
    JUT_ASSERT_F(!isnan(f), "%s:Float:nan", name);
    JUT_ASSERT_F(!isinf(f), "%s:Float:inf", name);
}

void SysDebug::checkInvalidFloat(f32 f, u32 id) {
    JUT_ASSERT_F(!isnan(f), "%d:Float:nan", id);
    JUT_ASSERT_F(!isinf(f), "%d:Float:inf", id);
}

void SysDebug::checkInvalidMatrix(Mtx m, char *name) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++) {
            f32 f = m[i][j];
#line 1287
            JUT_ASSERT_F(!isnan(f), "%s:MTX[%d][%d]:nan", name, i, j);
            JUT_ASSERT_F(!isinf(f), "%s:MTX[%d][%d]:inf", name, i, j);
        }
    }
}

void SysDebug::checkInvalidMatrix(Mtx m, u32 id) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++) {
            f32 f = m[i][j];
#line 1300
            JUT_ASSERT_F(!isnan(f), "%d:MTX[%d][%d]:nan", id, i, j);
            JUT_ASSERT_F(!isinf(f), "%d:MTX[%d][%d]:inf", id, i, j);
        }
    }
}

void SysDebug::checkInvalidVector(Vec *v, char *name) 
{
#line 1309
    JUT_ASSERT_F(!isnan(v->x), "%s:VEC.X:nan", name);
    JUT_ASSERT_F(!isnan(v->y), "%s:VEC.Y:nan", name);
    JUT_ASSERT_F(!isnan(v->z), "%s:VEC.Z:nan", name);

    JUT_ASSERT_F(!isinf(v->x), "%s:VEC.X:inf", name);
    JUT_ASSERT_F(!isinf(v->y), "%s:VEC.Y:inf", name);
    JUT_ASSERT_F(!isinf(v->z), "%s:VEC.Z:inf", name);
}

void SysDebug::checkInvalidVector(Vec *v, u32 id) 
{
    JUT_ASSERT_F(!isnan(v->x), "%d:VEC.X:nan", id);
    JUT_ASSERT_F(!isnan(v->y), "%d:VEC.Y:nan", id);
    JUT_ASSERT_F(!isnan(v->z), "%d:VEC.Z:nan", id);

    JUT_ASSERT_F(!isinf(v->x), "%d:VEC.X:inf", id);
    JUT_ASSERT_F(!isinf(v->y), "%d:VEC.Y:inf", id);
    JUT_ASSERT_F(!isinf(v->z), "%d:VEC.Z:inf", id);
}

SysDbUsrPage::SysDbUsrPage() : mLink(this) {}

SysDbUsrPage::~SysDbUsrPage() { GetSysDebug()->removePage(this); }
