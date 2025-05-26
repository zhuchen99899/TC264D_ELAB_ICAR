/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ���������������������� SDK ������������������
* Copyright (c) 2022 SEEKFREE ��������
*
* �������� TC264 ��������������
*
* TC264 ������ ����������
* ������������������������������ GPL��GNU General Public License���� GNU����������������������
* �� GPL ����3������ GPL3.0������������������������������������������/��������
*
* ��������������������������������������������������������
* ����������������������������������������
* �������������� GPL
*
* ���������������������������������� GPL ������
* ����������������<https://www.gnu.org/licenses/>
*
* ����������
* ������������ GPL3.0 �������������� ����������������������
* ���������������� libraries/doc ���������� GPL3_permission_statement.txt ������
* ������������ libraries �������� �������������� LICENSE ����
* ������������������������ ��������������������������������������������������
*
* ��������          zf_common_headfile
* ��������          ��������������������
* ��������          ���� libraries/doc �������� version ���� ��������
* ��������          ADS v1.10.2
* ��������          TC264D
* ��������          https://seekfree.taobao.com/
*
* ��������
* ����              ����                ����
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_

//===================================================C���� ������===================================================
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//===================================================C���� ������===================================================

//===================================================���� SDK ����===================================================
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"
//===================================================���� SDK ����===================================================

//====================================================������������====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "isr_config.h"
//====================================================������������====================================================

//===================================================��������������===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_spi.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
//===================================================��������������===================================================

//===================================================��������������===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_ble6a20.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_gnss.h"
#include "zf_device_camera.h"
#include "zf_device_dl1a.h"
#include "zf_device_dl1b.h"
#include "zf_device_icm20602.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu660rb.h"
#include "zf_device_imu660rx.h"
#include "zf_device_imu963ra.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_key.h"
#include "zf_device_menc15a.h"
#include "zf_device_mpu6050.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_oled.h"
#include "zf_device_ov7725.h"
#include "zf_device_scc8660.h"
#include "zf_device_tft180.h"
#include "zf_device_tsl1401.h"
#include "zf_device_type.h"
#include "zf_device_uart_receiver.h"
#include "zf_device_virtual_oscilloscope.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wifi_spi.h"
#include "zf_device_wireless_uart.h"
//===================================================��������������===================================================

//====================================================����������====================================================
#include "seekfree_assistant.h"
#include "seekfree_assistant_interface.h"
//====================================================����������====================================================

//=====================================================������======================================================

//=====================================================������======================================================

#endif

