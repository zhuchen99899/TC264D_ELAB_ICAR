#include "ringbuf_handle.h"
#include "../3rd/elab/common/elab_assert.h"
#include "elab_export.h"

#include "../3rd/elab/edf/normal/elab_serial.h"
#include "../component/state_machine/state_machine.h"
#ifdef __cplusplus
extern "C" {
#endif

ELAB_TAG("ringbuf_handle");


void init_fsm(void)
{

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



