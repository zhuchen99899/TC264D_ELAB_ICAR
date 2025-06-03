#include "serial1_ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");

#define TEMPERATURE_CHANGED 1

// 状态处理函数声明
sm_ret_t cooling(sm_t *me, const sm_event_t *e);
sm_ret_t heating(sm_t *me, const sm_event_t *e);
sm_ret_t idle(sm_t *me, const sm_event_t *e);

// 状态机对象
static sm_t temperature_fsm;

// 冷却状态处理函数
sm_ret_t cooling(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入冷却状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出冷却状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp <= 25) {
                elog_info("冷却 -> 空闲，当前温度: %d\n", temp);
                return SM_TRAN(me, idle); // 切换到空闲状态
            }
            elog_info("冷却中，当前温度: %d\n", temp);
            return SM_HANDLED();
        }
        default:
            return SM_IGNORE();
    }
}

// 加热状态处理函数
sm_ret_t heating(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入加热状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出加热状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp >= 20) {
                elog_info("加热 -> 空闲，当前温度: %d\n", temp);
                return SM_TRAN(me, idle); // 切换到空闲状态
            }
            elog_info("加热中，当前温度: %d\n", temp);
            return SM_HANDLED();
        }
        default:
            return SM_IGNORE();
    }
}

// 空闲状态处理函数
sm_ret_t idle(sm_t *me, const sm_event_t *e) {
    switch (e->sig) {
        case SM_ENTRY_SIG:
            elog_info("进入空闲状态\n");
            return SM_HANDLED();
        case SM_EXIT_SIG:
            elog_info("退出空闲状态\n");
            return SM_HANDLED();
        case TEMPERATURE_CHANGED: {
            int temp = *(int *)e->event; // 从事件中获取温度数据
            if (temp > 30) {
                elog_info("空闲 -> 冷却，当前温度: %d\n", temp);
                return SM_TRAN(me, cooling); // 切换到冷却状态
            } else if (temp < 15) {
                elog_info("空闲 -> 加热，当前温度: %d\n", temp);
                return SM_TRAN(me, heating); // 切换到加热状态
            }
            elog_info("空闲中，当前温度: %d\n", temp);
            return SM_TRAN(me, idle); // 切换到空闲
        }
        default:
            return SM_IGNORE();
    }
}

sm_ret_t init(sm_t *me, const sm_event_t *e) {
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        elog_info("进入初始状态\n");
        return SM_HANDLED();
    case SM_EXIT_SIG:
        elog_info("退出空闲状态\n");
        return SM_HANDLED();
        case TEMPERATURE_CHANGED: 
            return SM_TRAN(me, idle); // 切换到空闲
    default:
        return SM_IGNORE();
    }
    
}



void init_fsm(void)
{
     static int temperature = 25;
    sm_event_t temp_event = { TEMPERATURE_CHANGED, &temperature ,NULL}; // 定义温度事件

    // 初始化状态机，设置初始状态为空闲
    fsm_ctor(&temperature_fsm, init);

    // 初始化状态机
    fsm_init(&temperature_fsm, &temp_event);

    int temperatures[] = { 35, 28, 22, 18, 12, 20, 25 };
    for (int i = 0; i < 7; i++) {
        temperature = temperatures[i];
        elog_info("温度变化: %d\n", temperature);
        fsm_dispatch(&temperature_fsm, &temp_event);
    }
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



