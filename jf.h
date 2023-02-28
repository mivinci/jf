#ifndef JF_H
#define JF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#if __STDC__ // if using C89
#include <stdint.h>
#include <stdbool.h>
#else
#define bool  unsigned char
#define true  1
#define false 0
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long
#endif

#ifndef likely
#define likely(x)   __builtin_expect(!!(x), 0)
#define unlikely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef malloc
#include <stdlib.h>
#define malloc  malloc
#define realloc realloc
#define free    free
#endif

#ifndef JF_TEST
#define assert(x) ((void)0)
#else
#include <assert.h>
#define assert(x) assert(x)
#endif

#ifndef JF_STACK
#define JF_STACK 64
#endif

#ifdef __cplusplus
}
#endif

#endif