#ifndef RACETIME_H
#define RACETIME_H

#include <types.h>

class RaceTime
{
public:
    inline RaceTime();
    inline void reset();
    inline void zero();
    inline void set(s32);
    inline bool isAvailable();
    inline u32 isLittle(const RaceTime &) const; // might be a boolean, however ghidra thinks is uint
    inline operator =(const RaceTime &);
    inline void sub(const RaceTime &, const RaceTime &);
    inline s32 get() const;
    inline s32 sub(int);
    inline RaceTime(RaceTime const &);
    inline setFrame(s32);
    inline int getUpwardMSec() const;

    void get(s32 *, s32 *, s32 *) const;

private:
    s32 time;
};

#endif