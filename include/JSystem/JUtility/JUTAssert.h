#ifndef JUTASSERT_H
#define JUTASSERT_H

#include "dolphin/OS.h"
#include "types.h"

class JUTAssertion
{
public:
  static void flushMessage();
  static void flushMessage_dbPrint();
  static void create();
  static u32 getSDevice(void);
  static void showAssert_f(u32 device, char const *file, int line, char const *errormsg, ...);
  static void showAssert(u32 device, char const *file, int line, char const *errormsg) {
    showAssert_f(device, file, line, "%s", errormsg);
  }
  static void setConfirmMessage(u32 device, char *file, int line, bool condition, const char *msg);
};

#endif