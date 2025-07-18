/**
 * @file drv_pwm.h
 * @author GitHub Copilot
 * @brief PWM驱动层头文件
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef DRV_PWM_H
#define DRV_PWM_H

/* includes ----------------------------------------------------------------- */
#include "../../3rd/elab/edf/normal/elab_pwm.h"
#include "../../3rd/elab/common/elab_assert.h"
#include "../../3rd/elab/common/elab_log.h"
#include "zf_common_headfile.h"

#ifdef __cplusplus
extern "C" {
#endif

/* public typedef ----------------------------------------------------------- */
typedef struct elab_pwm_driver {
    elab_pwm_t device;        // 父类：PWM设备基类
    pwm_channel_enum pwm_pin; // PWM通道引脚
    uint32_t frequency;       // 当前频率
    uint32_t duty;            // 当前占空比
} elab_pwm_driver_t;

/* public functions --------------------------------------------------------- */
/**
 * @brief PWM驱动初始化函数
 * @param me PWM驱动对象指针
 * @param name PWM设备名称
 * @param pwm_pin PWM通道引脚
 */
void elab_driver_pwm_init(elab_pwm_driver_t *me, const char *name, pwm_channel_enum pwm_pin,
                          uint32_t frequency, uint32_t duty);

#ifdef __cplusplus
}
#endif

#endif /* DRV_PWM_H */

/* ----------------------------- end of file -------------------------------- */
