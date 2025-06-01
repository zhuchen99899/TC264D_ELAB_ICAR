#include "elab_export.h"
#include "elab_log.h"
#include "elab_device.h"
#include "cmsis_os2.h"
#include "../3rd/elab/edf/normal/elab_serial.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST,EXPORT_DRVIVER);



static void test_message_queue(void);
static void test_uart1_send(void);



static void _timer_func(void *argument){

    test_uart1_send();

}



static const osTimerAttr_t test_timer_attr_led =
{
    .name = "test_timer",
    .attr_bits = 0,
    .cb_mem = NULL,
    .cb_size = 0,
};
static osTimerId_t test_timer;
void init_test_timer(void)
{
    test_timer = osTimerNew(_timer_func, osTimerPeriodic, NULL, &test_timer_attr_led);

    osTimerStart(test_timer, 10);
}
INIT_EXPORT(init_test_timer,EXPORT_MIDWARE);



void test_uart1_send(void)
{
elab_device_t *uart1 = elab_device_find("uart1");
uint8_t data[] = "Hello, UART1!";   
elab_serial_write(uart1,data,sizeof(data));
}

// 测试用例函数
void test_message_queue(void) {
    // 创建一个消息队列，容量为 5，消息大小为 16 字节
    osMessageQueueId_t mq = osMessageQueueNew(1, 16, NULL);
    if (mq == NULL) {
        elog_info("Failed to create message queue.\n");
        return;
    }

    // 测试数据
    const char *msg1 = "Message1";
    const char *msg2 = "Message2";
    char buffer[16];

    // 测试消息发送
    if (osMessageQueuePut(mq, msg1, 0, 0) == osOK) {
        elog_info("Message1 sent successfully.\n");
    } else {
        elog_info("Failed to send Message1.\n");
    }

    if (osMessageQueuePut(mq, msg2, 0, 0) == osOK) {
        elog_info("Message2 sent successfully.\n");
    } else {
        elog_info("Failed to send Message2.\n");
    }

    // 测试消息接收
    if (osMessageQueueGet(mq, buffer, NULL, 0) == osOK) {
        elog_info("Received: %s\n", buffer);
    } else {
        elog_info("Failed to receive message.\n");
    }

    if (osMessageQueueGet(mq, buffer, NULL, 0) == osOK) {
        elog_info("Received: %s\n", buffer);
    } else {
        elog_info("Failed to receive message.\n");
    }

    // 禁止在裸机中使用阻塞的消息队列操作
    // if (osMessageQueueGet(mq, buffer, NULL, 1000) == osErrorTimeout) {
    //     elog_info("Timeout occurred while waiting for a message.\n");
    // }

    // 删除消息队列
    if (osMessageQueueDelete(mq) == osOK) {
        elog_info("Message queue deleted successfully.\n");
    } else {
        elog_info("Failed to delete message queue.\n");
    }
}


