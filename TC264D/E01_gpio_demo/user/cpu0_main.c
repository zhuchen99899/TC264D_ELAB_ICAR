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
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "elab_assert.h"
#include "elab_log.h"
#include "elab_export.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// *************************** ����Ӳ������˵�� ***************************
// ���İ��������缴�� �����������
// ���ʹ��������� �������Ҫ�õ�ع���


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ϵ�
// 2.���Կ������İ����ĸ� LED ����ˮ��״��˸
// 3.�� SWITCH1 / SWITCH2 �����궨���Ӧ�����ŷֱ��� 00 01 10 11 ����Ͻӵ� 1-VCC 0-GND ���߲�����Ӧ����Ĳ��뿪��
// 3.��ͬ������£��ĸ� LED ��ˮ��״��˸��Ƶ�ʻᷢ���仯
// 4.�� KEY1 / KEY2 / KEY3 / KEY4 �����궨���Ӧ�����Žӵ� 1-VCC 0-GND ���� ����Ӧ����
// 5.�������Ž� GND ���� �������»�ʹ������LEDһ����˸ �ɿ���ָ���ˮ��
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************
#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)

#define KEY1                    (P20_6)
#define KEY2                    (P20_7)
#define KEY3                    (P11_2)
#define KEY4                    (P11_3)

#define SWITCH1                 (P33_11)
#define SWITCH2                 (P33_12)
ELAB_TAG("Main_cpu0");


uint16 delay_time = 0;
uint8 led_state = 0;


void funcA(uint32_t *handle)
{
    elog_debug("funcA");
}
INIT_EXPORT(funcA,EXPORT_DRVIVER);
int core0_main(void)
{


    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������





    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED3, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED3 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED4, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED4 ��� Ĭ�ϸߵ�ƽ �������ģʽ

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY4 ���� Ĭ�ϸߵ�ƽ ��������

    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_PULL_UP);        // ��ʼ�� SWITCH1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_PULL_UP);        // ��ʼ�� SWITCH2 ���� Ĭ�ϸߵ�ƽ ��������

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����


    elab_run();
	while (TRUE)
	{


        // �˴���д��Ҫѭ��ִ�еĴ���
	    delay_time = 300;
	    if(gpio_get_level(SWITCH1)) delay_time /= 2;
	    if(gpio_get_level(SWITCH2)) delay_time /= 2;
	    if( !gpio_get_level(KEY1) || !gpio_get_level(KEY2) || !gpio_get_level(KEY3) || !gpio_get_level(KEY4) )         // ��ȡ KEYx ��ƽΪ��
	    {
	        gpio_set_level(LED1, led_state);
	        gpio_set_level(LED2, led_state);
	        gpio_set_level(LED3, led_state);
	        gpio_set_level(LED4, led_state);
	    }
	    else
	    {
            gpio_set_level(LED1, led_state);
            gpio_set_level(LED2, !led_state);
            gpio_set_level(LED3, led_state);
            gpio_set_level(LED4, !led_state);
	    }
	    led_state = !led_state;


	    system_delay_ms(delay_time);
        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1��LED ����˸
//      ���ʹ��������ԣ��������Ҫ�õ�ع���
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
// ����2��SWITCH1 / SWITCH2 ���������ˮ��Ƶ���ޱ仯
//      ���ʹ��������ԣ��������Ҫ�õ�ع���
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
//      ���ñ����Ӧ SWITCH1 / SWITCH2 ���ŵ�ѹ�Ƿ������仯���Ƿ�������źŲ����������Ƿ�Ӵ�
// ����2��KEY1 / KEY2 / KEY3 / KEY4 ��GND���߰��������ޱ仯
//      ���ʹ��������ԣ��������Ҫ�õ�ع���
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
//      ���ñ����Ӧ KEY1 / KEY2 / KEY3 / KEY4 ���ŵ�ѹ�Ƿ������仯���Ƿ�������źŲ����������Ƿ�Ӵ�

