#include "bsp_pit.h"
#include "isr_config.h"
#include "zf_common_headfile.h"



void bsp_pit_init(void)
{
    pit_ms_init(CCU60_CH0,1);
}

static volatile uint32_t systick_ms;
uint32_t get_TC264D_ms(void)
{

return systick_ms;
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);
    pit_clear_flag(CCU60_CH0);
    systick_ms++;
}








