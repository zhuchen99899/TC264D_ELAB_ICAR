#include "serial1_ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");



/**
 *  ├── HomeScreen
    └── Menu
      ├── DisplaySettings
      │     ├── Brightness
      │     └── Contrast
      └── NetworkSettings
            ├── WiFi
            └── Ethernet
 * 
 */

sm_t hsm;
enum
{
    SIG_MENU=0, //切到菜单
    SIG_ENTER_DISPLAY, 
    SIG_ENTER_BRIGHTNESS,
    SIG_ENTER_WIFI,
    SIG_DOWN, //同级别 向下菜单切换
    SIG_UP,  //同级别 向上菜单切换
    SIG_ESC
};

static sm_ret_t init(sm_t *me, sm_event_t const *e);
static sm_ret_t begin_state(sm_t *me, sm_event_t const *e);
static sm_ret_t home_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Menu_state(sm_t *me, sm_event_t const *e);
static sm_ret_t DisplaySettings_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Brightness_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Contrast_state(sm_t *me, sm_event_t const *e);
static sm_ret_t NetworkSettings_state(sm_t *me, sm_event_t const *e);
static sm_ret_t WIFI_state(sm_t *me, sm_event_t const *e);
static sm_ret_t Ethernet_state(sm_t *me, sm_event_t const *e);


static sm_ret_t init(sm_t *me, sm_event_t const *e)
{
    (void)me;(void)e;
    return SM_TRAN(me, begin_state);
}
static sm_ret_t begin_state(sm_t *me, sm_event_t const *e)
{
switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("begine entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("begine  exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_INIT_SIG:
        printf("begine init\r\n");
        return SM_TRAN(me, home_state);
    /* code */
    }
return SM_SUPER(me, hsm_top);

}

static sm_ret_t home_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("home_State entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("home_State exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_MENU:
        printf("go Menu\r\n");
        return SM_TRAN(me, Menu_state);
    /* code */
    }
return SM_SUPER(me, begin_state);

}

//Menu 父状态，统一处理ESC返回
static sm_ret_t Menu_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Menu_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Menu_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    // case SM_INIT_SIG:
    //     printf("Menu_state init\r\n");
    //     return SM_TRAN(me, DisplaySettings_state);
    case SIG_ESC:
        printf("back home\r\n");
        return SM_TRAN(me,begin_state); // 统一 ESC 返回首页
    case SIG_ENTER_DISPLAY:
        printf("go DisplaySettings\r\n");
        return SM_TRAN(me,DisplaySettings_state);
    /* code */
    }
return SM_SUPER(me, home_state);


}


static sm_ret_t DisplaySettings_state(sm_t *me, sm_event_t const *e)
{
    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("DisplaySettings_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("DisplaySettings_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    // case SM_INIT_SIG:
    //     printf("DisplaySettings_state init\r\n");
    //     return SM_TRAN(me, Brightness_state);
    case SIG_DOWN:
        printf("go NetworkSettings\r\n");
        return SM_TRAN(me, NetworkSettings_state);
    case SIG_ENTER_BRIGHTNESS:
        printf("go Brightness\r\n");
        return SM_TRAN(me, Brightness_state);
    /* code */
    }
return SM_SUPER(me, Menu_state);


}

static sm_ret_t NetworkSettings_state(sm_t *me, sm_event_t const *e)
{


    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("NetworkSettings_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("NetworkSettings_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    // case SM_INIT_SIG:
    //     printf("NetworkSettings_state init\r\n");
    //     return SM_TRAN(me, WIFI_state);
    case SIG_UP:
        printf("go DisplaySettings\r\n");
        return SM_TRAN(me, DisplaySettings_state);
    case SIG_ENTER_WIFI:
        printf("go WIFI\r\n");
        return SM_TRAN(me, WIFI_state);
        
    /* code */
    }
return SM_SUPER(me, Menu_state);

}

static sm_ret_t Brightness_state(sm_t *me, sm_event_t const *e)
{


    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Brightness_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Brightness_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_DOWN:
        printf("go Contrast_state\r\n");
        return SM_TRAN(me, Contrast_state);
        
    /* code */
    }
return SM_SUPER(me, DisplaySettings_state);
}

static sm_ret_t Contrast_state(sm_t *me, sm_event_t const *e)
{

    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Contrast_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Contrast_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_UP:
        printf("go Brightness_state\r\n");
        return SM_TRAN(me, Brightness_state);
        
    /* code */
    }
return SM_SUPER(me, DisplaySettings_state);
}

static sm_ret_t WIFI_state(sm_t *me, sm_event_t const *e)
{

    switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("WIFI_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("WIFI_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_DOWN:
        printf("go Ethernet_state\r\n");
        return SM_TRAN(me, Ethernet_state);
        
    /* code */
    }
return SM_SUPER(me, NetworkSettings_state);
}

static sm_ret_t Ethernet_state(sm_t *me, sm_event_t const *e)
{

        switch (e->sig)
    {
    case SM_ENTRY_SIG:
        printf("Ethernet_state entry\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SM_EXIT_SIG:
        printf("Ethernet_state exit\r\n");
        return SM_HANDLED();  // 使用宏标记事件已处理
    case SIG_UP:
        printf("go WIFI_state\r\n");
        return SM_TRAN(me, WIFI_state);
    /* code */
    }
return SM_SUPER(me, NetworkSettings_state);
}

void init_fsm(void)
{
    hsm_ctor(&hsm,init);
    hsm_init(&hsm,NULL);


    sm_event_t event = { .sig = SIG_MENU,0 };

    printf("sig_menu\r\n");
    hsm_dispatch(&hsm, &event);//进入菜单

    event.sig = SIG_ENTER_DISPLAY;//显示设置
    printf("sig_enter\r\n");
    hsm_dispatch(&hsm, &event);

    
    event.sig = SIG_DOWN;//显示设置->网络设置
    printf("sig_down\r\n");
    hsm_dispatch(&hsm, &event);


    event.sig = SIG_ENTER_WIFI;//wifi
    printf("sig_enter\r\n");
    hsm_dispatch(&hsm, &event);


    event.sig = SIG_DOWN;// wifi->ethernet
    printf("sig_down\r\n");
    hsm_dispatch(&hsm, &event);//wifi->ethernet

    event.sig = SIG_ESC;//回到主页
    printf("sig_esc\r\n");
    hsm_dispatch(&hsm, &event);//wifi->ethernet
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



