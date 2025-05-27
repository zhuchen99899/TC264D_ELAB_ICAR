#include "drv_pin.h"
#include "../../3rd/elab/common/elab_assert.h"
#include "../../3rd/elab/common/elab_log.h"
#include "../../3rd/elab/common/elab_def.h"

#ifdef __cplusplus
extern "C" {
#endif
ELAB_TAG("driver_pin");
/* private function prototype ----------------------------------------------- */
static elab_err_t _init(elab_pin_t * const me);
static elab_err_t _set_mode(elab_pin_t * const me, uint8_t mode);
static elab_err_t _get_status(elab_pin_t * const me, bool *status);
static elab_err_t _set_status(elab_pin_t * const me, bool status);

/* private variables -------------------------------------------------------- */
static const elab_pin_ops_t pin_driver_ops =
{
    .init = _init,
    .set_mode = _set_mode,
    .get_status = _get_status,
    .set_status = _set_status,
};

/* public functions --------------------------------------------------------- */
void elab_driver_pin_init(elab_pin_driver_t *me,const char *name,gpio_pin_enum pin)
{
    elab_assert(me != NULL);
    elab_assert(name != NULL);

    me->pin_port= pin;
    elab_pin_register(&me->device, name, &pin_driver_ops, me);
}


/* private functions -------------------------------------------------------- */
/**
  * @brief  The PIN driver initialization function.
  * @param  me  PIN device handle.
  * @retval None.
  */
static  elab_err_t _init(elab_pin_t * const me)
{
    elab_assert(me!= NULL);
    elab_pin_driver_t *driver = (elab_pin_driver_t *)me->super.user_data;
    gpio_init(driver->pin_port, GPO, GPIO_LOW, GPO_PUSH_PULL); 
    return ELAB_OK;

}

/**
  * @brief  The PIN driver set_mode function.
  * @param  me  PIN device handle.
  * @retval None.
  */
static elab_err_t _set_mode(elab_pin_t * const me, uint8_t mode)
{
    elab_assert(me!= NULL);
    elab_pin_driver_t *driver = (elab_pin_driver_t *)me->super.user_data;
       if (mode == PIN_MODE_INPUT)
    {
gpio_init(driver->pin_port, GPI, GPIO_LOW, GPI_FLOATING_IN); 
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
gpio_init(driver->pin_port, GPI, GPIO_LOW, GPI_PULL_UP); 
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
gpio_init(driver->pin_port, GPI, GPIO_LOW, GPI_PULL_DOWN); 
    }
    else if (mode == PIN_MODE_OUTPUT_PP)
    {
gpio_init(driver->pin_port, GPO, GPIO_LOW, GPO_PUSH_PULL); 
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
gpio_init(driver->pin_port, GPO, GPIO_LOW, GPO_OPEN_DTAIN); 
    }
    return ELAB_OK;
}


/**
  * @brief  The PIN driver set_mode function.
  * @param  me  PIN device handle.
  * @retval GPIO status.
  */
static elab_err_t _get_status(elab_pin_t * const me, bool *status)
{
    elab_assert(me != NULL);
    
    elab_pin_driver_t *driver = (elab_pin_driver_t *)me->super.user_data;


    uint8_t gpip_status = gpio_get_level(driver->pin_port);
    *status = (gpip_status == 1) ? true : false;
    
    return ELAB_OK;
}

/**
  * @brief  The PIN driver set_status function.
  * @param  me      PIN device handle.
  * @param  status  GPIO status.
  * @retval None.
  */
static elab_err_t _set_status(elab_pin_t * const me, bool status)
{
    elab_assert(me != NULL);
    
    elab_pin_driver_t *driver = (elab_pin_driver_t *)me->super.user_data;
    gpio_set_level(driver->pin_port, status? 1 : 0);
    
    return ELAB_OK;
}

#ifdef __cplusplus
}
#endif


