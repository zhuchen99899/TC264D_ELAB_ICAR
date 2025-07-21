#include "../driver/drv_pin.h"
#include "../driver/drv_uart.h"
#include "../driver/drv_motor.h"
#include "../driver/drv_pwm.h"
#include "../../3rd/elab/common/elab_export.h"
#include "isr_config.h"
#include "zf_common_headfile.h"
#include "../../component/ringbuf/ringbuf.h"
static elab_pin_driver_t pin_led1;

static void driver_pin_mcu_export(void)
{
    elab_driver_pin_init(&pin_led1, "led1_pin", P21_4);
    elab_pin_set_mode(&pin_led1.device.super, PIN_MODE_OUTPUT_PP);
}

INIT_EXPORT(driver_pin_mcu_export, EXPORT_DRVIVER);

static elab_uart_driver_t uart1;
static void dirver_uart_export(void)
{
    elab_driver_uart_init(&uart1, "uart1", 115200,
                          UART1_TX_P15_0, UART1_RX_P15_1, UART_1);
    __device_enable(&uart1.device.super, true);
}

INIT_EXPORT(dirver_uart_export, EXPORT_DRVIVER);

/* Motor driver export -------------------------------------------------------- */
static elab_pwm_driver_t pwm1;
static elab_pin_driver_t motor1_pin_in1;
static elab_pin_driver_t motor1_pin_in2;
static elab_motor_driver_t motor1;
static void driver_motor_export(void)
{
    // 初始化PWM设备
    elab_driver_pwm_init(&pwm1, "pwm1", ATOM0_CH7_P02_7, 17000, 0);
    elab_driver_pin_init(&motor1_pin_in1, "motor1_in1", P21_2);
    elab_driver_pin_init(&motor1_pin_in2, "motor1_in2", P21_3);


    elab_driver_motor_init(&motor1, "motor1",
                           &motor1_pin_in1, &motor1_pin_in2, &pwm1);
    elab_motor_set_direction(&motor1.device.super, ELAB_MOTOR_DIRECTION_FORWARD);
    elab_motor_set_speed(&motor1.device.super, 50); // 设置速度为50%
    //elab_pwm_set_duty_cycle(&pwm1.device, 5000); // 设置占空比为50%
}

INIT_EXPORT(driver_motor_export, EXPORT_DRVIVER);

/* Motor driver export ------------------------------------------------------ */


/**uart1 接收中断 */
uint8_t get_data;
void uart_rx_interrupt_handler(void)
{
    uart_query_byte(uart1.uart_index, &get_data);
    ringbuffer_put(&uart1.device.rx_ringbuf, &get_data, 1);
}

// IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
// {
//     interrupt_global_enable(0);                     // 开启中断嵌套

// }

IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);  // 开启中断嵌套
    uart_rx_interrupt_handler(); // 串口接收处理
}
