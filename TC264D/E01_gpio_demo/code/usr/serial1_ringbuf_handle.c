#include "serial1_ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");



typedef struct serial_hsm_s
{
sm_t supper;
ringbuf_t *ringbuffer; //环形缓冲区
uint8_t frame[256]; //数据帧
uint16_t len; //剩余长度字节
uint8_t checksum; //校验和字节
}serial_hsm_t;

#define CHAR_SIG 1
sm_t hsm;
static sm_ret_t init(sm_t *me,sm_event_t const *e);
static sm_ret_t top(sm_t *me,sm_event_t const *e);

static sm_ret_t s0(sm_t *me,sm_event_t const *e); //s0
static sm_ret_t s1(sm_t *me,sm_event_t const *e);//s1
static sm_ret_t s2(sm_t *me,sm_event_t const *e);//s2
static sm_ret_t s3(sm_t *me,sm_event_t const *e);//s3

static sm_ret_t init(sm_t *me,sm_event_t const *e)
{
    elog_error("init");
    (void) me; (void) *e;
    return SM_TRAN(me, top);
}
static sm_ret_t top(sm_t *me,sm_event_t const *e)
{
switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("top entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("top exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("top init\n");
    return SM_TRAN(me, s0);
    /* code */
}
return SM_SUPER(me, hsm_top);
}

static sm_ret_t s0(sm_t *me,sm_event_t const *e)
{

switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("s0 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s0 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("s0 init\n");
    return SM_TRAN(me, s2);
    /* code */
case CHAR_SIG:
    printf("s0 char\n");
    return SM_TRAN(me, s1);
    /* code */
}
return SM_SUPER(me, hsm_top);
}

static sm_ret_t s1(sm_t *me,sm_event_t const *e)
{

switch (e->sig)
{
case SM_ENTRY_SIG:
    printf("s1 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s1 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_INIT_SIG:
    printf("s1 init\n");
    return SM_TRAN(me, s3);
    /* code */
case CHAR_SIG:
    printf("s1 char\n");
    return SM_TRAN(me, s0);
    /* code */
}
return SM_SUPER(me, hsm_top);
}


static sm_ret_t s2(sm_t *me,sm_event_t const *e)
{

switch (e->sig) {
    case SM_ENTRY_SIG:
    printf("s2 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s2 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
}
return SM_SUPER(me, s0);

}

static sm_ret_t s3(sm_t *me,sm_event_t const*e)
{
switch (e->sig) {
    case SM_ENTRY_SIG:
    printf("s3 entry\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
case SM_EXIT_SIG:
    printf("s3 exit\n");
    return SM_HANDLED();  // 使用宏标记事件已处理
}
return SM_SUPER(me, s1);
}



//状态机初始化
// void serial_sm_ctor(serial_hsm_t *me,sm_state_handler_t init)
// {
// hsm_ctor(&me->supper, init);
// me->ringbuffer = NULL;
// memset(me->frame, 0, sizeof(me->frame));
// me->len = 0;
// me->checksum = 0;
// }



    // ringbuf_t ringbuffer;
    //     // 模拟环形缓冲区中的数据流
    // uint8_t *ring_pool[256];
    // uint8_t data_stream[] = {
    //     0xE0, 0xB7,       // 头部
    //     0x00, 0x06,       // 长度
    //     0x01, 0x02, 0x03, // 数据负载
    //     0x06,             // 校验和
    //     0x00, 0xAA        // 尾部
    // };
    // serial_hsm_t serial_hsm;
void init_fsm(void)
{

    hsm_ctor(&hsm, init);
    hsm_init(&hsm, NULL);
    elog_debug("hsm_init_finish");
    // // 模拟事件驱动
    sm_event_t event = { .sig = CHAR_SIG,0 };

    hsm_dispatch(&hsm, &event);

    // sm_event_t event = { .sig = SM_USER_SIG,0 };


    // ringbuffer_init(&ringbuffer, ring_pool, sizeof(ring_pool));
    // ringbuffer_put(&ringbuffer, data_stream, sizeof(data_stream));
    // serial_sm_ctor(&serial_hsm, hsm_top_state);

    // serial_hsm.ringbuffer=&ringbuffer;
    // // 初始化状态
    // hsm_init(&serial_hsm.supper, &init_event);



    // // 模拟事件驱动

    // elog_info("dispatch");
    // //while (!ringbuffer_is_empty(&ringbuffer)) {
    // hsm_dispatch(&serial_hsm.supper, &event);
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



