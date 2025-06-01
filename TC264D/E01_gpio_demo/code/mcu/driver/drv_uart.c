#include "drv_uart.h"
#include "../../3rd/elab/common/elab_assert.h"
#include "../../3rd/elab/common/elab_log.h"
#include "../../3rd/elab/common/elab_def.h"
#include "isr_config.h"
#include "zf_common_headfile.h"


#ifdef __cplusplus
extern "C" {
#endif
ELAB_TAG("driver_uart");

/* private function prototype ----------------------------------------------- */
static elab_err_t _enable(elab_serial_t *me, bool status);
static void _set_tx(elab_serial_t *me, bool status);
static elab_err_t  _config(elab_serial_t *me, elab_serial_config_t *config);
static int  _read(elab_serial_t *me, void *buffer, uint32_t size);
static int  _write(elab_serial_t *me, const void *buffer, uint32_t size);


static const elab_serial_ops_t uart_driver_ops =
{
    .enable = _enable,
    .write = _write,
    .read = _read,
    .set_tx = _set_tx,
    .config = _config,
};

static elab_serial_attr_t attr;

void elab_driver_uart_init(elab_uart_driver_t *me,const char *name,uint32_t boundrate,
                     uart_tx_pin_enum tx,uart_rx_pin_enum rx,uart_index_enum uart_index)
{

    elab_assert(me != NULL);
    elab_assert(name != NULL);
    elab_assert(boundrate != 0);

    me->tx_pin = tx;
    me->rx_pin = rx;
    me->uart_index = uart_index;
    me->baudrate =boundrate;


    attr.baud_rate = boundrate;
    attr.rx_bufsz = 256; //接受缓冲区大小，在操作系统推荐使用消息队列，裸机建议使用ringbuffer
    attr.mode = ELAB_SERIAL_MODE_FULL_DUPLEX; //全双工串口
    attr.parity=ELAB_SERIAL_PARITY_NONE;
    attr.data_bits = ELAB_SERIAL_DATA_BITS_8; //数据位
    attr.stop_bits = ELAB_SERIAL_STOP_BITS_1; //停止位
    elog_info("uart_index:%d,baudrate:%d,tx_pin:%d,rx_pin:%d\r\n",
    me->uart_index,me->baudrate,me->tx_pin,me->rx_pin);
    uart_init(me->uart_index,me->baudrate,me->tx_pin,me->rx_pin);

    uart_rx_interrupt(me->uart_index,1); //开启接收中断
    // uart_tx_interrupt(me->uart_index,1); //开启发送中断

    elab_serial_register(&me->device,name,&uart_driver_ops,
    &attr,me);

}

/**
 * @brief enable the uart driver
 * 
 * @param me 
 * @param status 
 * @note  为设备层提供接口，主要为在linux环境上使用。tc264D仅需提供函数接口
 */
elab_err_t _enable(elab_serial_t *me,bool status)
{
assert(me != NULL);
    (void)me;
    (void)status;
    return ELAB_OK;
}

/**
 * @brief half duplex uart to set sendmode
 * 
 * @param me 
 * @param status 
 * @note 半双工串口发送模式接口，全双工串口仅需实现函数接口
 */
void _set_tx(elab_serial_t *me,bool status)
{
    assert(me != NULL);
    (void)me;
    (void)status;
}

static elab_err_t  _config(elab_serial_t *me, elab_serial_config_t *config)
{
    assert(me != NULL);
    assert(config != NULL);
    assert(me->super.user_data != NULL);

elab_uart_driver_t *driver=(elab_uart_driver_t *)me->super.user_data;

driver->baudrate = config->baud_rate;

//TC264 ASCALLIN 不支持重设
// uart_init(driver->uart_index,driver->baudrate,driver->tx_pin,driver->rx_pin);
    return ELAB_OK; 
}

/**
 * @brief 接受串口数据
 * 
 * @param me 
 * @param buffer 
 * @param size 
 * @return elab_err_t 
 */
static int  _read(elab_serial_t *me, void *buffer, uint32_t size){
    assert(me != NULL);
    assert(buffer != NULL);
    assert(size != 0);

    elab_uart_driver_t *driver=(elab_uart_driver_t *)me->super.user_data;

uint8_t byte_data;
uint8_t *byte_buffer = (uint8_t *)buffer;
for(uint32_t i=0;i<size;i++){
    uart_query_byte(driver->uart_index,&byte_data);
    byte_buffer[i]=byte_data;

}
    return 0; //返回成功

}

/**
 * @brief 发送串口数据
 * 
 * @param me 
 * @param buffer 
 * @param size 
 * @return elab_err_t 
 */
static int  _write(elab_serial_t *me, const void *buffer, uint32_t size)
{
    assert(me != NULL);
    assert(buffer != NULL);
    assert(size != 0);

    elab_uart_driver_t *driver=(elab_uart_driver_t *)me->super.user_data;

    uart_write_buffer(driver->uart_index,buffer,size);

    return 0; //返回成功

}

void uart_rx_interrupt_handler (void)
{
//    get_data = uart_read_byte(UART_INDEX);                                      // 接收数据 while 等待式 不建议在中断使用
                      // 将数据写入 fifo 中
}


// IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
// {
//     interrupt_global_enable(0);                     // 开启中断嵌套

// }

IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套

    uart_rx_interrupt_handler();                    // 串口接收处理
}



#ifdef __cplusplus
}
#endif


