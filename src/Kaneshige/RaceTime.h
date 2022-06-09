#ifndef RACETIME_H
#define RACETIME_H

#include <types.h>

class RaceTime
{
public:
    inline RaceTime();
    //inline RaceTime(RaceTime const &);
    //inline void operator=(const RaceTime &);
    inline void reset();
    inline void zero();
    inline void set(int);
    inline void set(RaceTime const &);
    inline bool isAvailable();
    inline bool isLittle(const RaceTime &) const; 
    
    inline void sub(const RaceTime &, const RaceTime &);
    inline int get() const;
    inline int sub(int);
    
    inline void setFrame(int);
    inline int getUpwardMSec() const;

    void get(int *, int *, int *) const;

private:
    int time;
};

#endif