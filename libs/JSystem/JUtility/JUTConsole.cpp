#include <cstdio>

#include <dolphin/os.h>
#include <dolphin/vi.h>
#include "JSystem/J2D.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTVideo.h"

#include "JSystem/JUtility/JUTDbg.h"

// WIP

#define OUTPUT_NONE 0
#define OUTPUT_OSREPORT 1
#define OUTPUT_CONSOLE 2
#define OUTPUT_ALL OUTPUT_OSREPORT | OUTPUT_CONSOLE

JUTConsoleManager *JUTConsoleManager::sManager;

JUTConsole *JUTConsole::create(uint param_0, uint maxLines, JKRHeap *pHeap)
{
    JUTConsoleManager *pManager = JUTConsoleManager::sManager;
#line 33
    JUT_ASSERT(pManager != 0);

    u8 *buffer = (u8 *)JKRHeap::alloc(getObjectSizeFromBufferSize(param_0, maxLines), 0, pHeap);

    JUTConsole *newConsole = new (buffer) JUTConsole(param_0, maxLines, true);
    newConsole->mBuf = buffer + sizeof(JUTConsole);
    newConsole->clear();

    pManager->appendConsole(newConsole);
    return newConsole;
}

JUTConsole *JUTConsole::create(uint param_0, void *buffer, u32 bufferSize)
{
    JUTConsoleManager *pManager = JUTConsoleManager::sManager;
#line 59
    JUT_ASSERT(pManager != 0);
#line 63
    JUT_ASSERT(( (u32)buffer & 0x3 ) == 0);
    u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

    JUTConsole *newConsole = new (buffer) JUTConsole(param_0, maxLines, false);
    newConsole->mBuf = (u8 *)buffer + sizeof(JUTConsole);
    newConsole->clear();

    pManager->appendConsole(newConsole);
    return newConsole;
}

// unused
void JUTConsole::destroy(JUTConsole *console) {
    JUT_ASSERT(console != 0);
    delete console;
}

JUTConsole::JUTConsole(uint p1, uint maxLines, bool p3)
{
    field_0x2c = p3;
    field_0x20 = p1;
    mMaxLines = maxLines;

    mPositionX = 30;
    mPositionY = 50;
    mHeight = 20;

    if (mHeight > mMaxLines)
    {
        mHeight = mMaxLines;
    }

    mFont = NULL;
    mIsVisible = true;
    field_0x69 = false;
    field_0x6a = false;
    field_0x6b = false;
    mOutput = 1;

    field_0x5c.set(0, 0, 0, 100);
    field_0x60.set(0, 0, 0, 230);
    field_0x64 = 8;
}

JUTConsole::~JUTConsole() {
    JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getObjectSizeFromBufferSize(unsigned int param_0, unsigned int maxLines)
{
    return (param_0 + 2) * maxLines + sizeof(JUTConsole);
}

size_t JUTConsole::getLineFromObjectSize(u32 bufferSize, unsigned int param_1)
{
    return (bufferSize - sizeof(JUTConsole)) / (param_1 + 2);
}

void JUTConsole::clear()
{
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x3c = 0;

    for (int i = 0; i < mMaxLines; i++)
    {
        setLineAttr(i, 0);
    }
    setLineAttr(0, -1);
    *getLinePtr(0) = 0;
}

void JUTConsole::doDraw(JUTConsole::EConsoleType consoleType) const
{
    f32 font_yOffset;
    s32 changeLine_1;
    s32 changeLine_2;

    if (mIsVisible && (mFont != NULL || consoleType == CONSOLE_TYPE_2))
    {
        if (mHeight != 0)
        {
            bool temp_r30 = consoleType == CONSOLE_TYPE_0;
            font_yOffset = 2.0f + mFontSizeY;

            if (consoleType != CONSOLE_TYPE_2)
            {
                if (JUTVideo::getManager() == NULL)
                {
                    J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
                    ortho.setPort();
                }
                else
                {
                    J2DOrthoGraph ortho(0.0f, 0.0f, JUTVideo::getManager()->getFbWidth(),
                                        JUTVideo::getManager()->getEfbHeight(), -1.0f, 1.0f);
                    ortho.setPort();
                }

                const JUtility::TColor *color;
                if (temp_r30)
                {
                    color = &field_0x60;
                }
                else
                {
                    color = &field_0x5c;
                }

                J2DFillBox(mPositionX - 2, (int)(mPositionY - font_yOffset),
                           (int)((mFontSizeX * field_0x20) + 4.0f), (int)(font_yOffset * mHeight),
                           *color);
                mFont->setGX();

                if (temp_r30)
                {
                    s32 s = (diffIndex(field_0x30, field_0x38) - mHeight) + 1;
                    if (s <= 0)
                    {
                        mFont->setCharColor(JUtility::TColor(255, 255, 255, 255));
                    }
                    else if (field_0x30 == field_0x34)
                    {
                        mFont->setCharColor(JUtility::TColor(255, 230, 230, 255));
                    }
                    else
                    {
                        mFont->setCharColor(JUtility::TColor(230, 230, 255, 255));
                    }
                }
                else
                {
                    mFont->setCharColor(JUtility::TColor(230, 230, 230, 255));
                }
            }
            else
            {
                JUTDirectPrint::getManager()->erase(mPositionX - 3, mPositionY - 2,
                                                    (field_0x20 * 6) + 6,
                                                    (int)(font_yOffset * mHeight) + 4);
                JUTDirectPrint::getManager()->setCharColor(JUtility::TColor(255, 255, 255, 255));
            }

            char *linePtr;
            s32 curLine = field_0x30;
            s32 yFactor = 0;

            do
            {
                linePtr = (char *)getLinePtr(curLine);

                if ((u8)linePtr[-1] != NULL)
                {
                    if (consoleType != CONSOLE_TYPE_2)
                    {
                        mFont->drawString_scale(mPositionX, ((yFactor * font_yOffset) + mPositionY),
                                                mFontSizeX, mFontSizeY, linePtr, true);
                    }
                    else
                    {
                        JUTDirectPrint::getManager()->drawString(
                            mPositionX, ((yFactor * font_yOffset) + mPositionY), linePtr);
                    }

                    changeLine_1 = curLine + 1;
                    yFactor += 1;
                    changeLine_2 = changeLine_1 & ~(-((s32)mMaxLines <= (s32)changeLine_1));
                    curLine = changeLine_2;
                }
                else
                {
                    break;
                }
            } while (yFactor < mHeight && changeLine_2 != field_0x34);
        }
    }
}

void JUTConsole::print_f(char const *text, ...)
{
    va_list args;
    va_start(args, text);
    JUTConsole_print_f_va_(this, text, args);
    va_end(args);
}

void JUTConsole_print_f_va_(JUTConsole *console, const char *text, va_list args)
{
    char buf[1024];
#line 563
    JUT_ASSERT(console!=0);
    vsnprintf(buf, sizeof(buf), text, args);
    console->print(buf);
}

void JUTConsole::scroll(int scrollAmnt)
{
    if (scrollAmnt < 0)
    {
        int diff = diffIndex(field_0x34, field_0x30);
        if (scrollAmnt < -diff)
        {
            scrollAmnt = -diff;
        }
    }
    else
    {
        if (scrollAmnt > 0)
        {
            int diff = diffIndex(field_0x34, field_0x38);
            if (diff + 1 <= mHeight)
            {
                scrollAmnt = 0;
            }
            else
            {
                diff = diffIndex(field_0x30, field_0x38);
                if (scrollAmnt > (int)(diff - mHeight) + 1)
                {
                    scrollAmnt = (int)(diff - mHeight) + 1;
                }
            }
        }
    }

    field_0x30 += scrollAmnt;
    if (field_0x30 < 0)
    {
        field_0x30 += mMaxLines;
    }

    if (field_0x30 >= mMaxLines)
    {
        field_0x30 -= mMaxLines;
    }
}

int JUTConsole::getUsedLine() const
{
    return diffIndex(field_0x34, field_0x38);
}

int JUTConsole::getLineOffset() const
{
    return diffIndex(field_0x34, field_0x30);
}

JUTConsoleManager::JUTConsoleManager()
{
    mActiveConsole = nullptr;
    mDirectConsole = nullptr;
}

JUTConsoleManager *JUTConsoleManager::createManager(JKRHeap *pHeap)
{
#line 563
    JUT_ASSERT(924, sManager == 0);
    if (pHeap == nullptr)
    {
        pHeap = JKRGetCurrentHeap();
    }
    return sManager = new (pHeap, 0) JUTConsoleManager();
}

void JUTConsoleManager::appendConsole(JUTConsole *console)
{
#line 961
    JUT_ASSERT(sManager != 0 && console != 0);

    // not sure why this asser was 3 lines later
    JUT_ASSERT(soLink_.Find( console ) == soLink_.end());
    soLink_.Push_back(console);
    if (mActiveConsole == nullptr)
    {
        mActiveConsole = console;
    }
}

void JUTConsoleManager::drawDirect(bool waitRetrace) const {
    if(mDirectConsole != nullptr) {
        if(waitRetrace) {
            BOOL interrupt = OSEnableInterrupts();
            u32 retrace_count = VIGetRetraceCount();
            u32 new_count;
            do
            {
                new_count = VIGetRetraceCount();
            } while (retrace_count == new_count);
            OSRestoreInterrupts(interrupt);
        }
        mDirectConsole->doDraw(JUTConsole::CONSOLE_TYPE_2);
    }
}

void JUTConsoleManager::setDirectConsole(JUTConsole *console) {
    if (mDirectConsole != nullptr) {
        appendConsole(mDirectConsole);
    }
    if(console != nullptr) {
        removeConsole(console);
    }
    mDirectConsole = console;
}

static JUTConsole *sReportConsole;
static JUTConsole *sWarningConsole;

// C Functions
void JUTSetReportConsole(JUTConsole *console) {
    sReportConsole = console;
}

JUTConsole *JUTGetReportConsole() {
    return sReportConsole;
}

void JUTSetWarningConsole(JUTConsole *console) {
    sWarningConsole = console;
}

JUTConsole *JUTGetWarningConsole() {
    return sWarningConsole;
}

void JUTReportConsole_f_va(const char *text, va_list args) {
    char buf[256];
    if(JUTGetReportConsole() == NULL) {
        vsnprintf(buf, sizeof(buf), text, args);
        OSReport("%s", buf);
    }
    else if (JUTGetReportConsole()->getOutput() & OUTPUT_ALL) {
        vsnprintf(buf, sizeof(buf), text, args);
        JUTGetReportConsole()->print(buf);
    }
}

void JUTReportConsole_f(const char *text, ...)
{
    va_list vl;
    va_start(vl, text);
    JUTReportConsole_f_va(text, vl);
    va_end(vl);
}

void JUTReportConsole(const char *text) {
    JUTReportConsole_f("%s", text);
}

void JUTWarningConsole_f_va(const char *text, va_list args)
{
    char buf[256];
    if (JUTGetWarningConsole() == NULL)
    {
        vsnprintf(buf, sizeof(buf), text, args);
        OSReport("%s", buf);
    }
    else if (JUTGetWarningConsole()->getOutput() & OUTPUT_ALL)
    {
        vsnprintf(buf, sizeof(buf), text, args);
        JUTGetWarningConsole()->print(buf);
    }
}

void JUTWarningConsole_f(const char *text, ...)
{
    va_list vl;
    va_start(vl, text);
    JUTWarningConsole_f_va(text, vl);
    va_end(vl);
}

void JUTWarningConsole(const char *text) {
    JUTWarningConsole_f("%s", text);
}