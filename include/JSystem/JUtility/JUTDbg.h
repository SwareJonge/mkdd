#ifndef _JSYSTEM_JUTILITY_JUTDBG_H
#define _JSYSTEM_JUTILITY_JUTDBG_H

#include <JSystem/JUtility/JUTAssert.h>
#include <JSystem/JUtility/JUTException.h>

// NOTE: in TP debug this doesn't match

// Panic functions
#define JUT_PANIC(MSG) \
  JUTException::panic(__FILE__, __LINE__, MSG);
#define JUT_PANIC_F(...) \
  JUTException::panic_f(__FILE__, __LINE__, __VA_ARGS__);

#if DEBUG
// Asserts
#define JUT_CONFIRM_MESSAGE(COND) \
  JUTAssertion::setConfirmMessage(JUTAssertion::getSDevice(), __FILE__, __LINE__, COND, #COND);

// not sure if it's conditional?
#define JUT_WARNING(COND)                                                                   \
  if (!(COND))                                                                              \
  {                                                                                         \
    JUTAssertion::setWarningMessage(JUTAssertion::getSDevice(), __FILE__, __LINE__, #COND); \
  }

#define JUT_WARNING_F(COND, ...)                                                                    \
  if (!(COND))                                                                                      \
  {                                                                                                 \
    JUTAssertion::setWarningMessage_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, __VA_ARGS__); \
  }
#define JUT_WARNING_F2(...) \
  JUTAssertion::setWarningMessage_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, __VA_ARGS__);

// perhaps "%s" is already added here
#define JUT_CRITICAL_WARNING_F(...) \
  JUTAssertion::setWarningMessage_f(2, __FILE__, __LINE__, __VA_ARGS__);

#define JUT_ASSERT(COND)                                                             \
  if (!(COND))                                                                       \
  {                                                                                  \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, __LINE__, #COND); \
    OSHalt("Halt");                                                                  \
  }

#define JUT_ASSERT_F(COND, ...)                                                              \
  if (!(COND))                                                                               \
  {                                                                                          \
    JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, __VA_ARGS__); \
    OSHalt("Halt");                                                                          \
  }

#define JUT_ASSERT_MSG(COND, MSG)                                                  \
  if (!(COND))                                                                     \
  {                                                                                \
    JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, __LINE__, MSG); \
    OSHalt("Halt");                                                                \
  }

#define JUT_MINMAX_ASSERT(min, cur, max) \
  JUT_ASSERT_F((((cur) >= (min)) && ((cur) < (max))) != false, "range over: %d <= " #cur "=%d < %d", (min), (cur), (max));

#define JUT_MAX_ASSERT(cur, max) \
  JUT_ASSERT_F(((cur) < (max)), "range over: %d <= " #cur "=%d < %d", 0, (cur), (max));

#define JUT_LOG_F(...) \
  JUTAssertion::setLogMessage_f(JUTAssertion::getSDevice(), __FILE__, __LINE__, __VA_ARGS__);

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