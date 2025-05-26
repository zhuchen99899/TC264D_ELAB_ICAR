/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_device_menc15a
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-11-19       pudding            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   CLK               �鿴 zf_device_menc15a.h �� MENC15A_CLK_PIN   �궨��
*                   MOSI              �鿴 zf_device_menc15a.h �� MENC15A_MOSI_PIN  �궨��
*                   MISO              �鿴 zf_device_menc15a.h �� MENC15A_MISO_PIN  �궨��
*                   CS                �鿴 zf_device_menc15a.h �� MENC15A_CS_PIN    �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/


#ifndef _zf_device_menc15a_h_
#define _zf_device_menc15a_h_

#include "zf_common_typedef.h"

//================================================���� MENC15A ��������================================================

//====================================================Ӳ�� SPI ����====================================================
#define MENC15A_1_SPI_SPEED            (20 * 1000 * 1000)                        // �ű�����1 Ӳ�� SPI ����
#define MENC15A_1_SPI                  (SPI_0           )                        // �ű�����1 Ӳ�� SPI ��
#define MENC15A_1_CLK_PIN              (SPI0_SCLK_P20_11)                        // �ű�����1 Ӳ�� SPI SCK ����
#define MENC15A_1_MOSI_PIN             (SPI0_MOSI_P20_14)                        // �ű�����1 Ӳ�� SPI MOSI ����
#define MENC15A_1_MISO_PIN             (SPI0_MISO_P20_12)                        // �ű�����1 Ӳ�� SPI MISO ����
#define MENC15A_1_CS_PIN               (P20_13)                                  // �ű�����1 CS Ƭѡ����
#define MENC15A_1_CS(x)                ((x) ? (gpio_high(MENC15A_1_CS_PIN)) : (gpio_low(MENC15A_1_CS_PIN)))


#define MENC15A_2_SPI_SPEED            (20 * 1000 * 1000)                        // �ű�����2 Ӳ�� SPI ����
#define MENC15A_2_SPI                  (SPI_0           )                        // �ű�����2 Ӳ�� SPI ��
#define MENC15A_2_CLK_PIN              (SPI0_SCLK_P20_11  )                      // �ű�����2 Ӳ�� SPI SCK ����
#define MENC15A_2_MOSI_PIN             (SPI0_MOSI_P20_14 )                       // �ű�����2 Ӳ�� SPI MOSI ����
#define MENC15A_2_MISO_PIN             (SPI0_MISO_P20_12 )                       // �ű�����2 Ӳ�� SPI MISO ����
#define MENC15A_2_CS_PIN               (P20_15)                                  // �ű�����2 CS Ƭѡ����
#define MENC15A_2_CS(x)                ((x) ? (gpio_high(MENC15A_2_CS_PIN)) : (gpio_low(MENC15A_2_CS_PIN)))

//====================================================Ӳ�� SPI ����====================================================

// ö�ٴű���ģ��
typedef enum 
{
	menc15a_1_module,
    menc15a_2_module,
        
}menc15a_module_enum;

//================================================���� MENC15A ��������================================================


extern uint16 menc15a_absolute_data[2];

extern int16 menc15a_absolute_offset_data[2];

extern int16  menc15a_speed_data[2];


uint16 menc15a_get_absolute_data(menc15a_module_enum menc15a_module);
int16  menc15a_get_speed_data(menc15a_module_enum menc15a_module);

uint8  menc15a_init(void);


#endif
