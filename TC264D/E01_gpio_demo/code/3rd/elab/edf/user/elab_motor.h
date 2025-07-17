/**
 * @file elab_motor.h
 * @author ZC (387646983@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __ELAB_MOTOR_H__
#define __ELAB_MOTOR_H__
/* include ------------------------------------------------------------------ */
#include "../elab_device.h"

/* 电机方向枚举 */
typedef enum {
    ELAB_MOTOR_DIRECTION_STOP = 0, // 停止
    ELAB_MOTOR_DIRECTION_FORWARD,  // 正转
    ELAB_MOTOR_DIRECTION_BACKWARD, // 反转
} elab_motor_direction_t;

/* 电机状态枚举 */
typedef enum {
    ELAB_MOTOR_STATE_IDLE = 0,       // 空闲
    ELAB_MOTOR_STATE_RUNNING,        // 运行中
    ELAB_MOTOR_STATE_ERROR,          // 错误状态
    ELAB_MOTOR_STATE_EMERGENCY_STOP, // 紧急停止
} elab_motor_state_t;

typedef struct elab_motor_s {
    elab_device_t super;
    int speed;                        // 电机速度
    elab_motor_direction_t direction; // 电机方向
    elab_motor_state_t state;         // 电机状态
    struct elab_motor_ops_s *ops;     // 电机操作函数指针
} elab_motor_t;
#define ELAB_MOTOR_CAST(_dev) ((elab_motor_t *)_dev)

typedef struct elab_motor_ops_s {
    elab_err_t (*init)(elab_motor_t *const me);
    elab_err_t (*set_speed)(elab_motor_t *const me, int speed);
    elab_err_t (*get_speed)(elab_motor_t *const me, int *speed);
    elab_err_t (*set_direction)(elab_motor_t *const me, elab_motor_direction_t direction);
    elab_err_t (*elab_motor_emg_stop)(elab_motor_t *const me);
    elab_err_t (*enable)(elab_motor_t *const me, bool status);
} elab_motor_ops_t;

elab_err_t elab_motor_enable(elab_device_t *const me, bool status);
elab_err_t elab_motor_emg_stop(elab_device_t *const me);
elab_err_t elab_motor_set_speed(elab_device_t *const me, float speed);
elab_err_t elab_motor_get_speed(elab_device_t *const me, float *speed);
elab_err_t elab_motor_set_direction(elab_device_t *const me, elab_motor_direction_t direction);

/* For exporting. */
void elab_motor_init(elab_motor_t *const me, const char *name,
                     elab_motor_ops_t *ops, void *user_data);

#endif
