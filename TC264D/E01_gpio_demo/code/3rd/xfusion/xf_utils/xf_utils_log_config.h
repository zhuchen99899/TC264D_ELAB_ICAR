/**
 * @file xf_log_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_UTILS_LOG_CONFIG_H__
#define __XF_UTILS_LOG_CONFIG_H__
#include "../../elab/common/elab_def.h"
#include "../../elab/common/elab_utils.h"
#include "../xf_common/xf_err.h"
/* ==================== [Includes] ========================================== */



#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */


#if !defined(XF_LOG_DUMP_ENABLE) || (XF_LOG_DUMP_ENABLE)
#   define XF_LOG_DUMP_IS_ENABLE (1)
#else
#   define XF_LOG_DUMP_IS_ENABLE (0)
#endif

#define XF_LOG_NONE             (0)
#define XF_LOG_USER             (1)
#define XF_LOG_ERROR            (2)
#define XF_LOG_WARN             (3)
#define XF_LOG_INFO             (4)
#define XF_LOG_DEBUG            (5)
#define XF_LOG_VERBOSE          (6)




#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_UTILS_LOG_CONFIG_H__
