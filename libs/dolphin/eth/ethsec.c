#include <dolphin/md5.h>
#include <dolphin/os.h>
#include <dolphin/eth/eth.h>
#include <dolphin/ip/ip.h>
#include <dolphin/tcp/tcp.h>

#include <string.h>

#define IP_MIN_HLEN (s32)sizeof(IPHeader)
#define TCP_MIN_HLEN (s32)sizeof(TCPHeader)

static TCPResetInfo Ri;
static u8 SendBuf[82];
static const u8 NintendoAddr[6] = { 0,9,191,0,0,0 };

extern u32 __OSGetDIConfig(void);
vu16 __OSDeviceCode : (OS_BASE_CACHED | 0x30E6);

static inline BOOL CheckConsoleType() {
    if (__OSGetDIConfig() == 0xFF) {
        switch (__OSDeviceCode) {
        case 0x8200: {
            return TRUE;
        }
        case 0x8001: {
            if (OSGetPhysicalMemSize() == 0x3000000) {
                return TRUE;
            }
            return FALSE;
        }
        }
        return FALSE;
    }

    if ((OSGetConsoleType() & OS_CONSOLE_MASK) == 0) {
        return FALSE;
    }

    return TRUE;
}

static s16 AT_IP4_IsMyAddrMask(u8 val) { return 0; }

/*static inline BOOL IPIsLocalAddr(u32, const u8 *addr) {
    return AT_IP4_IsMyAddrMask(*addr) != -1;
}*/

static u16 CalcIpCheckSum(IPHeader *ip) {
    int len;
    u32 sum;
    u16 *p;

    sum = 0;
    len =  (ip->verlen & 0xf) << 2;
    p = (u16*)ip;

    while (len > 0) {
        sum += *p++;
        len -= 2;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = ((sum & 0xffff) + (sum >> 16));
    return sum ^ 0xffff;
}

static u16 CalcTcpCheckSum(IPHeader *ip, s32 len) {
    s32 hlen;
    u16 *p;
    u32 sum = 0;

#line 212
    ASSERT(IP_MIN_HLEN + TCP_MIN_HLEN <= len);
#line 215
    ASSERT(ip->proto == IP_PROTO_TCP);
    
    hlen = (ip->verlen & 0xf) << 2;
    sum += ((u16*)ip->src)[0];
    sum += ((u16*)ip->src)[1];
    sum += ((u16*)ip->dst)[0];
    sum += ((u16*)ip->dst)[1];
    sum += 6;
    sum += (u32)len - (u32)hlen;

    p = (u16*)((u32)ip+hlen);
    len = len - hlen;
    while (len > 1) {
        sum += *p++;
        len -= 2;
    }

    if (len == 1) {
        sum += *((u8*)p) * 0x100;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return sum ^ 0xffff;
}

static s32 GetTcpSegLen(IPHeader *ip, TCPHeader *tcp) {
    s32 len = ip->len - ((ip->verlen & 0xf) << 2) - (s32)(((tcp->flag & 0xf000) >> 10));

    if (tcp->flag & 2) {
        len += 1;
    }

    if (tcp->flag & 1) {
        len += 1;
    }

    return len;
}

static BOOL VerifyPacket(IPHeader *ip) {
    if(ip->verlen >> 4 != 4 || ip->len < ((ip->verlen & 0xf) << 2)) {
        return TRUE;
    }

    if (CalcIpCheckSum(ip) != 0) {
        return TRUE;
    }

    if ((ip->dst[0] & 0xf0) == 0xe0 || (ip->src[0] & 0xf0) == 0xe0 ||
         (ip->dst[0] & 0xf8) == 0xf0 || (ip->src[0] & 0xf8) == 0xf0) {
        return TRUE;
    }

    ASSERT(ip->proto == IP_PROTO_TCP);
    if (CalcTcpCheckSum(ip, ip->len) != 0) {
        return TRUE;
    }

    return FALSE;
}

static void CalcDigest(u8 *digest, TCPHeader *tcp) {
    MD5Context context;
    u64 secret;

    MD5Init(&context);
    MD5Update(&context,(u8*)&tcp->seq,sizeof(tcp->seq));
    MD5Update(&context,(u8*)&tcp->ack,sizeof(tcp->ack));

    secret = ((tcp->seq ^ tcp->ack) * 0x5DEECE66Dll + 0xb) % 0x1000000000000ll;
    MD5Update(&context,(u8 *)&secret + 2, sizeof(secret) - 2);
    MD5Final(digest,&context);
}

BOOL __ETHFilter(u8 *buf, s32 len) {
    ETHHeader *eh;
    IPHeader *ip;
    TCPHeader *tcp;
    TCPResetInfo *ri;
    u8 digest1[16];
    u8 digest2[16];

    ri = &Ri;
    if (CheckConsoleType()) {
        return TRUE;
    }

    eh = (ETHHeader *)buf;
    if (eh->type != 0x800) {
        return TRUE;
    }

    if (memcmp(eh->src, NintendoAddr, 3) == 0) {
        return TRUE;
    }

    len = len - sizeof(ETHHeader);
    ip = (IPHeader *)(eh + 1);
    if (len < IP_MIN_HLEN || len < ip->len) {
        return TRUE;
    }

    if(ip->proto != IP_PROTO_TCP) {
        return TRUE;
    }

    tcp = (TCPHeader *)((u32)ip + ((ip->verlen & 0xf) << 2));
    if ((s32)(((tcp->flag & 0xf000) >> 10)) < TCP_MIN_HLEN) {
        return TRUE;
    }

    if (ip->len < ((ip->verlen & 0xf) << 2) + (s32)(((tcp->flag & 0xf000) >> 10))) {
        return TRUE;
    }

    if ((tcp->flag & 3) == 2) {
        switch(tcp->src) {
        case 0x76c:
        case 0x35:
            return TRUE;
        }

        switch (tcp->dst) {
        case 0x76c:
            return TRUE;
        }

        if (IPIsLocalAddr(0, ip->src) == FALSE) {
            return TRUE;
        }

        if (tcp->flag & 4) {
            return TRUE;
        }

        if (VerifyPacket(ip)) {
            return TRUE;
        }

        if (ri->flag == 0) {
            memmove(ri->addr, eh->src, sizeof(ri->addr));
            memmove(ri->dstAddr, ip->dst, sizeof(ri->dstAddr));
            memmove(ri->srcAddr, ip->src, sizeof(ri->dstAddr));
            ri->dstPort = tcp->dst;
            ri->srcPort = tcp->src;

            if ((tcp->flag & 0x10) == 0) {
                ri->seq = 0;
                ri->ack = tcp->seq + GetTcpSegLen(ip, tcp);
                ri->flag = (0x10 | 0x2 | 0x1);
            }
            else {
                ri->seq = tcp->ack;
                ri->ack = 0;  
                ri->flag = (0x2 | 0x1);
            }
            ri->id = 0;
            return TRUE;
        }
        ip->len = 0;
        return FALSE;
    }

    if ((tcp->flag & 3) == 3) {
        if ((s32)(((tcp->flag & 0xf000) >> 10)) != TCP_MIN_HLEN) {
            return TRUE;
        }

        if (ip->len != 0x40) {
            return TRUE;
        }

        if (IPIsLocalAddr(0, ip->src) == FALSE) {
            return TRUE;
        }

        if (VerifyPacket(ip)) {
            return TRUE;
        }

        CalcDigest(digest1, tcp);
        __IPDecodeFromBase64(tcp + 1, 0x18, digest2);

        if (memcmp(digest1, digest2, sizeof(digest1))) {
            return TRUE;
        }
        ip->len = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL __ETHPostSend(u8 ltps , void (*callback)(u8), void *prev) {
    ETHHeader *eh;
    IPHeader *ip;
    TCPHeader *tcp;
    TCPResetInfo *ri;
    u8 digest[16];

    ri = &Ri;

    if (ri->flag == 0) {
        ri = NULL;
    }
    else if (prev != SendBuf) {
        eh = prev;
        ip = (IPHeader *)(eh + 1);
        tcp = (TCPHeader *)(ip + 1);
        if (*((u32*)ip->dst) == *((u32*)ri->srcAddr) && *((u32*)ip->src) == *((u32*)ri->dstAddr) 
            && tcp->src == ri->dstPort && tcp->dst == ri->srcPort) {
            
            if (tcp->flag & 4) {
                ri->flag = 0;
                ri = NULL;
            }
            else {
                ri->seq = tcp->seq + GetTcpSegLen(ip, tcp);
                ri->ack = tcp->ack;
                ri->flag |= tcp->flag & 0x10;

                ri->id = ip->id - 0x4000;
                if (ri->id == 0) {
                    ri->id = 1;
                }
            }
        }
    }

    if (ri != NULL && ri->id != 0) {
        eh = (ETHHeader *)SendBuf;
        ip = (IPHeader *)(eh + 1);
        eh->type = 0x800;
        memmove(SendBuf, ri, 6);
        ETHGetMACAddr(eh->src);

        ip->verlen = 0x45;
        ip->tos = 0;
        ip->len = 0x40;
        ip->id = ri->id;
        ip->frag = 0x4000;
        ip->ttl = 0xff;
        ip->proto = IP_PROTO_TCP;
        ip->sum = 0;
        memmove(ip->dst, ri->srcAddr, sizeof(ip->dst));
        memmove(ip->src, ri->dstAddr, sizeof(ip->src));
        

        tcp = (TCPHeader *)((u32)ip + ((ip->verlen & 0xf) << 2));
        tcp->src = ri->dstPort;
        tcp->dst = ri->srcPort;
        tcp->seq = ri->seq;
        tcp->ack = ri->ack;
        tcp->flag = ri->flag & 0x3f | 0x5000;
        tcp->win = 0;
        tcp->sum = 0;
        tcp->urg = 0;

        CalcDigest(digest, tcp);
        __IPEncodeToBase64(digest, sizeof(digest), tcp + 1);
        ip->sum = CalcIpCheckSum(ip);
        tcp->sum = CalcTcpCheckSum(ip, ip->len);
        ri->flag = 0;
        ETHSendAsync(SendBuf, 0x4e, callback);
    }
    else if (callback) {
        (*callback)(ltps);
    }
    return ETH_OK;
}
