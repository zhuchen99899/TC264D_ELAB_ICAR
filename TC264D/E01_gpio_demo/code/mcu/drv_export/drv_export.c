#include "../driver/drv_pin.h"
#include "../driver/drv_uart.h"
#include "../../3rd/elab/common/elab_export.h"


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
    elab_driver_uart_init(&uart1,"uart1",115200,
                    UART1_TX_P15_0,UART1_RX_P15_1,UART_1);
    __device_enable(&uart1.device.super,true);
}

INIT_EXPORT(dirver_uart_export, EXPORT_DRVIVER);
