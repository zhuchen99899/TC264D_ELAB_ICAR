/**
 * @file drv_motor.c
 * @author GitHub Copilot
 * @brief 电机驱动层实现文件
 * @version 0.1
 * @date 2025-07-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "drv_motor.h"
#include "../../3rd/elab/common/elab_assert.h"
#include "../../3rd/elab/common/elab_log.h"
#include "../../3rd/elab/common/elab_def.h"

#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("driver_motor");

/* private function prototype ----------------------------------------------- */
static elab_err_t _init(elab_motor_t *const me);
static elab_err_t _set_speed(elab_motor_t *const me, float speed);
static elab_err_t _get_speed(elab_motor_t *const me, float *speed);
static elab_err_t _set_direction(elab_motor_t *const me, elab_motor_direction_t direction);
static elab_err_t _emg_stop(elab_motor_t *const me);

/* private variables -------------------------------------------------------- */
static const elab_motor_ops_t motor_driver_ops = {
    .init                = _init,
    .set_speed           = _set_speed,
    .get_speed           = _get_speed,
    .set_direction       = _set_direction,
    .elab_motor_emg_stop = _emg_stop,
};

/* public functions --------------------------------------------------------- */
/**
 * @brief 初始化电机驱动（数字控制版本）
 * @param me 电机驱动对象指针
 * @param name 电机设备名称
 * @param pin_in1 方向控制引脚1
 * @param pin_in2 方向控制引脚2
 * @param pin_ena 使能引脚
 */
void elab_driver_motor_init(elab_motor_driver_t *me,
                            const char *name,
                            elab_pin_driver_t *pin_in1,
                            elab_pin_driver_t *pin_in2,
                            elab_pwm_driver_t *pin_ena)
{
    elab_assert(me != NULL);
    elab_assert(name != NULL);
    elab_assert(pin_in1 != NULL);
    elab_assert(pin_in2 != NULL);
    elab_assert(pin_ena != NULL);

    // 初始化电机驱动参数
    me->pin_in1   = pin_in1;
    me->pin_in2   = pin_in2;
    me->pin_ena   = pin_ena;
    me->max_speed = 100; // 默认最大速度
    me->min_speed = 0;   // 默认最小速度

    // 注册电机组件到elab框架
    elab_motor_init(&me->device, name, (elab_motor_ops_t *)&motor_driver_ops, me);

    elog_info("Motor driver [%s] initialized successfully", name);
}

/* private functions -------------------------------------------------------- */
/**
 * @brief 电机硬件初始化函数
 * @param me 电机设备对象指针
 * @return 错误码
 */
static elab_err_t _init(elab_motor_t *const me)
{

    elab_assert(me != NULL);

    elab_motor_driver_t *driver = (elab_motor_driver_t *)me->super.user_data;

    elog_debug("Initializing motor hardware...");

    // 初始化方向控制引脚为输出模式
    elab_pin_set_mode(&driver->pin_in1->device.super, PIN_MODE_OUTPUT_PP);
    elab_pin_set_mode(&driver->pin_in2->device.super, PIN_MODE_OUTPUT_PP);

    elab_pwm_set_frequency(&driver->pin_ena->device.super, 17000); // 17kHz PWM频率
    elab_pwm_set_duty_cycle(&driver->pin_ena->device.super, 0);    // 初始占空比为0

    // 初始状态：停止
    elab_pin_set_status(&driver->pin_in1->device.super, false); // 0
    elab_pin_set_status(&driver->pin_in2->device.super, false); // 0

    elog_debug("Motor hardware initialized");
    return ELAB_OK;
}

/**
 * @brief 设置电机速度
 * @param me 电机设备对象指针
 * @param speed 速度值（0-100）
 * @return 错误码
 */
static elab_err_t _set_speed(elab_motor_t *const me, float speed)
{
    elab_assert(me != NULL);
    elab_motor_driver_t *driver = (elab_motor_driver_t *)me->super.user_data;

    // 限制速度范围
    if (speed > driver->max_speed) speed = driver->max_speed;
    if (speed < driver->min_speed) speed = driver->min_speed;

    elog_debug("Setting motor speed to: %f", speed);

    // PWM模式：设置占空比
    if (speed > 0) {
        // 将速度（0-100）转换为PWM占空比（0-10000）
        uint32_t duty_cycle = (uint32_t)((speed * 10000) / 100);
        elab_pwm_set_duty_cycle(&driver->pin_ena->device.super, duty_cycle);
        elog_debug("Motor PWM duty cycle set to: %d (%.1f%%)", duty_cycle, (float)speed);
    } else {
        elab_pwm_set_duty_cycle(&driver->pin_ena->device.super, 0);
    }

    return ELAB_OK;
}

/**
 * @brief 获取电机速度
 * @param me 电机设备对象指针
 * @param speed 速度值指针
 * @return 错误码
 */
static elab_err_t _get_speed(elab_motor_t *const me, float *speed)
{
    elab_assert(me != NULL);
    elab_assert(speed != NULL);

    // 返回当前速度（从电机对象中获取）
    *speed = me->speed;
    elog_debug("Current motor speed: %d", *speed);
    return ELAB_OK;
}

/**
 * @brief 设置电机方向
 * @param me 电机设备对象指针
 * @param direction 方向
 * @return 错误码
 */
static elab_err_t _set_direction(elab_motor_t *const me, elab_motor_direction_t direction)
{
    elab_assert(me != NULL);
    elab_motor_driver_t *driver = (elab_motor_driver_t *)me->super.user_data;

    elog_debug("Setting motor direction: %d", direction);

    switch (direction) {
        case ELAB_MOTOR_DIRECTION_FORWARD:
            elab_pin_set_status(&driver->pin_in1->device.super, true);
            elab_pin_set_status(&driver->pin_in2->device.super, false);
            elog_debug("Motor direction: FORWARD");
            break;

        case ELAB_MOTOR_DIRECTION_BACKWARD:
            elab_pin_set_status(&driver->pin_in1->device.super, false);
            elab_pin_set_status(&driver->pin_in2->device.super, true);
            elog_debug("Motor direction: BACKWARD");
            break;

        case ELAB_MOTOR_DIRECTION_STOP:
        default:
            elab_pin_set_status(&driver->pin_in1->device.super, false);
            elab_pin_set_status(&driver->pin_in2->device.super, false);
            elog_debug("Motor direction: STOP");
            break;
    }

    return ELAB_OK;
}

/**
 * @brief 电机紧急停止
 * @param me 电机设备对象指针
 * @return 错误码
 */
static elab_err_t _emg_stop(elab_motor_t *const me)
{
    elab_assert(me != NULL);
    elab_motor_driver_t *driver = (elab_motor_driver_t *)me->super.user_data;

    elog_warn("Motor emergency stop!");

    // 紧急停止：立即关闭所有输出

    elab_pwm_set_duty_cycle(&driver->pin_ena->device.super, 0);

    elab_pin_set_status(&driver->pin_in1->device.super, false);
    elab_pin_set_status(&driver->pin_in2->device.super, false);

    return ELAB_OK;
}

#ifdef __cplusplus
}
#endif

/* ----------------------------- end of file -------------------------------- */
