#ifndef _JSYSTEM_JUT_JUTEXCEPTION_H
#define _JSYSTEM_JUT_JUTEXCEPTION_H

#include "dolphin/os.h"
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/Jutility/JUTGamePad.h"
#include "types.h"

struct JUTConsole;
struct JUTDirectPrint;

typedef void JUTErrorHandler(OSError, OSContext *, u32, u32);

/**
 * @size{0xA4}
 */
struct JUTException : public JKRThread
{
    enum EInfoPage
    {
    };

    struct JUTExMapFile
    {
        inline JUTExMapFile(const char *fileName)
            : m_link(this)
        {
            m_fileName = fileName;
        }
        const char *m_fileName;
        JSULink<JUTExMapFile> m_link;
    };

    /** @fabricated */
    struct ExCallbackObject
    {
        OSErrorHandler m_errorHandler; // _00
        OSError m_error;               // _04
        OSContext *m_context;          // _08
        u32 _0C;                       // _0C
        u32 _10;                       // _10
    };

    JUTException(JUTDirectPrint *); // unused/inlined

    virtual ~JUTException(); // _08 (weak)
    virtual void *run();     // _0C

    void showFloat(OSContext *);
    void showStack(OSContext *);
    void showMainInfo(unsigned short, OSContext *, unsigned long, unsigned long);
    void showMapInfo_subroutine(unsigned long, bool);
    void showGPRMap(OSContext *);
    void printDebugInfo(JUTException::EInfoPage, unsigned short, OSContext *, unsigned long, unsigned long);
    void readPad(unsigned long *, unsigned long *);
    void printContext(unsigned short, OSContext *, unsigned long, unsigned long);
    static void waitTime(long);
    void createFB();
    static OSErrorHandler setPreUserCallback(JUTErrorHandler);
    static void appendMapFile(const char *);
    static bool queryMapAddress(char *, unsigned long, long, unsigned long *, unsigned long *, char *, unsigned long, bool, bool);
    static bool queryMapAddress_single(char *, unsigned long, long, unsigned long *, unsigned long *, char *, unsigned long, bool, bool);

    static JUTException *create(JUTDirectPrint *);
    static void createConsole(void *buffer, u32 bufferSize);
    static void panic(const char * file, int line, const char * msg) {
        panic_f(file, line, "%s", msg);
    }
    static void panic_f(const char *file, int line, const char *msg, ...);
    static void errorHandler(unsigned short, OSContext *, unsigned long, unsigned long);
    static void setFPException(unsigned long);

    // unused/inlined:
    //static void panic_f_va(const char *, int, const char *, va_list_struct *);
    void showFloatSub(int, float);
    void searchPartialModule(unsigned long, unsigned long *, unsigned long *, unsigned long *, unsigned long *);
    void showGPR(OSContext *);
    void showSRR0Map(OSContext *);
    bool isEnablePad() const;
    u32 getFpscr();
    void setFpscr(unsigned long);
    static OSErrorHandler setPostUserCallback(OSErrorHandler);
    void enableFpuException();
    void disableFpuException();

    void *m_frameBuffer;            // _7C
    JUTDirectPrint *m_directPrint;  // _80
    u32 _84;                        // _84
    JUTGamePad::EPadPort m_padPort; // _88
    int _8C;                        // _8C
    int _90;                        // _90
    int _94;                        // _94
    u32 _98;                        // _98
    int _9C;                        // _9C
    void *m_stackPointer;           // _A0

    static JUTConsole *sConsole;
    static void *sConsoleBuffer;
    static size_t sConsoleBufferSize;
    static JUTException *sErrorManager;
    static OSMessageQueue sMessageQueue;
    static void *sMessageBuffer[1];
    static OSErrorHandler sPreUserCallback;
    static OSErrorHandler sPostUserCallback;
    static u32 msr;
    static u32 fpscr;
    static const char *const sCpuExpName[OS_ERROR_MAX + 1];
    static JSUList<JUTExMapFile> sMapFileList;
};

#endif