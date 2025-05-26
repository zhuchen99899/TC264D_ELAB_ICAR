#include <os/cmsis_os2.h>

/* Kernel functions */
osStatus_t osKernelInitialize(void) { return 0; }
osStatus_t osKernelStart(void)      { return 0; }

/* Mutex functions */
osMutexId_t osMutexNew(const osMutexAttr_t *attr) {
    (void)attr;
    return (osMutexId_t)1;  // 返回非空指针表示创建成功
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
