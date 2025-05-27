#ifndef __DRIVER_PIN_H__
#define __DRIVER_PIN_H__

#include "../../3rd/elab/edf/normal/elab_pin.h"
#include "zf_driver_gpio.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef struct elab_pin_driver
{
    elab_pin_t device;
    gpio_pin_enum pin_port; //引脚
} elab_pin_driver_t;

void elab_driver_pin_init(elab_pin_driver_t *me,const char *name,gpio_pin_enum pin);
#ifdef __cplusplus
}
#endif

#endif //__DRIVER_PIN_H__



