#ifndef JUTASSERT_H
#define JUTASSERT_H

#include <stdio.h>
#include <dolphin/os.h>
#include "types.h"

namespace JUTAssertion
{
  void create();
  void flushMessage();
  void flushMessage_dbPrint();
  u32 getSDevice(void);

  void showAssert_f(u32 device, char const *file, int line, char const *errormsg, ...);
  inline void showAssert(u32 device, char const *file, int line, char const *errormsg) {
    showAssert_f(device, file, line, "%s", errormsg);
  }

  void setConfirmMessage(u32 device, char *file, int line, bool condition, const char *msg);
  void setWarningMessage_f(u32 device, char *file, int line, char const *, ...);
  inline void setWarningMessage(u32 device, char *file, int line, char const *errormsg) {
    setWarningMessage_f(device, file, line, "%s", errormsg);
  }

  void setLogMessage_f(u32 device, char *file, int line, char const *fmt, ...);
  extern "C"
  {
    void showAssert_f_va(u32 device, const char *file, int line, const char *fmt, va_list vl);
    void setWarningMessage_f_va(u32 device, char *file, int line, const char *fmt, va_list vl);
    void setLogMessage_f_va(u32 device, char *file, int line, const char *fmt, va_list vl);
  }
}



#endif
