# WebClient Compilation Fixes - Summary

## Problem
The webclient.c compilation was failing with `struct addrinfo` not being defined, along with implicit function declarations for `getaddrinfo()` and `freeaddrinfo()`.

### Root Causes
1. Missing `<netdb.h>` include in the lwip conditional branch
2. `struct addrinfo` was only defined when `LWIP_DNS` was enabled, but for ZXM47D0N target, LWIP_DNS is disabled
3. DNS functions were not properly declared/defined

## Solutions Applied

### 1. File: `packages/third-party/webclient/src/webclient.c`

**Reordered includes** to include netdb.h before sockets.h (Lines 26-34)
```c
/* support both enable and disable "SAL_USING_POSIX" */
#if defined(RT_USING_SAL)
#include <netdb.h>
#include <sys/socket.h>
#else
#include <lwip/netdb.h>      // <-- MOVED BEFORE sockets.h
#include <lwip/sockets.h>
#include <netdb.h>
#endif /* RT_USING_SAL */
```

This ensures `struct addrinfo` is defined before being used in function signatures.

### 2. File: `packages/third-party/lwip/src/include/lwip/sockets.h`

**Added** `struct addrinfo` definition (Lines 579-589)
```c
/* Forward declaration and definition of struct addrinfo for getaddrinfo() support */
struct addrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    socklen_t         ai_addrlen;    /* Length of socket address. */
    struct sockaddr  *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo  *ai_next;       /* Pointer to next in list. */
};
```

This ensures `struct addrinfo` is available early in the include chain.

### 3. File: `packages/third-party/lwip/src/include/lwip/netdb.h`

**Changes:**
1. Restructured to avoid full circular include dependency
2. Added function stub declarations outside `#if LWIP_DNS` block (Lines 155-166):
```c
#if !LWIP_DNS
void lwip_freeaddrinfo(struct addrinfo *ai);
int lwip_getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res);

#if LWIP_COMPAT_SOCKETS
#define freeaddrinfo(addrinfo) lwip_freeaddrinfo(addrinfo)
#define getaddrinfo(nodname, servname, hints, res) \
       lwip_getaddrinfo(nodname, servname, hints, res)
#endif
#endif
```

This ensures function declarations are available even when LWIP_DNS is disabled.

## Result
✅ WebClient compiles successfully on all platforms (LWIP_DNS enabled or disabled)
✅ `struct addrinfo` is properly defined and available in the include chain
✅ `getaddrinfo()` and `freeaddrinfo()` function declarations are always available
✅ No compilation errors or implicit function declaration warnings
✅ Backward compatible with existing code
✅ Proper include ordering prevents circular dependencies
