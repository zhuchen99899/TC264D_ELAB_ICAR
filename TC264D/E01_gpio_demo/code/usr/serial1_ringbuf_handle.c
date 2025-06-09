#include "serial1_ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");


#define SIG_PARSE 1


typedef struct com_fsm_s
{
sm_t supper;
ringbuf_t *ringbuffer;
uint16_t remainingLen;
uint8_t *peekPointer;
}com_fsm_t;

void serial_fsm_ctor(sm_t *me, sm_state_handler_t init)
{
com_fsm_t *fsm=(com_fsm_t *)me;
fsm->ringbuffer=NULL;
fsm->remainingLen=0;
fsm->peekPointer=NULL;
fsm_ctor(&fsm->supper,init);
}


static sm_ret_t init(sm_t *me,sm_event_t const *e);
static sm_ret_t parse_header(sm_t *me,sm_event_t const *e); 
static sm_ret_t parse_remainingLen(sm_t *me,sm_event_t const *e);
static sm_ret_t parse_sum(sm_t *me,sm_event_t const *e);
static sm_ret_t handleData(sm_t *me,sm_event_t const *e);


static sm_ret_t init(sm_t *me,sm_event_t const *e)
{
(void)me;(void)e;

return SM_TRAN(me,parse_header);

}

static sm_ret_t parse_header(sm_t *me,sm_event_t const *e)
{
    com_fsm_t * fsm= (com_fsm_t *)me;
    uint16_t len;
    switch (e->sig)
    {
    case SIG_PARSE:

        len=ringbuffer_peek(fsm->ringbuffer, &fsm->peekPointer);
        if(len>0) 
        {
        elog_info("header0=%x",fsm->peekPointer[0]);
        elog_info("header1=%x",fsm->peekPointer[1]);
            if(fsm->peekPointer[0]==0x55&&fsm->peekPointer[1]==0xaa)
            {
            return SM_TRAN(&fsm->supper,parse_remainingLen);

            }
            else{
                return SM_TRAN(&fsm->supper,init);
            }

        }
        else{
            elog_info("ringbuffer empty");
        }
    }

    return SM_IGNORE();   // 使用宏标记事件忽略
}



static sm_ret_t parse_remainingLen(sm_t *me,sm_event_t const *e)
{
    com_fsm_t * fsm= (com_fsm_t *)me;
    switch (e->sig)
    {
    case SIG_PARSE:
        elog_info("remainingLen=%x",fsm->peekPointer[2]<<8|fsm->peekPointer[3]);
        fsm->remainingLen=fsm->peekPointer[2]<<8|fsm->peekPointer[3];
        if(fsm->remainingLen>0)
        return SM_TRAN(&fsm->supper,parse_sum);
        
    }
return SM_IGNORE();   // 使用宏标记事件忽略
}

uint8_t data[256];

static sm_ret_t parse_sum(sm_t *me,sm_event_t const *e)
{
    com_fsm_t *fsm =(com_fsm_t* )me;
    uint8_t sum=0;
    switch (e->sig)
    {
    case SIG_PARSE:
        for(int i=0;i<fsm->remainingLen+3;i++)
        {
            sum+=fsm->peekPointer[i];
        }
        elog_info("caculate_sum=%x",sum);
        elog_info("sum=%x",fsm->peekPointer[fsm->remainingLen+3]); 
        if(sum==fsm->peekPointer[fsm->remainingLen+3])
        return SM_TRAN(&fsm->supper,handleData);
        else 
        {
        ringbuffer_get(fsm->ringbuffer,data,fsm->remainingLen+4); //从缓冲区取出废弃数据
        return SM_TRAN(&fsm->supper,init);
        }

    }
return SM_IGNORE();   // 使用宏标记事件忽略
}

static sm_ret_t handleData(sm_t *me,sm_event_t const *e)
{
    com_fsm_t *fsm=(com_fsm_t *)me;
    uint16_t len;
    switch (e->sig)
    {
    case SIG_PARSE:
    len=ringbuffer_get(fsm->ringbuffer,data,fsm->remainingLen+2);
    if(data[2]==0xFA) elog_info("get cmd and handledata:%x",data[3]);
    }
    return SM_TRAN(&fsm->supper,init);
}


static com_fsm_t serial_fsm;

ringbuf_t ringbuffer;
uint8_t ringpool[256];

uint8_t data_stream[]={
0x55,0xAA, //固定头部
0x00,0x06, //大端模式
0XFA,0XFF,0XFF,0XFF,0XFF, //数据负载
0xFA //校验和
};
void init_fsm(void)
{

    ringbuffer_init(&ringbuffer, ringpool, sizeof(ringpool));
    ringbuffer_put(&ringbuffer, data_stream, sizeof(data_stream));
    
    serial_fsm_ctor(&serial_fsm.supper,init);
    serial_fsm.ringbuffer=&ringbuffer;
    fsm_init(&serial_fsm.supper,NULL);

    sm_event_t event = { .sig = SIG_PARSE,0 };

    // // 模拟事件驱动

    elog_info("dispatch");
    //while (!ringbuffer_is_empty(&ringbuffer)) {
    fsm_dispatch(&serial_fsm.supper, &event);
    fsm_dispatch(&serial_fsm.supper, &event);
    fsm_dispatch(&serial_fsm.supper, &event);
    fsm_dispatch(&serial_fsm.supper, &event);
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



