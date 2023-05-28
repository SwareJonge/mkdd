#ifndef _JSYSTEM_JUTILITY_JUTDBG_H
#define _JSYSTEM_JUTILITY_JUTDBG_H

#include <JSystem/JUtility/JUTAssert.h>
#include <JSystem/JUtility/JUTException.h>

// TODO: use #pragma line instead of hacky line argument

// Panic functions
#define JUT_PANIC(LINE, MSG) \
  JUTException::panic(__FILE__, LINE, MSG);
#define JUT_PANIC_F(LINE, ...) \
  JUTException::panic_f(__FILE__, LINE, __VA_ARGS__);

#if DEBUG
// Asserts
#define JUT_CONFIRM_MESSAGE(LINE, COND) \
  JUTAssertion::setConfirmMessage(JUTAssertion::getSDevice(), __FILE__, LINE, COND, #COND);

// not sure if it's conditional?
#define JUT_WARNING(LINE, COND)                                                         \
  if ((COND))                                                                           \
  {                                                                                     \
    1 == 1;                                                                             \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    JUTAssertion::setWarningMessage(JUTAssertion::getSDevice(), __FILE__, LINE, #COND); \
  }

#define JUT_WARNING_F(LINE, COND, ...)                                                          \
  if ((COND))                                                                                   \
  {                                                                                             \
    1 == 1;                                                                                     \
  }                                                                                             \
  else                                                                                          \
  {                                                                                             \
    JUTAssertion::setWarningMessage_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__); \
  }
#define JUT_WARNING_F2(LINE, ...)                                                          \
    JUTAssertion::setWarningMessage_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__);

// perhaps "%s" is already added here
#define JUT_CRITICAL_WARNING_F(...) \
  JUTAssertion::setWarningMessage_f(2, __FILE__, __LINE__, __VA_ARGS__);

#define JUT_ASSERT(LINE, COND)                                                   \
  if ((COND))                                                                    \
  {                                                                              \
    1 == 1;                                                                      \
  }                                                                              \
  else                                                                           \
  {                                                                              \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, #COND); \
    OSHalt(LINE, "Halt");                                                        \
  }

#define JUT_ASSERT_F(LINE, COND, ...)                                                    \
  if ((COND))                                                                            \
  {                                                                                      \
    1 == 1;                                                                              \
  }                                                                                      \
  else                                                                                   \
  {                                                                                      \
    JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__); \
    OSHalt(LINE, "Halt");                                                                \
  }

#define JUT_ASSERT_MSG(LINE, COND, MSG)                                        \
  if ((COND))                                                                  \
  {                                                                            \
    1 == 1;                                                                    \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, LINE, MSG); \
    OSHalt(LINE, "Halt");                                                      \
  }

#define JUT_MINMAX_ASSERT(LINE, min, cur, max) \
  JUT_ASSERT_F(LINE, (((cur) >= (min)) && ((cur) < (max))) != false, "range over: %d <= " #cur "=%d < %d", (min), (cur), (max));

#define JUT_MAX_ASSERT(LINE, cur, max) \
  JUT_ASSERT_F(LINE, ((cur) < (max)), "range over: %d <= " #cur "=%d < %d", 0, (cur), (max));

#define JUT_LOG_F(LINE, ...) \
  JUTAssertion::setLogMessage_f(JUTAssertion::getSDevice(), __FILE__, LINE, __VA_ARGS__);

#define JUT_REPORT_MSG(...) \
  OSReport(__VA_ARGS__);

#else // for non debug build
#define JUT_CONFIRM_MESSAGE(...)
#define JUT_WARNING(...)
#define JUT_WARNING_F(...)
#define JUT_WARNING_F2(...) // remove condintional?
#define JUT_CRITICAL_WARNING_F(...)
#define JUT_ASSERT(...)
#define JUT_ASSERT_F(...)
#define JUT_ASSERT_MSG(...)
#define JUT_MINMAX_ASSERT(...)
#define JUT_MAX_ASSERT(...)
#define JUT_LOG_F(...)
#define JUT_REPORT_MSG(...)
#endif

#endif