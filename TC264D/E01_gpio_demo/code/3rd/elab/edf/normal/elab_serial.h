/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_SERIAL_H
#define ELAB_SERIAL_H

/* includes ----------------------------------------------------------------- */
#include "../elab_device.h"
#include "../../os/cmsis_os.h"
#include "../../../../component/ringbuf/ringbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* public macros ------------------------------------------------------------ */
enum elab_serial_data_bits
{
    ELAB_SERIAL_DATA_BITS_5 = 5,
    ELAB_SERIAL_DATA_BITS_6,
    ELAB_SERIAL_DATA_BITS_7,
    ELAB_SERIAL_DATA_BITS_8,
};

enum elab_serail_stop_bits
{
    ELAB_SERIAL_STOP_BITS_1 = 0,
    ELAB_SERIAL_STOP_BITS_2,
};

enum elab_serial_parity
{
    ELAB_SERIAL_PARITY_NONE = 0,
    ELAB_SERIAL_PARITY_ODD,
    ELAB_SERIAL_PARITY_EVEN,
};

enum elab_serail_mode
{
    ELAB_SERIAL_MODE_FULL_DUPLEX = 0,
    ELAB_SERIAL_MODE_HALF_DUPLEX,
};

/* Default config for elab_serial_config_t */
#define ELAB_SERIAL_ATTR_DEFAULT                                               \
{                                                                              \
    115200,                                         /* 115200 bits/s */        \
    ELAB_SERIAL_DATA_BITS_8,                        /* 8 databits */           \
    ELAB_SERIAL_STOP_BITS_1,                        /* 1 stopbit */            \
    ELAB_SERIAL_PARITY_NONE,                        /* No parity  */           \
    ELAB_SERIAL_MODE_FULL_DUPLEX,                   /* Full / half duplex */   \
    0,                                                                         \
    256,                                            /* rx buffer size */       \
}

/* public types ------------------------------------------------------------- */
typedef struct elab_serial_attr
{
    uint32_t baud_rate;
    uint32_t data_bits                  : 4;
    uint32_t stop_bits                  : 2;
    uint32_t parity                     : 2;
    uint32_t mode                       : 1;
    uint32_t reserved                   : 7;
    uint32_t rx_bufsz                   : 16;
} elab_serial_attr_t;

typedef struct elab_serial_config
{
    uint32_t baud_rate;
    uint32_t data_bits                  : 4;
    uint32_t stop_bits                  : 2;
    uint32_t parity                     : 2;
    uint32_t mode                       : 1;
    uint32_t reserved                   : 7;
} elab_serial_config_t;


#if ELAB_RTOS_CMSIS_OS_EN != 1
//缓冲区回调
typedef void (* serial_handle_ring_buffer)(ringbuf_t *ringbuf);
#endif

typedef struct elab_serail
{
    elab_device_t super;

#if defined(__linux__) || defined(_WIN32)
    osThreadId_t thread_rx;
#endif
    osMutexId_t mutex_tx;
    osSemaphoreId_t sem_tx;
    osMessageQueueId_t queue_rx;

//裸机推荐使用ringbuffer，操作系统中可以删除
#if ELAB_RTOS_CMSIS_OS_EN != 1
    osTimerId_t timer; //定时处理循环缓冲区定时器
    ringbuf_t rx_ringbuf; //接收缓冲区
    serial_handle_ring_buffer cb;//缓冲区回调函数
#endif

    const struct elab_serial_ops *ops;
    elab_serial_attr_t attr;
} elab_serial_t;

typedef struct elab_serial_ops
{
    elab_err_t (* enable)(elab_serial_t *serial, bool status);
    int32_t (* write)(elab_serial_t *serial, const void *buffer, uint32_t size);
    int32_t (* read)(elab_serial_t *serial, void *buffer, uint32_t size);
    void (* set_tx)(elab_serial_t *serial, bool status);
    elab_err_t (* config)(elab_serial_t *serial, elab_serial_config_t *config);
} elab_serial_ops_t;

#define ELAB_SERIAL_CAST(_dev)          ((elab_serial_t *)_dev)

/* public functions --------------------------------------------------------- */
void elab_serial_register(elab_serial_t *serial, const char *name,
                            const elab_serial_ops_t *ops,
                            elab_serial_attr_t *attr,
                            void *user_data);
void elab_serial_unregister(elab_serial_t *serial);
void elab_serial_tx_end(elab_serial_t *serial);

#if !defined(__linux__) && !defined(_WIN32)
void elab_serial_isr_rx(elab_serial_t *serial, void *buffer, uint32_t size);
#endif

/* For high level program. */
int32_t elab_serial_write(elab_device_t * const me, void *buff, uint32_t size);
int32_t elab_serial_read(elab_device_t * const me, void *buff,
                            uint32_t size, uint32_t timeout);
void elab_serial_set_baudrate(elab_device_t * const me, uint32_t baudrate);
void elab_serial_set_attr(elab_device_t * const me, elab_serial_attr_t *attr);
elab_serial_attr_t elab_serial_get_attr(elab_device_t * const me);

//ringbuffer handler_register
void elab_serial_rx_ringbuf_handler_register(elab_device_t *me, serial_handle_ring_buffer callback);

/* TODO Just for half duplex mode. */
int32_t elab_serial_xfer(elab_device_t *me,
                            void *buff_tx, uint32_t size_tx,
                            void *buff_rx, uint32_t size_rx,
                            uint32_t timeout);



#ifdef __cplusplus
}
#endif

#endif /* ELAB_SERIAL_H */

/* ----------------------------- end of file -------------------------------- */
