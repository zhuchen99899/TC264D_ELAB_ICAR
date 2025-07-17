/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_LOG_H_
#define ELAB_LOG_H_

/* includes ----------------------------------------------------------------- */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* public config ------------------------------------------------------------ */
#define ELOG_COLOR_ENABLE (1)

/* public defines ----------------------------------------------------------- */
/* debug level */
enum elog_level_enum {
    ELOG_LEVEL_ERROR = 1,
    ELOG_LEVEL_WARNING,
    ELOG_LEVEL_INFO,
    ELOG_LEVEL_DEBUG,

    ELOG_LEVEL_MAX,
};

/* public functions --------------------------------------------------------- */
#define ELAB_TAG(_tag) static const char *TAG = _tag

void elog_level_set(uint8_t level);

// default log level
#ifndef ELOG_LEVEL_CURRENT
#define ELOG_LEVEL_CURRENT ELOG_LEVEL_DEBUG // 默认等级
#endif

#ifndef ELOG_DISABLE
void _elog_printf(const char *tag, uint32_t line, uint8_t level, const char *s_format, ...);

/* Enable error level debug message */
#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_ERROR
#define elog_error(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define elog_error(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_WARNING
#define elog_warn(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_WARNING, __VA_ARGS__)
#else
#define elog_warn(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_INFO
#define elog_info(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_INFO, __VA_ARGS__)
#else
#define elog_info(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_DEBUG
#define elog_debug(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_DEBUG, __VA_ARGS__)
#else
elog_debug(...)
#endif

#endif /*no define ELOG_DISABLE */

#ifdef __cplusplus
}
#endif

#endif /* ELAB_LOG_H_ */

/* ----------------------------- end of file -------------------------------- */
