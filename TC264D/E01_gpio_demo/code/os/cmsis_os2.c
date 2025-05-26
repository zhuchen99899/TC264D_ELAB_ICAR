#include "cmsis_os2.h"
#include "../3rd/elab/common/elab_config.h"
#include "../3rd/elab/common/elab_export.h"
#include "../3rd/elab/common/elab_common.h"


#if ELAB_RTOS_CMSIS_OS_EN != 1
#define MAX_ELAB_NOOS_CMSIS_TIMERS 16 //总计16个定时
#endif

typedef struct {
    osTimerFunc_t callback;
    void *argument;
    uint32_t period_ms;
    bool is_periodic;
    bool is_running;
    uint32_t timeout_ms;
} elab_cmsis_timer_t;



static elab_cmsis_timer_t *g_cmsis_timers[MAX_ELAB_NOOS_CMSIS_TIMERS] = {0};

/* Kernel functions */
osStatus_t osKernelInitialize(void) { return 0; }
osStatus_t osKernelStart(void)      { return 0; }



/* Mutex functions */
osMutexId_t osMutexNew(const osMutexAttr_t *attr) {
    (void)attr;
    return (osMutexId_t)1;  // 杩斿洖闈炵┖鎸囬拡琛ㄧず鍒涘缓鎴愬姛
}

osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout) {
    (void)mutex_id;
    (void)timeout;
    return osOK;
}

osStatus_t osMutexRelease(osMutexId_t mutex_id) {
    (void)mutex_id;
    return osOK;
}

osStatus_t osMutexDelete(osMutexId_t mutex_id) {
    (void)mutex_id;
    return osOK;
}

/* Stub implementations for threads (not used in bare metal) */
osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr) {
    (void)func; (void)argument; (void)attr;
    return NULL;
}


osThreadId_t osThreadGetId (void)
{
    (void)0;
    return NULL;
}

uint32_t osKernelGetTickCount (void)
{
return elab_time_ms();

}


//no_os_timer

static void elab_cmsis_timer_poll(void)
{
    uint32_t now = elab_time_ms();

    for (int i = 0; i < MAX_ELAB_NOOS_CMSIS_TIMERS; i++) {
        elab_cmsis_timer_t *timer = g_cmsis_timers[i];
        if (timer && timer->is_running) {
            if ((int32_t)(now - timer->timeout_ms) >= 0) {
                timer->callback(timer->argument);
                if (timer->is_periodic) {
                    timer->timeout_ms += timer->period_ms;
                } else {
                    timer->is_running = false;
                }
            }
        }
    }
}

POLL_EXPORT(elab_cmsis_timer_poll, 1); //elab定时器轮询

osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    for (int i = 0; i < MAX_ELAB_NOOS_CMSIS_TIMERS; i++) {
        if (g_cmsis_timers[i] == NULL) {
            elab_cmsis_timer_t *timer = malloc(sizeof(elab_cmsis_timer_t));
            if (!timer) return NULL;

            timer->callback = func;
            timer->argument = argument;
            timer->is_periodic = (type == osTimerPeriodic);
            timer->is_running = false;
            timer->period_ms = 0;
            timer->timeout_ms = 0;

            g_cmsis_timers[i] = timer;
            return (osTimerId_t)timer;
        }
    }
    return NULL;
}

osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
    if (!timer_id || ticks == 0) return osErrorParameter;
    elab_cmsis_timer_t *timer = (elab_cmsis_timer_t *)timer_id;

    timer->period_ms = ticks;
    timer->timeout_ms = elab_time_ms() + ticks;
    timer->is_running = true;

    return osOK;
}

osStatus_t osTimerStop(osTimerId_t timer_id)
{
    if (!timer_id) return osErrorParameter;
    elab_cmsis_timer_t *timer = (elab_cmsis_timer_t *)timer_id;

    timer->is_running = false;
    return osOK;
}


