/**
 * @file drv_motor.h
 * @author GitHub Copilot
 * @brief 电机驱动层头文件
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __DRIVER_MOTOR_H__
#define __DRIVER_MOTOR_H__

#include "../../3rd/elab/edf/user/elab_motor.h"
#include "drv_pin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 电机驱动结构体
 */
typedef struct elab_motor_driver {
    elab_motor_t device;        // 电机设备对象
    elab_pin_driver_t *pin_in1; // 方向控制引脚1
    elab_pin_driver_t *pin_in2; // 方向控制引脚2
    elab_pin_driver_t *pin_ena; // 使能引脚（PWM控制）
    uint32_t max_speed;         // 最大速度
    uint32_t min_speed;         // 最小速度
    bool is_enabled;            // 使能状态
} elab_motor_driver_t;

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
                            elab_pin_driver_t *pin_ena);

#ifdef __cplusplus
}
#endif

#endif //__DRIVER_MOTOR_H__
