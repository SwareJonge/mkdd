#ifndef _JSYSTEM_JUTILITY_JUTDBG_H
#define _JSYSTEM_JUTILITY_JUTDBG_H

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"

#if DEBUG
// Panic functions
#define JUT_PANIC(LINE, MSG) \
    JUTException::panic(__FILE__, LINE, MSG);
#define JUT_PANIC_F(LINE, ...) \
    JUTException::panic_f(__FILE__, LINE, __VA_ARGS__);
// Asserts
#define JUT_CONFIRM_MESSAGE(LINE, COND) \
  JUTAssertion::setConfirmMessage(JUTAssertion::getSDevice(), __FILE__, LINE, COND, #COND);

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

#define JUT_ASSERT_MSG(LINE, COND, MSG)                                        \
  if (!(COND))                                                                 \
  {                                                                            \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, MSG); \
    OSPanic(__FILE__, LINE, "Halt");                                           \
  }

#define JUT_MINMAX_ASSERT(LINE, min, cur, max) \
  JUT_ASSERT_F(LINE, (((cur) >= (min)) && ((cur) < (max))) != false, "range over: %d <= " #cur "=%d < %d", (min), (cur), (max));

#define JUT_MAX_ASSERT(LINE, cur, max) \
  JUT_ASSERT_F(LINE, ((cur) < (max)), "range over: %d <= " #cur "=%d < %d", 0, (cur), (max));

#else // for non debug build
#define JUT_PANIC(...)
#define JUT_PANIC_F(...)
#define JUT_CONFIRM_MESSAGE(...)
#define JUT_ASSERT(...)
#define JUT_ASSERT_F(...)
#define JUT_ASSERT_MSG(...)
#define JUT_MINMAX_ASSERT(...)
#define JUT_MAX_ASSERT(...)
#endif

#endif