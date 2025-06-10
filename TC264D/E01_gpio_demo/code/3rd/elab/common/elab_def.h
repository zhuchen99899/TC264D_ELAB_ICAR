/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_DEF_H
#define ELAB_DEF_H

/* includes ----------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "elab_utils.h"
#ifdef __cplusplus
extern "C" {
#endif

//#define ELAB_TAG(_tag) static const char TAG[] = _tag

/* public typedef ----------------------------------------------------------- */
typedef enum elab_err
{
    ELAB_OK                             = 0,
    ELAB_ERROR                          = -1,
    ELAB_ERR_EMPTY                      = -2,
    ELAB_ERR_FULL                       = -3,
    ELAB_ERR_TIMEOUT                    = -4,
    ELAB_ERR_BUSY                       = -5,
    ELAB_ERR_NO_MEMORY                  = -6,
    ELAB_ERR_IO                         = -7,
    ELAB_ERR_INVALID                    = -8,
    ELAB_ERR_MEM_OVERLAY                = -9,
    ELAB_ERR_MALLOC                     = -10,
    ELAB_ERR_NOT_ENOUGH                 = -11,
    ELAB_ERR_NO_SYSTEM                  = -12,
    ELAB_ERR_BUS                        = -13,
} elab_err_t;

#if defined(__linux__)
#define STR_ENTER                                   "\n"
#else
#define STR_ENTER                                   "\r\n"
#endif

/* Compiler Related Definitions */



#if defined(__x86_64__) || defined(__aarch64__)
typedef int64_t                         elab_pointer_t;
#elif defined(__i386__) || defined(__arm__)
typedef int32_t                         elab_pointer_t;
#elif defined(__TASKING__)
typedef int32_t                         elab_pointer_t;
#else
    #error The currnet CPU is NOT supported!
#endif

/**
 * @brief 分支预测，x 为真的可能性更大。
 */
#if defined(__GNUC__)
#   if !defined(likely)
#       
#   endif
#else
#   define likely(x) (x)
#endif

/**
 * @brief 分支预测，x 为假的可能性更大。
 */
#if defined(__GNUC__)
#   if !defined(unlikely)
#       define unlikely(x) __builtin_expect(!!(x), 0)
#   endif
#else
#   define unlikely(x) (x)
#endif



#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARM Compiler */

    #include <stdarg.h>
    #define ELAB_SECTION(x)             __attribute__((section(x)))
    #define ELAB_USED                   __attribute__((used))
    #define ELAB_ALIGN(n)               __attribute__((aligned(n)))
    #define ELAB_WEAK                   __attribute__((weak))
    #define elab_inline                 static __inline
    #define elab_likely(x)              (x)
    #define elab_unlikely(x)            (x)
#elif defined (__IAR_SYSTEMS_ICC__)           /* for IAR Compiler */

    #include <stdarg.h>
    #define ELAB_SECTION(x)             @ x
    #define ELAB_USED                   __root
    #define ELAB_PRAGMA(x)              _Pragma(#x)
    #define ELAB_ALIGN(n)               ELAB_PRAGMA(data_alignment=n)
    #define ELAB_WEAK                   __weak
    #define elab_inline                 static inline
    #define elab_likely(x)              (x)
    #define elab_unlikely(x)            (x)
#elif defined (__GNUC__)                      /* GNU GCC Compiler */

    #include <stdarg.h>
    #define ELAB_SECTION(x)             __attribute__((section(x)))
    #define ELAB_USED                   __attribute__((used))
    #define ELAB_ALIGN(n)               __attribute__((aligned(n)))
    #define ELAB_WEAK                   __attribute__((weak))
    #define elab_inline                 static inline
    #define elab_likely(x)              __builtin_expect(!!(x), 1)
    #define elab_unlikely(x)            __builtin_expect(!!(x), 0)
#elif defined(__TASKING__)                  /* Tasking Compiler for AURIX */

    #include <stdarg.h>
    #define ELAB_USED                   __attribute__((protect))  //infineon must use protect to keep seciton
    #define ELAB_ALIGN(n)               __attribute__((aligned(n)))
    #define ELAB_WEAK                   __attribute__((weak))
    #define elab_inline                 static inline
    #define elab_likely(x)              (x)
    #define elab_unlikely(x)            (x)

#else
    #error The current compiler is NOT supported!
#endif

#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
}
#endif

#endif
