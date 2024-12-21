#ifndef _DOLPHIN_IP_H_
#define _DOLPHIN_IP_H_

#include <dolphin/types.h>

// Protocols
#define IP_PROTO_ICMP 1
#define IP_PROTO_TCP  6
#define IP_PROTO_UDP  17

typedef struct IPHeader {
    u8 verlen;
    u8 tos;
    u16 len;
    u16 id;
    u16 frag;
    u8 ttl;
    u8 proto;
    u16 sum;
    u8 src[4];
    u8 dst[4];
} IPHeader;

BOOL IPIsLocalAddr(u32, const u8 *addr);

#endif
