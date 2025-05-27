#include "../driver/drv_pin.h"
#include "../../3rd/elab/common/elab_export.h"


static elab_pin_driver_t pin_led1;


static void driver_pin_mcu_export(void)
{
    elab_driver_pin_init(&pin_led1, "led1_pin", P21_4);
    elab_pin_set_mode(&pin_led1.device.super, PIN_MODE_OUTPUT_PP);
}

//INIT_EXPORT(driver_pin_mcu_export, EXPORT_DRVIVER);


