// /**
//  * @file motor_test.c
//  * @author GitHub Copilot
//  * @brief 电机驱动测试示例
//  * @version 0.1
//  * @date 2025-07-17
//  *
//  * @copyright Copyright (c) 2025
//  *
//  */

// #include "../../3rd/elab/edf/elab_device.h"
// #include "../../3rd/elab/edf/user/elab_motor.h"
// #include "../../3rd/elab/common/elab_log.h"
// #include "../../3rd/elab/common/elab_common.h"

// ELAB_TAG("motor_test");

// /**
//  * @brief 电机测试函数
//  */
// void motor_test_demo(void)
// {
//     ELAB_LOG_I("Starting motor test demo...");

//     // 查找电机设备
//     elab_device_t *motor_dev = elab_device_find("motor1");
//     if (motor_dev == NULL) {
//         ELAB_LOG_E("Motor device 'motor1' not found!");
//         return;
//     }

//     ELAB_LOG_I("Motor device found: %s", motor_dev->attr.name);

//     // 测试电机控制
//     elab_err_t ret;

//     // 1. 设置正转方向
//     ELAB_LOG_I("Testing motor forward direction...");
//     ret = elab_motor_set_direction(motor_dev, ELAB_MOTOR_DIRECTION_FORWARD);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to set motor direction forward");
//         return;
//     }

//     // 2. 设置速度为50%
//     ELAB_LOG_I("Setting motor speed to 50...");
//     ret = elab_motor_set_speed(motor_dev, 50.0f);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to set motor speed");
//         return;
//     }

//     // 延时2秒
//     elab_delay_ms(2000);

//     // 3. 获取当前速度
//     float current_speed;
//     ret = elab_motor_get_speed(motor_dev, &current_speed);
//     if (ret == ELAB_OK) {
//         ELAB_LOG_I("Current motor speed: %.1f", current_speed);
//     }

//     // 4. 停止电机
//     ELAB_LOG_I("Stopping motor...");
//     ret = elab_motor_set_direction(motor_dev, ELAB_MOTOR_DIRECTION_STOP);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to stop motor");
//         return;
//     }

//     // 延时1秒
//     elab_delay_ms(1000);

//     // 5. 设置反转方向
//     ELAB_LOG_I("Testing motor backward direction...");
//     ret = elab_motor_set_direction(motor_dev, ELAB_MOTOR_DIRECTION_BACKWARD);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to set motor direction backward");
//         return;
//     }

//     // 6. 设置速度为30%
//     ELAB_LOG_I("Setting motor speed to 30...");
//     ret = elab_motor_set_speed(motor_dev, 30.0f);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to set motor speed");
//         return;
//     }

//     // 延时2秒
//     elab_delay_ms(2000);

//     // 7. 紧急停止测试
//     ELAB_LOG_I("Testing emergency stop...");
//     ret = elab_motor_emg_stop(motor_dev);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to emergency stop motor");
//         return;
//     }

//     ELAB_LOG_I("Motor test demo completed successfully!");
// }

// /**
//  * @brief 电机控制命令处理函数
//  * @param direction 方向：0-停止，1-正转，2-反转
//  * @param speed 速度：0-100
//  */
// void motor_control_cmd(int direction, int speed)
// {
//     elab_device_t *motor_dev = elab_device_find("motor1");
//     if (motor_dev == NULL) {
//         ELAB_LOG_E("Motor device not found!");
//         return;
//     }

//     elab_err_t ret;

//     // 设置方向
//     elab_motor_direction_t motor_dir;
//     switch (direction) {
//         case 0:
//             motor_dir = ELAB_MOTOR_DIRECTION_STOP;
//             ELAB_LOG_I("Motor command: STOP");
//             break;
//         case 1:
//             motor_dir = ELAB_MOTOR_DIRECTION_FORWARD;
//             ELAB_LOG_I("Motor command: FORWARD, Speed: %d", speed);
//             break;
//         case 2:
//             motor_dir = ELAB_MOTOR_DIRECTION_BACKWARD;
//             ELAB_LOG_I("Motor command: BACKWARD, Speed: %d", speed);
//             break;
//         default:
//             ELAB_LOG_E("Invalid motor direction: %d", direction);
//             return;
//     }

//     ret = elab_motor_set_direction(motor_dev, motor_dir);
//     if (ret != ELAB_OK) {
//         ELAB_LOG_E("Failed to set motor direction");
//         return;
//     }

//     // 设置速度（只有在非停止状态下）
//     if (motor_dir != ELAB_MOTOR_DIRECTION_STOP) {
//         ret = elab_motor_set_speed(motor_dev, (float)speed);
//         if (ret != ELAB_OK) {
//             ELAB_LOG_E("Failed to set motor speed");
//             return;
//         }
//     }

//     ELAB_LOG_I("Motor control command executed successfully");
// }
