/**
 * @file elab_pwm.c
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

/* includes ----------------------------------------------------------------- */
#include "elab_pwm.h"
#include "../../common/elab_assert.h"
#include "../../common/elab_log.h"

#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("Edf_Pwm");

/* public functions --------------------------------------------------------- */
/**
 * @brief Register elab pwm device to edf framework.
 * @param me       PWM device handle.
 * @param name     PWM device name.
 * @param ops      PWM device operations.
 * @param user_data The pointer of private data.
 * @retval None.
 */
void elab_pwm_register(elab_pwm_t *const me,
                       const char *name,
                       const elab_pwm_ops_t *ops,
                       void *user_data)
{
    elab_assert(me != NULL);
    elab_assert(name != NULL);
    elab_assert(ops != NULL);

    memset(me, 0, sizeof(elab_pwm_t));

    /* PWM class data */
    me->ops        = ops;
    me->frequency  = 1000;  // 默认频率1kHz
    me->duty_cycle = 0;     // 默认占空比0%

    /* Super class data */
    elab_device_t *device = &(me->super);
    device->ops           = NULL;
    device->user_data     = user_data;

    /* Register PWM device */
    elab_device_attr_t attr =
        {
            .name = name,
            .sole = true,
            .type = ELAB_DEVICE_PWM,
        };
    elab_device_register(device, &attr);

    /* PWM device initialization */
    if (me->ops->init != NULL) {
        me->ops->init(me);
    }

    ELAB_LOG_D("PWM device [%s] registered successfully", name);
}

/**
 * @brief Set PWM frequency.
 * @param me        PWM device handle.
 * @param frequency PWM frequency in Hz.
 * @retval Error code.
 */
elab_err_t elab_pwm_set_frequency(elab_device_t *const me, uint32_t frequency)
{
    elab_assert(me != NULL);
    elab_assert(frequency > 0);
    elab_assert(elab_device_is_enabled(me));

    elab_pwm_t *pwm = ELAB_PWM_CAST(me);
    elab_assert(pwm->ops != NULL);
    elab_assert(pwm->ops->set_frequency != NULL);

    elab_err_t ret = ELAB_OK;

    elab_device_lock(me);
    ret = pwm->ops->set_frequency(pwm, frequency);
    if (ret == ELAB_OK) {
        pwm->frequency = frequency;
    }
    elab_device_unlock(me);

    return ret;
}

/**
 * @brief Get PWM frequency.
 * @param me        PWM device handle.
 * @param frequency Pointer to store frequency value.
 * @retval Error code.
 */
elab_err_t elab_pwm_get_frequency(elab_device_t *const me, uint32_t *frequency)
{
    elab_assert(me != NULL);
    elab_assert(frequency != NULL);
    elab_assert(elab_device_is_enabled(me));

    elab_pwm_t *pwm = ELAB_PWM_CAST(me);
    elab_assert(pwm->ops != NULL);

    elab_err_t ret = ELAB_OK;

    elab_device_lock(me);
    if (pwm->ops->get_frequency != NULL) {
        ret = pwm->ops->get_frequency(pwm, frequency);
    } else {
        *frequency = pwm->frequency;
    }
    elab_device_unlock(me);
    return ret;
}

/**
 * @brief Set PWM duty cycle.
 * @param me         PWM device handle.
 * @param duty_cycle PWM duty cycle (0-PWM_DUTY_CYCLE_MAX).
 * @retval Error code.
 */
elab_err_t elab_pwm_set_duty_cycle(elab_device_t *const me, uint32_t duty_cycle)
{
    elab_assert(me != NULL);
    elab_assert(duty_cycle <= PWM_DUTY_CYCLE_MAX);
    elab_assert(elab_device_is_enabled(me));

    elab_pwm_t *pwm = ELAB_PWM_CAST(me);
    elab_assert(pwm->ops != NULL);
    elab_assert(pwm->ops->set_duty_cycle != NULL);

    elab_err_t ret = ELAB_OK;

    elab_device_lock(me);
    ret = pwm->ops->set_duty_cycle(pwm, duty_cycle);
    if (ret == ELAB_OK) {
        pwm->duty_cycle = duty_cycle;
    }
    elab_device_unlock(me);

    return ret;
}

/**
 * @brief Get PWM duty cycle.
 * @param me         PWM device handle.
 * @param duty_cycle Pointer to store duty cycle value.
 * @retval Error code.
 */
elab_err_t elab_pwm_get_duty_cycle(elab_device_t *const me, uint32_t *duty_cycle)
{
    elab_assert(me != NULL);
    elab_assert(duty_cycle != NULL);
    elab_assert(elab_device_is_enabled(me));

    elab_pwm_t *pwm = ELAB_PWM_CAST(me);
    elab_assert(pwm->ops != NULL);

    elab_err_t ret = ELAB_OK;

    elab_device_lock(me);
    if (pwm->ops->get_duty_cycle != NULL) {
        ret = pwm->ops->get_duty_cycle(pwm, duty_cycle);
    } else {
        *duty_cycle = pwm->duty_cycle;
    }
    elab_device_unlock(me);

    return ret;
}

#ifdef __cplusplus
}
#endif

/* ----------------------------- end of file -------------------------------- */