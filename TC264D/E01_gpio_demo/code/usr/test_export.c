#include "elab_export.h"
#include "elab_log.h"
#include "elab_device.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST,EXPORT_DRVIVER);



static void _timer_func(void *argument){

         elog_debug("test_timer");

}



static const osTimerAttr_t test_timer_attr_led =
{
    .name = "test_timer",
    .attr_bits = 0,
    .cb_mem = NULL,
    .cb_size = 0,
};
static osTimerId_t test_timer;
void init_test_timer(void)
{
    test_timer = osTimerNew(_timer_func, osTimerPeriodic, NULL, &test_timer_attr_led);

    osTimerStart(test_timer, 1);
}
INIT_EXPORT(init_test_timer,EXPORT_MIDWARE);


