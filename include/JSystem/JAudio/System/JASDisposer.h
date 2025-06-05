#ifndef JAUDIO_JASDISPOSER_H
#define JAUDIO_JASDISPOSER_H

class JASDisposer
{
public:
    JASDisposer() {}
    virtual ~JASDisposer() {}
    virtual void onDispose() {}
};

#endif
