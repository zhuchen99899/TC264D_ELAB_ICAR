#include "serial1_ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");

#define SIG_PEEK_HEAD 1


typedef struct serial_hsm_s
{
sm_t supper;
uint8_t *ringbuffer; //环形缓冲区
uint8_t frame[256]; //数据帧
uint16_t len; //剩余长度字节
uint8_t checksum; //校验和字节
}serial_hsm_t;

// 状态函数声明
static sm_ret_t hsm_top_state(sm_t *me, const sm_event_t *e);
static sm_ret_t hsm_peek_header_state(sm_t *me, const sm_event_t *e);
static sm_ret_t hsm_peek_length_state(sm_t *me, const sm_event_t *e);
static sm_ret_t hsm_peek_tail_state(sm_t *me, const sm_event_t *e);
static sm_ret_t hsm_get_data_state(sm_t *me, const sm_event_t *e);
static sm_ret_t hsm_process_payload_state(sm_t *me, const sm_event_t *e);


//状态机初始化
void serial_sm_ctor(serial_hsm_t *me,sm_state_handler_t init)
{
hsm_ctor(&me->supper, init);
me->ringbuffer = NULL;
memset(me->frame, 0, sizeof(me->frame));
me->len = 0;
me->checksum = 0;
}


// 初始化状态
static sm_ret_t hsm_top_state(sm_t *me, const sm_event_t *e) {

    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("State Machine Initialized\n");
            return SM_HANDLED();
        case SM_USER_SIG:

            return SM_TRAN(me, hsm_peek_header_state);
        case SM_EMPTY_SIG:
            elog_info("empty_sig");
        // 根状态可以忽略 SM_EMPTY_SIG
        return SM_IGNORE();
        default:

            return SM_UNHANDLED();
    }
}

// Peek 头部状态
static sm_ret_t hsm_peek_header_state(sm_t *me, const sm_event_t *e) {
    serial_hsm_t *self = (serial_hsm_t *)me;

    switch (e->sig) {
        case SM_ENTRY_SIG:
            printf("Peeking header...\n");
            return SM_HANDLED();

        case SM_USER_SIG: {
            uint8_t bytes[2];
            ringbuffer_peek(self->ringbuffer, bytes);  // Peek 2 字节头部
                            elog_info("byte[0]=%x",bytes[0]);
                elog_info("byte[1]=%x",bytes[1]);
            if (bytes[0] == 0xE0 && bytes[1] == 0xB7) {

                printf("Header matched: 0xE0 0xB7\n");
                return SM_TRAN(me, hsm_peek_length_state);
            } else {
                printf("Header mismatch\n");
            }
            return SM_HANDLED();
        }
        case SM_EMPTY_SIG://层次状态机必须子状态必须返回父状态
            // 返回父状态 hsm_top_state
            return SM_SUPER(me, hsm_top_state);
        default:
            elog_info("header");
            return SM_UNHANDLED();
    }
}


// Peek 长度状态
static sm_ret_t hsm_peek_length_state(sm_t *me, const sm_event_t *e) {
    serial_hsm_t *self = (serial_hsm_t *)me;

    switch (e->sig) {
        case SM_ENTRY_SIG:
            printf("Peeking length...\n");
            return SM_HANDLED();

        case SM_USER_SIG: {
            uint8_t length_bytes[4];//取出前四个字节
            ringbuffer_peek(self->ringbuffer, length_bytes);  // Peek 2 字节长度
            self->len = (length_bytes[3] << 8) | length_bytes[2];
            printf("Payload length: %d\n", self->len);
            return SM_TRAN(me, hsm_peek_tail_state);
        }
        case SM_EMPTY_SIG:
            // 返回父状态 hsm_peek_header_state
            return SM_SUPER(me, hsm_peek_header_state);
        default:
            return SM_UNHANDLED();
    }
}

// Peek 尾部状态
static sm_ret_t hsm_peek_tail_state(sm_t *me, const sm_event_t *e) {
    serial_hsm_t *self = (serial_hsm_t *)me;

    switch (e->sig) {
        case SM_ENTRY_SIG:
            printf("Peeking tail...\n");
            return SM_HANDLED();

        case SM_USER_SIG: {
            uint8_t tail_bytes[self->len+2];//取出所有字节
            ringbuffer_peek(self->ringbuffer, tail_bytes);  // Peek 2 字节尾部
            if (tail_bytes[self->len] == 0x00 && tail_bytes[self->len+1] == 0xAA) {
                printf("Tail matched: 0x00 0xAA\n");
                return SM_TRAN(me, hsm_get_data_state);
            } else {
                printf("Tail mismatch\n");
            }
            return SM_HANDLED();
        }
        case SM_EMPTY_SIG:
            // 返回父状态 hsm_peek_length_state
            return SM_SUPER(me, hsm_peek_length_state);
        default:
            return SM_UNHANDLED();
    }
}



// Get 数据状态
static sm_ret_t hsm_get_data_state(sm_t *me, const sm_event_t *e) {
    serial_hsm_t *self = (serial_hsm_t *)me;

    switch (e->sig) {
        case SM_ENTRY_SIG:
            printf("Getting data...\n");
            return SM_HANDLED();

        case SM_USER_SIG: {
            uint16_t total_length = 2 + self->len;  // 总长度
            ringbuffer_get(self->ringbuffer, self->frame, total_length);  // 获取完整数据包

            // 校验和计算
            self->checksum = 0;
            for (uint16_t i = 0; i <total_length; i++) {
                self->checksum += self->frame[i];
            }

            // 校验和验证
            if (self->checksum == self->frame[self->len-2]) {
                printf("Checksum verified: 0x%02X\n", self->checksum);
                return SM_TRAN(me, hsm_process_payload_state);
            } else {
                printf("Checksum error! Expected: 0x%02X, Received: 0x%02X\n",
                       self->checksum, self->frame[self->len-2]);
                return SM_TRAN(me, hsm_peek_header_state);  // 重置到头部状态
            }
        }
        case SM_EMPTY_SIG:
            // 返回父状态 hsm_peek_tail_state
            return SM_SUPER(me, hsm_peek_tail_state);
        default:
            return SM_UNHANDLED();
    }
}



// 处理数据负载状态
static sm_ret_t hsm_process_payload_state(sm_t *me, const sm_event_t *e) {
    serial_hsm_t *self = (serial_hsm_t *)me;

    switch (e->sig) {
        case SM_ENTRY_SIG:
            printf("Processing payload...\n");
            // 在这里解析数据负载
            for (uint16_t i = 0; i < self->len-2; i++) {
                printf("Payload[%d]: 0x%02X\n", i, self->frame[4+ i]);
            }
            return SM_TRAN(me, hsm_peek_header_state);  // 重置到头部状态
            case SM_EMPTY_SIG:
            // 返回父状态，例如 hsm_peek_tail_state
            return SM_SUPER(me, hsm_peek_tail_state);
        default:
            return SM_UNHANDLED();
    }
}

void init_fsm(void)
{
    // 模拟事件驱动
    sm_event_t init_event = { .sig = SM_USER_SIG,0 };

    serial_hsm_t serial_hsm;
    serial_sm_ctor(&serial_hsm, hsm_top_state);
    // 初始化状态
    hsm_init(&serial_hsm.supper, &init_event);
    // 模拟环形缓冲区中的数据流
    uint8_t data_stream[] = {
        0xE0, 0xB7,       // 头部
        0x00, 0x06,       // 长度
        0x01, 0x02, 0x03, // 数据负载
        0x06,             // 校验和
        0x00, 0xAA        // 尾部
    };
    ringbuf_t ringbuffer;

    ringbuffer_init(&ringbuffer, data_stream, sizeof(data_stream));
    serial_hsm.ringbuffer=&ringbuffer;
    // 模拟事件驱动
    sm_event_t event = { .sig = SM_USER_SIG,0 };

    //while (!ringbuffer_is_empty(&ringbuffer)) {
    hsm_dispatch(&serial_hsm.supper, &event);
}


INIT_EXPORT(init_fsm,EXPORT_MIDWARE);


void _ringbuf_handler(ringbuf_t *ringbuf)
{
 

    elab_assert(ringbuf != NULL);
    uint8_t data = 0;
    //ringbuffer_getbyte(ringbuf, 1); 
    // fsm_dispatch(&traffic_fsm, &timer_event); // 分发定时器事件

}



#ifdef __cplusplus
}
#endif



