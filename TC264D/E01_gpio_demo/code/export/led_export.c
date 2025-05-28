#include "./3rd/elab/edf/user/elab_led.h"
#include "./3rd/elab/common/elab_export.h"
#include "./3rd/elab/common/elab_assert.h"

ELAB_TAG("EXPORT_LED");
static elab_led_t led1;


static void led_export(void)
{

    elab_led_register(&led1, "led1", "led1_pin",true);
    elab_assert(&led1 != NULL);
	 //elab_led_toggle(&led1.super,500);
    elab_led_set_value(&led1.super,2);
}

INIT_EXPORT(led_export, EXPORT_DEVICE);
