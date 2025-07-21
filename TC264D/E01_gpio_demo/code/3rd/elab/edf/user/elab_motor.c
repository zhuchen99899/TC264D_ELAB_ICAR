/**
 * @file elab_motor.c
 * @author ZC (387646983@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "elab_motor.h"
#include "../../common/elab_assert.h"
#include "../../common/elab_log.h"

ELAB_TAG("EdfMotor");
/**
 * @brief 初始化电机对象
 * @param me 电机对象指针
 * @param name 设备名称
 * @param ops 操作函数指针
 * @param user_data 用户数据
 */
void elab_motor_init(elab_motor_t *const me, const char *name,
                     elab_motor_ops_t *ops, void *user_data)
{
    elab_assert(me != NULL);
    elab_assert(ops != NULL);

    /* 初始化电机属性 */
    me->speed     = 0;
    me->direction = ELAB_MOTOR_DIRECTION_STOP;
    me->ops       = ops;

    /* 初始化设备基类 */

    elab_device_attr_t attr_motor =
        {
            .name = name,
            .sole = false,
            .type = ELAB_DEVICE_UNKNOWN,
        };
    elab_device_register(&me->super, &attr_motor);
        //驱动数据注册到设备层
    me->super.user_data = user_data;
    me->super.ops = NULL;

    /* 调用硬件初始化 */
    if (me->ops->init != NULL) {
        me->ops->init(me);
    }
}

/**
 * @brief 紧急停止电机
 * @param me 设备对象指针
 * @return 错误码
 */
elab_err_t elab_motor_emg_stop(elab_device_t *const me)
{
    elab_assert(me != NULL);

    elab_motor_t *motor = ELAB_MOTOR_CAST(me);

    if (motor->ops->elab_motor_emg_stop == NULL) {
        return ELAB_ERR_EMPTY;
    }

    elab_err_t ret = motor->ops->elab_motor_emg_stop(motor);

    if (ret == ELAB_OK) {
        motor->speed     = 0;
        motor->direction = ELAB_MOTOR_DIRECTION_STOP;
    }

    return ret;
}

/**
 * @brief 设置电机速度
 * @param me 设备对象指针
 * @param speed 速度值（浮点型）
 * @return 错误码
 */
elab_err_t elab_motor_set_speed(elab_device_t *const me, float speed)
{
    elab_assert(me != NULL);

    elab_motor_t *motor = ELAB_MOTOR_CAST(me);


    if (motor->ops->set_speed == NULL) {
        return ELAB_ERR_EMPTY;
    }


    elab_err_t ret = motor->ops->set_speed(motor, speed);

    if (ret == ELAB_OK) {
        motor->speed = speed;
    
    }

    return ret;
}

/**
 * @brief 获取电机速度
 * @param me 设备对象指针
 * @param speed 速度值指针（浮点型）
 * @return 错误码
 */
elab_err_t elab_motor_get_speed(elab_device_t *const me, float *speed)
{
    elab_assert(me != NULL);
    elab_assert(speed != NULL);

    elab_motor_t *motor = ELAB_MOTOR_CAST(me);

    if (motor->ops->get_speed == NULL) {
        /* 如果没有硬件获取函数，返回缓存的速度值 */
        *speed = (float)motor->speed;
        return ELAB_OK;
    }

    float get_speed;
    elab_err_t ret = motor->ops->get_speed(motor, &get_speed);

    if (ret == ELAB_OK) {
        *speed       = (float)get_speed;
        motor->speed = get_speed;
    }

    return ret;
}

/**
 * @brief 设置电机方向
 * @param me 设备对象指针
 * @param direction 方向
 * @return 错误码
 */
elab_err_t elab_motor_set_direction(elab_device_t *const me, elab_motor_direction_t direction)
{
    elab_assert(me != NULL);

    elab_motor_t *motor = ELAB_MOTOR_CAST(me);


    if (motor->ops->set_direction == NULL) {
        return ELAB_ERR_EMPTY;
    }

    elab_err_t ret = motor->ops->set_direction(motor, direction);

    if (ret == ELAB_OK) {
        motor->direction = direction;
        elog_debug("Motor direction set to: %d", direction);
    }

    return ret;
}