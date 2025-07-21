/**
 * @file drv_motor.h
 * @author GitHub Copilot
 * @brief 电机驱动层头文件
 * @version 0.1
 * @date 2025-07-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __DRIVER_MOTOR_H__
#define __DRIVER_MOTOR_H__

#include "../../3rd/elab/edf/user/elab_motor.h"
#include "drv_pin.h"
#include "drv_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct pid_s{
    float kp; // 比例系数
    float ki; // 积分系数
    float kd; // 微分系数
    float setpoint; // 目标值
    float integral; // 积分值
    float last_error; // 上一次误差
}pid_t;

/* public typedef ----------------------------------------------------------- */
/**
 * @brief 电机驱动结构体
 */
typedef struct elab_motor_driver {
    elab_motor_t device;        // 电机设备对象（父类）
    elab_pin_driver_t *pin_in1; // 方向控制引脚1
    elab_pin_driver_t *pin_in2; // 方向控制引脚2
    elab_pwm_driver_t *pin_ena; // 使能PWM（速度控制，可选）
    float max_speed;         // 最大速度（0-100）
    float min_speed;         // 最小速度（0-100）
    pid_t pid; // PID控制参数
} elab_motor_driver_t;

/* public functions --------------------------------------------------------- */
/**
 * @brief 初始化电机驱动
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
                            elab_pwm_driver_t *pin_ena);

#ifdef __cplusplus
}
#endif

#endif //__DRIVER_MOTOR_H__

/* ----------------------------- end of file -------------------------------- */
