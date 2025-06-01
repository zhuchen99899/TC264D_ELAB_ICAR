#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "../../3rd/elab/edf/normal/elab_serial.h"
#include "zf_driver_uart.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct elab_uart_driver
{
    elab_serial_t device;
    uart_tx_pin_enum tx_pin; //引脚
    uart_rx_pin_enum rx_pin; //引脚
    uart_index_enum uart_index; //串口枚举号
    uint32_t baudrate; //波特率
} elab_uart_driver_t;


void elab_driver_uart_init(elab_uart_driver_t *me,const char *name,uint32_t boundrate,
                     uart_tx_pin_enum tx,uart_rx_pin_enum rx,uart_index_enum uart_index);


#ifdef __cplusplus
}
#endif

#endif // __DRV_UART_H
