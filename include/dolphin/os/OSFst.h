#ifndef _DOLPHIN_OSFST_H
#define _DOLPHIN_OSFST_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef struct OSFstEntry
{
    int entryNum;
    int nextEntryNum;
    char *fileNameMaybe;
} OSFstEntry;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif