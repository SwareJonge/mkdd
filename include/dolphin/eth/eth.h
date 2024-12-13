#ifndef _DOLPHIN_OS_ETH_H_
#define _DOLPHIN_OS_ETH_H_

#include <dolphin/types.h>
#include <dolphin/os/OSTime.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ME_NCRA_TXFIFO (1 << 2)

#define ETH_OK 1

#define ETH_TYPE_AUTO 0
#define ETH_TYPE_100FULL 1
#define ETH_TYPE_100HALF 2
#define ETH_TYPE_10FULL 3
#define ETH_TYPE_10HALF 4
#define ETH_TYPE_NONE 5

typedef struct ETHHeader {
    u8 dst[6];
    u8 src[6];
    u16 type;
} ETHHeader;

typedef struct ETHStat {
    u32 rcvcount;
    u32 sendcount;
    u16 cntof;
    u16 re;
    u16 te;
    u16 fifoe;
    u16 rbf;
    u16 rx_bf;
    u16 rx_crc;
    u16 rx_fae;
    u16 rx_fo;
    u16 rx_rw;
    u16 rx_rf;
    u16 tx_crs;
    u16 tx_uf;
    u16 tx_owc;
} ETHStat;

s32 ETHInit(s32 mode);
void ETHSendAsync(void* addr, s32 length, void (*callback2)(u8));
void ETHGetMACAddr(u8* macaddr);
void ETHSetMACAddr(u8* macaddr);
void ETHSetRecvCallback(void* (*cb0)(u16, s32, u8), void (*cb1)(u8*, s32));
BOOL ETHGetLinkStateAsync(BOOL* status);
s32 ETHGetNWAYMode(void);
void ETHSetProtoType(u16* array, s32 num);
void ETHGetStatus(ETHStat* stat);
s32 ETHGetLibraryVersion(void);
s32 ETHGetBBAType(void);
void ETHAddMulticastAddress(const u8* macaddr);
void ETHChangeIntPriority(BOOL flag);
u32 ETHGetREVID(void);
void ETHClearMulticastAddresses(void);

BOOL __ETHPostSend(u8, void (*)(u8), void*);
BOOL __ETHFilter(u8 *, s32);

extern OSTime __ETHInterruptTime;

#ifdef __cplusplus
}
#endif

#endif
