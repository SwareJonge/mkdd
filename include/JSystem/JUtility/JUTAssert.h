#ifndef JUTASSERT_H
#define JUTASSERT_H

#include "types.h"

class JUTAssertion {
public:
    static u32 getSDevice(void);
    static void showAssert_f(u32 device, char const * file, int line, char const * errormsg, ...);
    static void showAssert(u32 device, char const *file, int line, char const *errormsg); /* {
      showAssert_f(device, file, line, "%s", errormsg);
  }*/
};

#endif