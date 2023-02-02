#include "dolphin/OS.h"
#include "dolphin/stl.h"

#include "JSystem/JAudio/JASReport.h"
#include "JSystem/JAudio/JASMutex.h"

static OSMutex sMutex;
static char *sBuffer;
static int sLineMax;
static int sLineCount;
static int sTop;

void JASReport(const char * str, ...) {
    if(sBuffer) {
        va_list vl;
        va_start(vl, str);
        JASMutexLock mutexLock(&sMutex);
        vsnprintf(sBuffer + (sTop * 64), 64, str, vl);
        va_end(vl);

        sTop++;
        if (sTop >= sLineMax)
            sTop = 0;

        if (sLineCount < sLineMax)
            sLineCount++;
    }
}