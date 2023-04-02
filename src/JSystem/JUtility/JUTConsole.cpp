#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <JSystem/JUtility/JUTConsole.h>
#include <JSystem/JUtility/JUTDbg.h>

// WIP

#define OUTPUT_NONE 0
#define OUTPUT_OSREPORT 1
#define OUTPUT_CONSOLE 2
#define OUTPUT_ALL OUTPUT_OSREPORT | OUTPUT_CONSOLE

JUTConsoleManager *JUTConsoleManager::sManager;

JUTConsole *JUTConsole::create(uint param_0, uint maxLines, JKRHeap *pHeap)
{
    JUTConsoleManager *pManager = JUTConsoleManager::sManager;

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
    u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

    JUTConsole *newConsole = new (buffer) JUTConsole(param_0, maxLines, false);
    newConsole->mBuf = (u8 *)buffer + sizeof(JUTConsole);
    newConsole->clear();

    pManager->appendConsole(newConsole);
    return newConsole;
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
    JUT_ASSERT(563, console!=0);
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
    JUT_ASSERT(924, sManager == 0);
    if (pHeap == nullptr)
    {
        pHeap = JKRGetCurrentHeap();
    }
    return sManager = new (pHeap, 0) JUTConsoleManager();
}

void JUTConsoleManager::appendConsole(JUTConsole *console)
{
    JUT_ASSERT(961, sManager != 0 && console != 0);
    // maybe local soLink_
    JUT_ASSERT(964, soLink_.Find( console ) == soLink_.end());
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
        mDirectConsole->doDraw(JUTConsole::UNK_TYPE2);
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