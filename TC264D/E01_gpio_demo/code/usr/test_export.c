#include "elab_export.h"
#include "elab_log.h"
#include "elab_device.h"
#include "cmsis_os2.h"
#include "../3rd/elab/edf/normal/elab_serial.h"
#include "serial1_ringbuf_handle.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST, EXPORT_DRVIVER);

void init_uart_handler_export(void)
{

    elab_device_t *uart1 = elab_device_find("uart1");
    elab_serial_rx_ringbuf_handler_register(uart1, _ringbuf_handler);
}
INIT_EXPORT(init_uart_handler_export, EXPORT_APP);

static void test_message_queue(void);
static void test_uart1_send(void);

static void _timer_func(void *argument)
{

    // test_uart1_send();
}

static const osTimerAttr_t test_timer_attr_led =
    {
        .name      = "test_timer",
        .attr_bits = 0,
        .cb_mem    = NULL,
        .cb_size   = 0,
};
static osTimerId_t test_timer;
void init_test_timer(void)
{
    test_timer = osTimerNew(_timer_func, osTimerPeriodic, NULL, &test_timer_attr_led);

    osTimerStart(test_timer, 10);
}
INIT_EXPORT(init_test_timer, EXPORT_MIDWARE);

void test_uart1_send(void)
{
    elab_device_t *uart1 = elab_device_find("uart1");
    uint8_t data[]       = "Hello, UART1!";
    elab_serial_write(uart1, data, sizeof(data));
}
