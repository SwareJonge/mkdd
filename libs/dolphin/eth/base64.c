#include <string.h>

static const char Base64[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="
};

char *__IPEncodeToBase64(void *_src, s32 len , char *dst) {
    const u8 *src;
    int i;
    char *p;

    src = _src;
    p = dst;    
    for (i = 0; i < len - len % 3; i += 3) {
        *p++ = Base64[(src[i+0] >> 2) & 0x3f];
        *p++ = Base64[((src[i+0] & 0x3) << 4) | ((src[i+1] >> 4) & 0xf)];
        *p++ = Base64[((src[i+1] & 0xf) << 2) | ((src[i+2] >> 6) & 0x3)];
        *p++ = Base64[src[i+2] & 0x3f];
    }

    i = len - len % 3;
    // Encode remaining characters
    switch (len % 3) {
    case 2: {
        *p++ = Base64[(src[i+0] >> 2)  & 0x3f];
        *p++ = Base64[((src[i+0] & 0x3) << 4) | ((src[i+1] >> 4) & 0xf)];
        *p++ = Base64[((src[i+1] & 0xf) << 2)];
        *p++ = '=';
        break;
    }
    case 1: {
        *p++ = Base64[(src[i+0] >> 2) & 0x3f];
        *p++ = Base64[((src[i+0] & 0x3) << 4)];
        *p++ = '=';  
        *p++ = '=';
        break;
    }
    }

    return p;
}

void *__IPDecodeFromBase64(const char *src, long len , void *_dst) {
    u8 *dst;
    int i;
    u8 *p;

    dst = _dst;
    p = dst;
    for (i = 0; i < len; i += 4) {
        int n0;
        int n1;
        int n2;
        int n3;
        n0 = strchr(Base64, src[i+0]) - Base64;
        n1 = strchr(Base64, src[i+1]) - Base64;
        *p++ = (n0 << 2) | (n1 >> 4) & 0x3;

        if (src[i+2] == '=') break;

        n2 = strchr(Base64, src[i+2]) - Base64;
        *p++ = ((n1 << 4) & 0xff) | (n2 >> 2) & 0xf;

        if (src[i+3] == '=') break;

        n3 = strchr(Base64, src[i+3]) - Base64;
        *p++ = ((n2 << 6) & 0xff) | (n3 & 0x3f);
    }
    return p;
}
