/**
 * @file drv_pwm.c
 * @author GitHub Copilot
 * @brief PWM驱动层实现文件
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "drv_pwm.h"
#include "../../3rd/elab/common/elab_assert.h"
#include "../../3rd/elab/common/elab_log.h"
#include "../../3rd/elab/common/elab_def.h"

#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("driver_pwm");

/* private function prototype ----------------------------------------------- */
static elab_err_t _init(elab_pwm_t *const me);
static elab_err_t _set_frequency(elab_pwm_t *const me, uint32_t frequency);
static elab_err_t _get_frequency(elab_pwm_t *const me, uint32_t *frequency);
static elab_err_t _set_duty_cycle(elab_pwm_t *const me, uint32_t duty_cycle);
static elab_err_t _get_duty_cycle(elab_pwm_t *const me, uint32_t *duty_cycle);

/* private variables -------------------------------------------------------- */
static const elab_pwm_ops_t pwm_driver_ops = {
    .init           = _init,
    .set_frequency  = _set_frequency,
    .get_frequency  = _get_frequency,
    .set_duty_cycle = _set_duty_cycle,
    .get_duty_cycle = _get_duty_cycle,
};

/* public functions --------------------------------------------------------- */
/**
 * @brief PWM驱动初始化函数
 * @param me PWM驱动对象指针
 * @param name PWM设备名称
 * @param pwm_pin PWM通道引脚
 */
void elab_driver_pwm_init(elab_pwm_driver_t *me, const char *name,
                          pwm_channel_enum pwm_pin, uint32_t frequency, uint32_t duty)
{
    elab_assert(me != NULL);
    elab_assert(name != NULL);

    me->pwm_pin   = pwm_pin;
    me->frequency = frequency; // 默认频率1kHz
    me->duty      = duty;      // 默认占空比0%

    elab_pwm_register(&me->device, name, &pwm_driver_ops, me);

    elog_debug("PWM driver [%s] initialized with pin %d", name, pwm_pin);
}

/* private functions -------------------------------------------------------- */
/**
 * @brief PWM硬件初始化函数
 * @param me PWM设备对象指针
 * @return 错误码
 */
static elab_err_t _init(elab_pwm_t *const me)
{
    elab_assert(me != NULL);
    elab_pwm_driver_t *driver = (elab_pwm_driver_t *)me->super.user_data;
    elog_debug("Initializing PWM hardware for pin %d", driver->pwm_pin);

    // 初始化PWM - 默认频率1kHz，占空比0%
    pwm_init(driver->pwm_pin, driver->frequency, driver->duty);

    elog_debug("PWM hardware initialized successfully");
    return ELAB_OK;
}

/**
 * @brief 设置PWM频率
 * @param me PWM设备对象指针
 * @param frequency 频率值(Hz)
 * @return 错误码
 */
static elab_err_t _set_frequency(elab_pwm_t *const me, uint32_t frequency)
{
    elab_assert(me != NULL);
    elab_pwm_driver_t *driver = (elab_pwm_driver_t *)me->super.user_data;

    if (frequency == 0) {
        elog_error("Invalid frequency: %d", frequency);
        return ELAB_ERR_INVALID;
    }

    elog_debug("Setting PWM frequency to: %d Hz", frequency);

    // 重新初始化PWM以更改频率，保持当前占空比
    pwm_init(driver->pwm_pin, frequency, driver->duty);

    driver->frequency = frequency;
    me->frequency     = frequency;

    return ELAB_OK;
}

/**
 * @brief 获取PWM频率
 * @param me PWM设备对象指针
 * @param frequency 频率值指针
 * @return 错误码
 */
static elab_err_t _get_frequency(elab_pwm_t *const me, uint32_t *frequency)
{
    elab_assert(me != NULL);
    elab_assert(frequency != NULL);
    elab_pwm_driver_t *driver = (elab_pwm_driver_t *)me->super.user_data;
    *frequency                = driver->frequency;

    elog_debug("Current PWM frequency: %d Hz", *frequency);

    return ELAB_OK;
}

/**
 * @brief 设置PWM占空比
 * @param me PWM设备对象指针
 * @param duty_cycle 占空比值(0-PWM_DUTY_CYCLE_MAX)
 * @return 错误码
 */
static elab_err_t _set_duty_cycle(elab_pwm_t *const me, uint32_t duty_cycle)
{
    elab_assert(me != NULL);

    elab_pwm_driver_t *driver = (elab_pwm_driver_t *)me->super.user_data;
    if (duty_cycle > PWM_DUTY_CYCLE_MAX) {
        elog_error("Invalid duty cycle: %d (max: %d)", duty_cycle, PWM_DUTY_CYCLE_MAX);
        return ELAB_ERR_INVALID;
    }

    elog_debug("Setting PWM duty cycle to: %d/%d (%.1f%%)",
               duty_cycle, PWM_DUTY_CYCLE_MAX,
               (float)duty_cycle * 100.0f / PWM_DUTY_CYCLE_MAX);

    // 将elab的占空比范围(0-10000)转换为zf库的范围(0-PWM_DUTY_MAX)
    uint32_t zf_duty = (uint32_t)((uint64_t)duty_cycle * PWM_DUTY_MAX / PWM_DUTY_CYCLE_MAX);

    pwm_set_duty(driver->pwm_pin, zf_duty);

    driver->duty = duty_cycle;

    return ELAB_OK;
}

/**
 * @brief 获取PWM占空比
 * @param me PWM设备对象指针
 * @param duty_cycle 占空比值指针
 * @return 错误码
 */
static elab_err_t _get_duty_cycle(elab_pwm_t *const me, uint32_t *duty_cycle)
{
    elab_assert(me != NULL);
    elab_assert(duty_cycle != NULL);
    elab_pwm_driver_t *driver = (elab_pwm_driver_t *)me->super.user_data;

    // 将zf库的占空比范围转换回elab的范围
    *duty_cycle = (uint32_t)((uint64_t)driver->duty * PWM_DUTY_CYCLE_MAX / PWM_DUTY_MAX);

    elog_debug("Current PWM duty cycle: %d/%d (%.1f%%)",
               *duty_cycle, PWM_DUTY_CYCLE_MAX,
               (float)*duty_cycle * 100.0f / PWM_DUTY_CYCLE_MAX);

    return ELAB_OK;
}

#ifdef __cplusplus
}
#endif

/* ----------------------------- end of file -------------------------------- */
