#ifndef JUTASSERT_H
#define JUTASSERT_H

#include "types.h"

#include "Dolphin/OS.h"

class JUTAssertion
{
public:
  static void create();
  static u32 getSDevice(void);
  static void showAssert_f(u32 device, char const *file, int line, char const *errormsg, ...);
  static void showAssert(u32 device, char const *file, int line, char const *errormsg); /* {
    showAssert_f(device, file, line, "%s", errormsg);
}*/
};

#if DEBUG
#define JUT_ASSERT(LINE, COND)                                                   \
  if (!(COND))                                                                   \
  {                                                                              \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, #COND); \
    OSPanic(__FILE__, LINE, "Halt");                                             \
  }

#define JUT_ASSERT_F(LINE, COND, ...)                                                    \
  if (!(COND))                                                                           \
  {                                                                                      \
    JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__); \
    OSPanic(__FILE__, LINE, "Halt");                                                     \
  }

#define JUT_PANIC(LINE, TEXT)                                                 \
  { \
  JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, TEXT); \
  OSPanic(__FILE__, LINE, "Halt"); \
  }

#define JUT_RANGE_ASSERT(LINE, min, cur, max) \
  JUT_ASSERT_F(LINE, (((cur) >= (min)) && ((cur) < (max))) != false, "range over: %d <= " #cur "=%d < %d", (min), (cur), (max))

#define JUT_MAX_ASSERT(LINE, cur, max) \
  JUT_ASSERT_F(LINE, ((cur) < (max)), "range over: %d <= " #cur "=%d < %d", 0, (cur), (max))

#define JUT_VALIDATE(LINE, COND) \
  JUT_ASSERT_F(LINE, COND, "%s", #COND)

#else // This will be changed in the future
#define JUT_ASSERT(...)
#define JUT_ASSERT_F(...)
#define JUT_PANIC(...)
#define JUT_RANGE_ASSERT(...)
#define JUT_MAX_ASSERT(...)
#define JUT_VALIDATE(...)
#endif

#endif