/*
 * blob - library for generating/parsing tagged binary data
 *
 * Copyright (C) 2010 Felix Fietkau <nbd@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _BLOB_H__
#define _BLOB_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*
 * calloc_a(size_t len, [void **addr, size_t len,...], NULL)
 *
 * allocate a block of memory big enough to hold multiple aligned objects.
 * the pointer to the full object (starting with the first chunk) is returned,
 * all other pointers are stored in the locations behind extra addr arguments.
 * the last argument needs to be a NULL pointer
 */

#define calloc_a(len, ...) __calloc_a(len, ##__VA_ARGS__, NULL)

void *__calloc_a(size_t len, ...);

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define __BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#ifdef __OPTIMIZE__
extern int __BUILD_BUG_ON_CONDITION_FAILED;
#define BUILD_BUG_ON(condition)					\
	do {							\
		__BUILD_BUG_ON(condition);			\
		if (condition)					\
			__BUILD_BUG_ON_CONDITION_FAILED = 1;	\
	} while(0)
#else
#define BUILD_BUG_ON __BUILD_BUG_ON
#endif

#if defined(__APPLE__) && !defined(CLOCK_MONOTONIC)
#define LIBUBOX_COMPAT_CLOCK_GETTIME

#include <mach/clock_types.h>
#define CLOCK_REALTIME	CALENDAR_CLOCK
#define CLOCK_MONOTONIC	SYSTEM_CLOCK

int clock_gettime(int type, struct timespec *tv);

#endif

#ifdef __GNUC__
#define _GNUC_MIN_VER(maj, min) (((__GNUC__ << 8) + __GNUC_MINOR__) >= (((maj) << 8) + (min)))
#else
#define _GNUC_MIN_VER(maj, min) 0
#endif

#if defined(__linux__) || defined(__CYGWIN__)
#include <byteswap.h>
#include <endian.h>

#elif defined(__APPLE__)
#include <machine/endian.h>
#include <machine/byte_order.h>
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#else
#include <machine/endian.h>
#endif

#ifndef __BYTE_ORDER
#define __BYTE_ORDER BYTE_ORDER
#endif
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN BIG_ENDIAN
#endif
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#endif

#define __constant_swap16(x) ((uint16_t)(				\
	(((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |			\
	(((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define __constant_swap32(x) ((uint32_t)(				\
	(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |		\
	(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |		\
	(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |		\
	(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#define __constant_swap64(x) ((uint64_t)(				\
	(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) |	\
	(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) |	\
	(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) |	\
	(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) |	\
	(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) |	\
	(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) |	\
	(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) |	\
	(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

/*
 * This returns a constant expression while determining if an argument is
 * a constant expression, most importantly without evaluating the argument.
 */
#define __is_constant(x)						\
	(sizeof(int) == sizeof(*(1 ? ((void*)((long)(x) * 0l)) : (int*)1)))

#define __eval_once(func, x)						\
	({ __typeof__(x) __x = x; func(__x); })

#ifdef __cplusplus
/*
 * g++ does not support __builtin_choose_expr, so always use __eval_once.
 * Unfortunately this means that the byte order functions can't be used
 * as a constant expression anymore
 */
#define __eval_safe(func, x) __eval_once(func, x)
#else
#define __eval_safe(func, x)						\
	__builtin_choose_expr(__is_constant(x),				\
			      func(x), __eval_once(func, x))
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define const_cpu_to_be64(x) __constant_swap64(x)
#define const_cpu_to_be32(x) __constant_swap32(x)
#define const_cpu_to_be16(x) __constant_swap16(x)

#define const_be64_to_cpu(x) __constant_swap64(x)
#define const_be32_to_cpu(x) __constant_swap32(x)
#define const_be16_to_cpu(x) __constant_swap16(x)

#define const_cpu_to_le64(x) (x)
#define const_cpu_to_le32(x) (x)
#define const_cpu_to_le16(x) (x)

#define const_le64_to_cpu(x) (x)
#define const_le32_to_cpu(x) (x)
#define const_le16_to_cpu(x) (x)

#define cpu_to_be64(x) __eval_safe(__constant_swap64, x)
#define cpu_to_be32(x) __eval_safe(__constant_swap32, x)
#define cpu_to_be16(x) __eval_safe(__constant_swap16, x)

#define be64_to_cpu(x) __eval_safe(__constant_swap64, x)
#define be32_to_cpu(x) __eval_safe(__constant_swap32, x)
#define be16_to_cpu(x) __eval_safe(__constant_swap16, x)

#define cpu_to_le64(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)

#define le64_to_cpu(x) (x)
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)

#else /* __BYTE_ORDER == __LITTLE_ENDIAN */

#define const_cpu_to_le64(x) __constant_swap64(x)
#define const_cpu_to_le32(x) __constant_swap32(x)
#define const_cpu_to_le16(x) __constant_swap16(x)

#define const_le64_to_cpu(x) __constant_swap64(x)
#define const_le32_to_cpu(x) __constant_swap32(x)
#define const_le16_to_cpu(x) __constant_swap16(x)

#define const_cpu_to_be64(x) (x)
#define const_cpu_to_be32(x) (x)
#define const_cpu_to_be16(x) (x)

#define const_be64_to_cpu(x) (x)
#define const_be32_to_cpu(x) (x)
#define const_be16_to_cpu(x) (x)

#define cpu_to_le64(x) __eval_safe(__constant_swap64, x)
#define cpu_to_le32(x) __eval_safe(__constant_swap32, x)
#define cpu_to_le16(x) __eval_safe(__constant_swap16, x)

#define le64_to_cpu(x) __eval_safe(__constant_swap64, x)
#define le32_to_cpu(x) __eval_safe(__constant_swap32, x)
#define le16_to_cpu(x) __eval_safe(__constant_swap16, x)

#define cpu_to_be64(x) (x)
#define cpu_to_be32(x) (x)
#define cpu_to_be16(x) (x)

#define be64_to_cpu(x) (x)
#define be32_to_cpu(x) (x)
#define be16_to_cpu(x) (x)

#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef __constructor
#define __constructor __attribute__((constructor))
#endif

#ifndef __destructor
#define __destructor __attribute__((destructor))
#endif

#ifndef __hidden
#define __hidden __attribute__((visibility("hidden")))
#endif

#ifndef __has_attribute
# define __has_attribute(x) 0
#endif

#ifndef fallthrough
# if __has_attribute(__fallthrough__)
#  define fallthrough __attribute__((__fallthrough__))
# else
#  define fallthrough do {} while (0)  /* fallthrough */
# endif
#endif

#define BLOB_COOKIE		0x01234567

enum {
	BLOB_ATTR_UNSPEC,
	BLOB_ATTR_NESTED,
	BLOB_ATTR_BINARY,
	BLOB_ATTR_STRING,
	BLOB_ATTR_INT8,
	BLOB_ATTR_INT16,
	BLOB_ATTR_INT32,
	BLOB_ATTR_INT64,
	BLOB_ATTR_DOUBLE,
	BLOB_ATTR_LAST
};

#define BLOB_ATTR_ID_MASK  0x7f000000
#define BLOB_ATTR_ID_SHIFT 24
#define BLOB_ATTR_LEN_MASK 0x00ffffff
#define BLOB_ATTR_ALIGN    4
#define BLOB_ATTR_EXTENDED 0x80000000

struct blob_attr {
	uint32_t id_len;
	char data[];
} __packed;

struct blob_attr_info {
	unsigned int type;
	unsigned int minlen;
	unsigned int maxlen;
	bool (*validate)(const struct blob_attr_info *, struct blob_attr *);
};

struct blob_buf {
	struct blob_attr *head;
	bool (*grow)(struct blob_buf *buf, int minlen);
	int buflen;
	void *buf;
};

/*
 * blob_data: returns the data pointer for an attribute
 */
static inline void *
blob_data(const struct blob_attr *attr)
{
	return (void *) attr->data;
}

/*
 * blob_id: returns the id of an attribute
 */
static inline unsigned int
blob_id(const struct blob_attr *attr)
{
	int id = (be32_to_cpu(attr->id_len) & BLOB_ATTR_ID_MASK) >> BLOB_ATTR_ID_SHIFT;
	return id;
}

static inline bool
blob_is_extended(const struct blob_attr *attr)
{
	return !!(attr->id_len & cpu_to_be32(BLOB_ATTR_EXTENDED));
}

/*
 * blob_len: returns the length of the attribute's payload
 */
static inline size_t
blob_len(const struct blob_attr *attr)
{
	return (be32_to_cpu(attr->id_len) & BLOB_ATTR_LEN_MASK) - sizeof(struct blob_attr);
}

/*
 * blob_raw_len: returns the complete length of an attribute (including the header)
 */
static inline size_t
blob_raw_len(const struct blob_attr *attr)
{
	return blob_len(attr) + sizeof(struct blob_attr);
}

/*
 * blob_pad_len: returns the padded length of an attribute (including the header)
 */
static inline size_t
blob_pad_len(const struct blob_attr *attr)
{
	unsigned int len = blob_raw_len(attr);
	len = (len + BLOB_ATTR_ALIGN - 1) & ~(BLOB_ATTR_ALIGN - 1);
	return len;
}

static inline uint8_t
blob_get_u8(const struct blob_attr *attr)
{
	return *((uint8_t *) attr->data);
}

static inline uint16_t
blob_get_u16(const struct blob_attr *attr)
{
	uint16_t *tmp = (uint16_t*)attr->data;
	return be16_to_cpu(*tmp);
}

static inline uint32_t
blob_get_u32(const struct blob_attr *attr)
{
	uint32_t *tmp = (uint32_t*)attr->data;
	return be32_to_cpu(*tmp);
}

static inline uint64_t
blob_get_u64(const struct blob_attr *attr)
{
	uint32_t *ptr = (uint32_t *) blob_data(attr);
	uint64_t tmp = ((uint64_t) be32_to_cpu(ptr[0])) << 32;
	tmp |= be32_to_cpu(ptr[1]);
	return tmp;
}

static inline int8_t
blob_get_int8(const struct blob_attr *attr)
{
	return blob_get_u8(attr);
}

static inline int16_t
blob_get_int16(const struct blob_attr *attr)
{
	return blob_get_u16(attr);
}

static inline int32_t
blob_get_int32(const struct blob_attr *attr)
{
	return blob_get_u32(attr);
}

static inline int64_t
blob_get_int64(const struct blob_attr *attr)
{
	return blob_get_u64(attr);
}

static inline const char *
blob_get_string(const struct blob_attr *attr)
{
	return attr->data;
}

static inline struct blob_attr *
blob_next(const struct blob_attr *attr)
{
	return (struct blob_attr *) ((char *) attr + blob_pad_len(attr));
}

extern void blob_fill_pad(struct blob_attr *attr);
extern void blob_set_raw_len(struct blob_attr *attr, unsigned int len);
extern bool blob_attr_equal(const struct blob_attr *a1, const struct blob_attr *a2);
extern int blob_buf_init(struct blob_buf *buf, int id);
extern void blob_buf_free(struct blob_buf *buf);
extern bool blob_buf_grow(struct blob_buf *buf, int required);
extern struct blob_attr *blob_new(struct blob_buf *buf, int id, int payload);
extern void *blob_nest_start(struct blob_buf *buf, int id);
extern void blob_nest_end(struct blob_buf *buf, void *cookie);
extern struct blob_attr *blob_put(struct blob_buf *buf, int id, const void *ptr, unsigned int len);
extern bool blob_check_type(const void *ptr, unsigned int len, int type);
extern int blob_parse(struct blob_attr *attr, struct blob_attr **data, const struct blob_attr_info *info, int max);
extern int blob_parse_untrusted(struct blob_attr *attr, size_t attr_len, struct blob_attr **data, const struct blob_attr_info *info, int max);
extern struct blob_attr *blob_memdup(struct blob_attr *attr);
extern struct blob_attr *blob_put_raw(struct blob_buf *buf, const void *ptr, unsigned int len);

static inline struct blob_attr *
blob_put_string(struct blob_buf *buf, int id, const char *str)
{
	return blob_put(buf, id, str, strlen(str) + 1);
}

static inline struct blob_attr *
blob_put_u8(struct blob_buf *buf, int id, uint8_t val)
{
	return blob_put(buf, id, &val, sizeof(val));
}

static inline struct blob_attr *
blob_put_u16(struct blob_buf *buf, int id, uint16_t val)
{
	val = cpu_to_be16(val);
	return blob_put(buf, id, &val, sizeof(val));
}

static inline struct blob_attr *
blob_put_u32(struct blob_buf *buf, int id, uint32_t val)
{
	val = cpu_to_be32(val);
	return blob_put(buf, id, &val, sizeof(val));
}

static inline struct blob_attr *
blob_put_u64(struct blob_buf *buf, int id, uint64_t val)
{
	val = cpu_to_be64(val);
	return blob_put(buf, id, &val, sizeof(val));
}

#define blob_put_int8	blob_put_u8
#define blob_put_int16	blob_put_u16
#define blob_put_int32	blob_put_u32
#define blob_put_int64	blob_put_u64

#define __blob_for_each_attr(pos, attr, rem) \
	for (pos = (struct blob_attr *) attr; \
	     rem >= sizeof(struct blob_attr) && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))


#define blob_for_each_attr(pos, attr, rem) \
	for (rem = attr ? blob_len(attr) : 0, \
	     pos = (struct blob_attr *) (attr ? blob_data(attr) : NULL); \
	     rem >= sizeof(struct blob_attr) && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))

#define blob_for_each_attr_len(pos, attr, attr_len, rem) \
	for (rem = attr ? blob_len(attr) : 0, \
	     pos = (struct blob_attr *) (attr ? blob_data(attr) : NULL); \
	     rem >= sizeof(struct blob_attr) && rem < attr_len && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))

#endif
