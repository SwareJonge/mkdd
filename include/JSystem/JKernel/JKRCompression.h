#ifndef JSYSTEM_JKRCOMPRESSION_H
#define JSYSTEM_JKRCOMPRESSION_H

// TODO: this probably isn't an enum, change with int and const values or defines
#define JKRCOMPRESSION_NONE 0
#define JKRCOMPRESSION_YAY0 1
#define JKRCOMPRESSION_YAZ0 2
#define JKRCOMPRESSION_ASR 3

enum CompressionMethod
{
    TYPE_NONE,
    TYPE_YAY0,
    TYPE_YAZ0,
    TYPE_ASR
};

#endif