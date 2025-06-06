/**
 * \file IfxAsclin.c
 * \brief ASCLIN  basic functionality
 *
 * \version iLLD_1_0_1_11_0
 * \copyright Copyright (c) 2019 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxAsclin.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxAsclin_disableModule(Ifx_ASCLIN *asclin)
{
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw);            /* clears the endinit protection*/
    IfxAsclin_setDisableModuleRequest(asclin); /* disables the module*/
    IfxScuWdt_setCpuEndinit(psw);              /* sets the endinit protection back on*/
}


void IfxAsclin_enableAscErrorFlags(Ifx_ASCLIN *asclin, boolean parEnable, boolean rfoEnable)
{
    IfxAsclin_enableParityErrorFlag(asclin, parEnable);     /* enables parity error*/
    IfxAsclin_enableRxFifoOverflowFlag(asclin, rfoEnable);  /* enables Rx fifo Overflow error*/
}


void IfxAsclin_enableModule(Ifx_ASCLIN *asclin)
{
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw);           /* clears the endinit protection*/
    IfxAsclin_setEnableModuleRequest(asclin); /* enables the module*/
    IfxScuWdt_setCpuEndinit(psw);             /* sets the endinit protection back on*/
}


Ifx_ASCLIN *IfxAsclin_getAddress(IfxAsclin_Index asclin)
{
    Ifx_ASCLIN *module;

    if (asclin < IFXASCLIN_NUM_MODULES)
    {
        module = (Ifx_ASCLIN *)IfxAsclin_cfg_indexMap[asclin].module;
    }
    else
    {
        module = NULL_PTR;
    }

    return module;
}


float32 IfxAsclin_getFaFrequency(Ifx_ASCLIN *asclin)
{
    float32               frequency;
    IfxAsclin_ClockSource clockSource = (IfxAsclin_ClockSource)IfxAsclin_getClockSource(asclin);    /* gets the current clock source*/

    switch (clockSource)
    {
    case IfxAsclin_ClockSource_noClock: /* gets the respective frequency*/
        frequency = 0.0;
        break;
    case IfxAsclin_ClockSource_kernelClock:
        frequency = IfxScuCcu_getSpbFrequency();
        break;
    case IfxAsclin_ClockSource_oscillatorClock:
        frequency = IfxScuCcu_getOsc0Frequency();
        break;
    case IfxAsclin_ClockSource_flexRayClock:
        frequency = IfxScuCcu_getPllErayFrequency();
        break;
    case IfxAsclin_ClockSource_ascFastClock:
        frequency = IfxScuCcu_getBaud2Frequency();
        break;
    case IfxAsclin_ClockSource_ascSlowClock:
        frequency = IfxScuCcu_getBaud1Frequency();
        break;
    default:
        frequency = 0.0;
        break;
    }

    return frequency;
}


IfxAsclin_Index IfxAsclin_getIndex(Ifx_ASCLIN *asclin)
{
    uint32          index;
    IfxAsclin_Index result;

    result = IfxAsclin_Index_none;

    for (index = 0; index < IFXASCLIN_NUM_MODULES; index++)
    {
        if (IfxAsclin_cfg_indexMap[index].module == asclin)
        {
            result = (IfxAsclin_Index)IfxAsclin_cfg_indexMap[index].index;
            break;
        }
    }

    return result;
}


float32 IfxAsclin_getOvsFrequency(Ifx_ASCLIN *asclin)
{
    return (IfxAsclin_getPdFrequency(asclin) * asclin->BRG.B.NUMERATOR) / asclin->BRG.B.DENOMINATOR;
}


float32 IfxAsclin_getPdFrequency(Ifx_ASCLIN *asclin)
{
    return IfxAsclin_getFaFrequency(asclin) / (asclin->BITCON.B.PRESCALER + 1);
}


float32 IfxAsclin_getShiftFrequency(Ifx_ASCLIN *asclin)
{
    return IfxAsclin_getOvsFrequency(asclin) / asclin->BITCON.B.OVERSAMPLING;
}


volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerEr(Ifx_ASCLIN *asclin)
{
    return (volatile Ifx_SRC_SRCR *)(&MODULE_SRC.ASCLIN.ASCLIN[IfxAsclin_getIndex(asclin)].ERR);
}


volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerRx(Ifx_ASCLIN *asclin)
{
    return (volatile Ifx_SRC_SRCR *)(&MODULE_SRC.ASCLIN.ASCLIN[IfxAsclin_getIndex(asclin)].RX);
}


volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerTx(Ifx_ASCLIN *asclin)
{
    return (volatile Ifx_SRC_SRCR *)(&MODULE_SRC.ASCLIN.ASCLIN[IfxAsclin_getIndex(asclin)].TX);
}


uint32 IfxAsclin_read16(Ifx_ASCLIN *asclin, uint16 *data, uint32 count)
{
    volatile Ifx_ASCLIN_RXDATA *rxData = (volatile Ifx_ASCLIN_RXDATA *)&asclin->RXDATA.U;

    while (count > 0)
    {
        *data++ = (uint16)rxData->U;
        count--;
    }

    return count;
}


uint32 IfxAsclin_read32(Ifx_ASCLIN *asclin, uint32 *data, uint32 count)
{
    volatile Ifx_ASCLIN_RXDATA *rxData = (volatile Ifx_ASCLIN_RXDATA *)&asclin->RXDATA.U;

    while (count > 0)
    {
        *data++ = rxData->U;
        count--;
    }

    return count;
}


uint32 IfxAsclin_read8(Ifx_ASCLIN *asclin, uint8 *data, uint32 count)
{
    volatile Ifx_ASCLIN_RXDATA *rxData = (volatile Ifx_ASCLIN_RXDATA *)&asclin->RXDATA.U;

    while (count > 0)
    {
        *data++ = (uint8)rxData->U;
        count--;
    }

    return count;
}


void IfxAsclin_resetModule(Ifx_ASCLIN *asclin)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    asclin->KRST0.B.RST = 1;            /* Only if both Kernel reset bits are set a reset is executed */
    asclin->KRST1.B.RST = 1;
    IfxScuWdt_setCpuEndinit(passwd);

    while (0 == asclin->KRST0.B.RSTSTAT)    /* Wait until reset is executed */
    {}

    IfxScuWdt_clearCpuEndinit(passwd);
    asclin->KRSTCLR.B.CLR = 1;          /* Clear Kernel reset status bit */

    IfxScuWdt_setCpuEndinit(passwd);
}


void IfxAsclin_setBaudrateBitFields(Ifx_ASCLIN *asclin, uint16 prescaler, uint16 numerator, uint16 denominator, IfxAsclin_OversamplingFactor oversampling)
{
    IfxAsclin_ClockSource clockSource = (IfxAsclin_ClockSource)IfxAsclin_getClockSource(asclin); /* gets the current clock source */
    IfxAsclin_setClockSource(asclin, IfxAsclin_ClockSource_noClock);                             /* turns off the clock for settings */
    IfxAsclin_setPrescaler(asclin, prescaler);                                                   /* sets the prescaler*/
    IfxAsclin_setNumerator(asclin, numerator);                                                   /* sets the numerator*/
    IfxAsclin_setDenominator(asclin, denominator);                                               /* sets the denominator*/
    IfxAsclin_setOversampling(asclin, oversampling);                                             /* sets the oversampling*/
    IfxAsclin_setClockSource(asclin, clockSource);                                               /* sets the clock source back on*/
}


boolean IfxAsclin_setBitTiming(Ifx_ASCLIN *asclin, float32 baudrate, IfxAsclin_OversamplingFactor oversampling, IfxAsclin_SamplePointPosition samplepoint, IfxAsclin_SamplesPerBit medianFilter)
{
    IfxAsclin_ClockSource source = (IfxAsclin_ClockSource)IfxAsclin_getClockSource(asclin);
    float32               fOvs;
    uint32                d      = 0, n, dBest = 1, nBest = 1;
    float32               f;

    /* Set the PD frequency */
    float32               fpd = IfxAsclin_getPdFrequency(asclin);
    oversampling = (IfxAsclin_OversamplingFactor)__maxu((oversampling + 1), 4);
    samplepoint  = (IfxAsclin_SamplePointPosition)__maxu(samplepoint, 1);
    fOvs         = baudrate * oversampling;
    float32               relError   = fOvs;
    float32               limit      = 0.001 * fOvs;                     // save the error limit

    boolean               terminated = FALSE;
    float32               newRelError;                                   //modified by Hassan
    uint32                adder_facL, adder_facH, adder_facL_min, count; //modified by Hassan

    d = (uint32)(fpd / fOvs);
    n = 1;

    if (d >> 12)
    {
        if (((uint32)(fpd / (baudrate * 16))) >> 12)
        {
            /* Increase the value of the prescalar to generate the required baudrate */
            IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, FALSE);
        }
        else
        {
            /* Increase the value of the oversampling to generate the required baudrate */
            IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, FALSE);
        }
    }

    dBest          = d;
    nBest          = n;
    adder_facL_min = 0;
    f              = (fpd * n) / d;
    relError       = __absf(fOvs - f);

    if (relError <= limit)
    {
        terminated = TRUE;
    }

    for (n = 2; ((!terminated) && ((n * d) <= 0xFFF)); n++)
    {
        if (n == 2)
        {
            adder_facL = 1;
            adder_facH = 1;
        }
        else
        {
            adder_facL = (adder_facL_min * n) / nBest;
            adder_facH = adder_facL + 1;
        }

        for (count = adder_facL; count <= adder_facH; count++)
        {
            f           = (fpd * n) / (n * d + count);
            newRelError = __absf(fOvs - f);

            if (relError > (newRelError))
            {
                relError       = newRelError;
                nBest          = n;
                dBest          = (n * d + count);
                adder_facL_min = count;
            }
        }

        if (relError <= limit)
        {
            break;
        }
    }

    IfxAsclin_setClockSource(asclin, IfxAsclin_ClockSource_noClock);
    asclin->BRG.B.DENOMINATOR = dBest;
    asclin->BRG.B.NUMERATOR   = nBest;

    /* Set the SHIFT frequency */
    asclin->BITCON.B.OVERSAMPLING = oversampling - 1;

    /* Set the sampling point */
    asclin->BITCON.B.SAMPLEPOINT = samplepoint;

    /* Set the median filter */
    asclin->BITCON.B.SM = medianFilter ? 1 : 0;

    IfxAsclin_setClockSource(asclin, source);

    return TRUE;
}


void IfxAsclin_setClockSource(Ifx_ASCLIN *asclin, IfxAsclin_ClockSource clockSource)
{
    asclin->CSR.B.CLKSEL = clockSource; /* selects the given clock source*/

    /* Waits TW or polls for CSR.CON to change */
    if (clockSource == IfxAsclin_ClockSource_noClock)
    {
        while (IfxAsclin_getClockStatus(asclin) != 0U)
        {}
    }
    else
    {
        while (IfxAsclin_getClockStatus(asclin) != 1U)
        {}
    }
}


uint32 IfxAsclin_write16(Ifx_ASCLIN *asclin, uint16 *data, uint32 count)
{
    volatile Ifx_ASCLIN_TXDATA *txData = (volatile Ifx_ASCLIN_TXDATA *)&asclin->TXDATA.U;

    while ((count > 0))
    {
        txData->U = *data++;
        count--;
    }

    return count;
}


uint32 IfxAsclin_write32(Ifx_ASCLIN *asclin, uint32 *data, uint32 count)
{
    volatile Ifx_ASCLIN_TXDATA *txData = (volatile Ifx_ASCLIN_TXDATA *)&asclin->TXDATA.U;

    while ((count > 0))
    {
        txData->U = *data++;
        count--;
    }

    return count;
}


uint32 IfxAsclin_write8(Ifx_ASCLIN *asclin,const uint8 *data, uint32 count)
{
    volatile Ifx_ASCLIN_TXDATA *txData = (volatile Ifx_ASCLIN_TXDATA *)&asclin->TXDATA.U;

    while ((count > 0))
    {
        txData->U = *data++;
        count--;
        while(asclin->TXFIFOCON.B.FILL == 16);
    }

    return count;
}
