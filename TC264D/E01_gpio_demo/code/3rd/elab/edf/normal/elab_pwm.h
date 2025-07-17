/**
 * @file elab_pwm.h
 * @author ZC (387646983@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

/*
 * eLesson Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_PWM_H
#define ELAB_PWM_H

/* includes ----------------------------------------------------------------- */
#include "../elab_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/* public typedef ----------------------------------------------------------- */
typedef struct elab_pwm {
    elab_device_t super;            // 父类:设备基类
    const struct elab_pwm_ops *ops; // 接口方法
    uint32_t frequency;             // 频率 (Hz)
    uint32_t duty_cycle;            // 占空比 (0-10000, 对应0%-100%)
} elab_pwm_t;                       // PWM抽象设备

typedef struct elab_pwm_ops {
    elab_err_t (*init)(elab_pwm_t *const me);
    elab_err_t (*set_frequency)(elab_pwm_t *const me, uint32_t frequency);
    elab_err_t (*get_frequency)(elab_pwm_t *const me, uint32_t *frequency);
    elab_err_t (*set_duty_cycle)(elab_pwm_t *const me, uint32_t duty_cycle);
    elab_err_t (*get_duty_cycle)(elab_pwm_t *const me, uint32_t *duty_cycle);
} elab_pwm_ops_t;

#define ELAB_PWM_CAST(_dev) ((elab_pwm_t *)_dev)

/* public define ------------------------------------------------------------ */
#define PWM_DUTY_CYCLE_MAX (10000) // 占空比最大值，对应100%

/* public functions --------------------------------------------------------- */
/* For low-level driver. */
void elab_pwm_register(elab_pwm_t *const me,
                       const char *name,
                       const elab_pwm_ops_t *ops,
                       void *user_data);

/* For high-level code. */
elab_err_t elab_pwm_set_frequency(elab_device_t *const me, uint32_t frequency);
elab_err_t elab_pwm_get_frequency(elab_device_t *const me, uint32_t *frequency);
elab_err_t elab_pwm_set_duty_cycle(elab_device_t *const me, uint32_t duty_cycle);
elab_err_t elab_pwm_get_duty_cycle(elab_device_t *const me, uint32_t *duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /* ELAB_PWM_H */

/* ----------------------------- end of file -------------------------------- */