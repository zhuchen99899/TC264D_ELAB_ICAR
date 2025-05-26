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
* �ļ�����          zf_device_imu660rx
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2025-03-6        SeekFree            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCL/SPC           �鿴 zf_device_imu660rx.h �� IMU660RX_SPC_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660rx.h �� IMU660RX_SDI_PIN �궨��
*                   SA0/SDO           �鿴 zf_device_imu660rx.h �� IMU660RX_SDO_PIN �궨��
*                   CS                �鿴 zf_device_imu660rx.h �� IMU660RX_CS_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*
*                   // ��� IIC ����
*                   SCL/SPC           �鿴 zf_device_imu660rx.h �� IMU660RX_SCL_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660rx.h �� IMU660RX_SDA_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_driver_delay.h"
#include "zf_driver_spi.h"
#include "zf_driver_gpio.h"
#include "zf_driver_soft_iic.h"
#include "zf_device_config.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu660rb.h"

#include "zf_device_imu660rx.h"


int16 imu660rx_gyro_x = 0, imu660rx_gyro_y = 0, imu660rx_gyro_z = 0;            // ��������������   gyro (������)
int16 imu660rx_acc_x = 0, imu660rx_acc_y = 0, imu660rx_acc_z = 0;               // ������ٶȼ�����  acc  (accelerometer ���ٶȼ�)

uint8 imu660rx_self_check_id = 0;
uint8  imu660rx_acc_address = 0, imu660rx_gyro_address = 0;

#if IMU660RX_USE_SOFT_IIC
static soft_iic_info_struct imu660rx_iic_struct;
#define imu660rx_write_register(reg, data)        (soft_iic_write_8bit_register (&imu660rx_iic_struct, (reg), (data)))
#define imu660rx_write_registers(reg, data, len)  (soft_iic_write_8bit_registers(&imu660rx_iic_struct, (reg), (data), (len)))
#define imu660rx_read_registers(reg, data, len)   (soft_iic_read_8bit_registers (&imu660rx_iic_struct, (reg), (data), (len)))

#else

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RX д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rx_write_register(IMU660RB_PWR_CONF, 0x00);                   // �رո߼�ʡ��ģʽ
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rx_write_register(uint8 reg, uint8 data)
{
    IMU660RX_CS(0);
    spi_write_8bit_register(IMU660RX_SPI, reg | IMU660RX_SPI_W, data);
    IMU660RX_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RX д����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rx_write_registers(IMU660RA_INIT_DATA, imu660ra_config_file, sizeof(imu660ra_config_file));
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rx_write_registers(uint8 reg, const uint8 *data, uint32 len)
{
    IMU660RX_CS(0);
    spi_write_8bit_registers(IMU660RX_SPI, reg | IMU660RX_SPI_W, data, len);
    IMU660RX_CS(1);
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RX ������
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu660rx_read_registers(IMU660RB_ACC_ADDRESS, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rx_read_registers(uint8 reg, uint8 *data, uint32 len)
{
    uint8 temp_data[8];

    IMU660RB_CS(0);
    if (imu660rx_self_check_id == IMU660RX_CHIP_IDA)
    {
        spi_read_8bit_registers(IMU660RX_SPI, reg | IMU660RX_SPI_R, temp_data, len + 1);
        IMU660RB_CS(1);
        for(int i = 0; i < len; i ++)
        {
            *(data ++) = temp_data[i + 1];
        }
    }
    else if (imu660rx_self_check_id == IMU660RX_CHIP_IDB)
    {
        spi_read_8bit_registers(IMU660RX_SPI, reg | IMU660RX_SPI_R, data, len);
        IMU660RB_CS(1);
    }

}

#endif

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RX ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     bit             �Ĵ���λ��
// ���ز���     uint8           ����
// ʹ��ʾ��     imu660rx_read_register(IMU660RA_CHIP_ID1,2);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rx_read_register(uint8 reg, uint8 bit)
{
    uint8 data[2];
#if IMU660RX_USE_SOFT_IIC
    return soft_iic_read_8bit_register (&imu660rx_iic_struct, (reg));
#else
    IMU660RX_CS(0);
    spi_read_8bit_registers(IMU660RX_SPI, reg | IMU660RX_SPI_R, data, bit);
    IMU660RX_CS(1);
#endif

    return data[bit - 1];
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RX �Լ�
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu660rx_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rx_self_check (void)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;
    do
    {
        if(timeout_count ++ > IMU660RX_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
#if IMU660RX_USE_SOFT_IIC
        imu660rx_iic_struct.addr = IMU660RA_DEV_ADDR;
#endif
        dat = imu660rx_read_register(IMU660RX_CHIP_IDA,2);
        system_delay_ms(1);
        if(0x24 == dat)                                             // ��ȡ�豸ID1�Ƿ����0X24���������0X24����Ϊû��⵽�豸
        {
            imu660rx_self_check_id = IMU660RX_CHIP_IDA;             //��¼��ȡ�����豸ID
            break;
        }
#if IMU660RX_USE_SOFT_IIC
        imu660rx_iic_struct.addr = IMU660RB_DEV_ADDR;
#endif
        dat = imu660rx_read_register(IMU660RX_CHIP_IDB,1);
        system_delay_ms(1);
        if(0x6B == dat)                                              // ��ȡ�豸ID2�Ƿ����0X6B���������0X6B����Ϊû��⵽�豸
        {
            imu660rx_self_check_id = IMU660RX_CHIP_IDB;               //��¼��ȡ�����豸ID
            break;
        }
    }while(1);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RX ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rx_get_acc();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us        �ɼ����ٶȼƵ�ʱ����ɼ������ǵ�ʱ��һ�µ�ԭ���Ƕ�ֻ�Ƕ�ȡ�Ĵ�������
//-------------------------------------------------------------------------------------------------------------------
void imu660rx_get_acc (void)
{
    uint8 dat[6];

    imu660rx_read_registers(imu660rx_acc_address, dat, 6);
    imu660rx_acc_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rx_acc_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rx_acc_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RX ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rx_get_gyro();                                            // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us
//-------------------------------------------------------------------------------------------------------------------
void imu660rx_get_gyro (void)
{
    uint8 dat[6];

    imu660rx_read_registers(imu660rx_gyro_address, dat, 6);
    imu660rx_gyro_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rx_gyro_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rx_gyro_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}



//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RX ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     gyro_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float data = imu660rx_acc_transition(imu660rx_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660rx_acc_transition (int16 acc_value)
{
    float acc_data = 0;

    if (imu660rx_self_check_id == IMU660RX_CHIP_IDA)
    {
        switch(IMU660RA_ACC_SAMPLE)
        {
            case 0x00: acc_data = (float)acc_value / 16384; break;                  // 0x00 ���ٶȼ�����Ϊ:��2g     ��ȡ���ļ��ٶȼ����� ���� 16384     ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            case 0x01: acc_data = (float)acc_value / 8192; break;                   // 0x01 ���ٶȼ�����Ϊ:��4g     ��ȡ���ļ��ٶȼ����� ���� 8192      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            case 0x02: acc_data = (float)acc_value / 4096; break;                   // 0x02 ���ٶȼ�����Ϊ:��8g     ��ȡ���ļ��ٶȼ����� ���� 4096      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            case 0x03: acc_data = (float)acc_value / 2048; break;                   // 0x03 ���ٶȼ�����Ϊ:��16g    ��ȡ���ļ��ٶȼ����� ���� 2048      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            default: break;
        }
    }
    else if(imu660rx_self_check_id == IMU660RX_CHIP_IDB)
    {
        switch(IMU660RB_ACC_SAMPLE)
        {
            case 0x30: acc_data = (float)acc_value / 16393; break;                  // 0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ���� 16393 ������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            case 0x38: acc_data = (float)acc_value / 8197;  break;                  // 0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ���� 8197 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            case 0x3C: acc_data = (float)acc_value / 4098;  break;                  // 0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ���� 4098 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            case 0x34: acc_data = (float)acc_value / 2049;  break;                  // 0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ���� 2049 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            default: break;
        }
    }
    return acc_data;
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RX ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float data = imu660rx_gyro_transition(imu660rx_gyro_x);         // ��λΪ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660rx_gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
    if (imu660rx_self_check_id == IMU660RX_CHIP_IDA)
    {
        switch(IMU660RA_GYR_SAMPLE)
        {
            case 0x00: gyro_data = (float)gyro_value / 16.4f;  break;               //  0x00 ����������Ϊ:��2000dps    ��ȡ�������������ݳ��� 16.4    ����ת��Ϊ������λ������ ��λΪ����/s
            case 0x01: gyro_data = (float)gyro_value / 32.8f;  break;               //  0x01 ����������Ϊ:��1000dps    ��ȡ�������������ݳ��� 32.8    ����ת��Ϊ������λ������ ��λΪ����/s
            case 0x02: gyro_data = (float)gyro_value / 65.6f;  break;               //  0x02 ����������Ϊ:��500 dps    ��ȡ�������������ݳ��� 65.6    ����ת��Ϊ������λ������ ��λΪ����/s
            case 0x03: gyro_data = (float)gyro_value / 131.2f; break;               //  0x03 ����������Ϊ:��250 dps    ��ȡ�������������ݳ��� 131.2   ����ת��Ϊ������λ������ ��λΪ����/s
            case 0x04: gyro_data = (float)gyro_value / 262.4f; break;               //  0x04 ����������Ϊ:��125 dps    ��ȡ�������������ݳ��� 262.4   ����ת��Ϊ������λ������ ��λΪ����/s
            default: break;
        }
    }
    else if (imu660rx_self_check_id == IMU660RX_CHIP_IDB)
    {
        switch(IMU660RB_GYR_SAMPLE)
        {
            case 0x52: gyro_data = (float)gyro_value / 228.6f;  break;              //  0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ��� 228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
            case 0x50: gyro_data = (float)gyro_value / 114.3f;  break;              //  0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ��� 114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
            case 0x54: gyro_data = (float)gyro_value / 57.1f;   break;              //  0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ��� 57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            case 0x58: gyro_data = (float)gyro_value / 28.6f;   break;              //  0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ��� 28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            case 0x5C: gyro_data = (float)gyro_value / 14.3f;   break;              //  0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ��� 14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            case 0x51: gyro_data = (float)gyro_value / 7.1f;    break;              //  0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ��� 7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s
            default: break;
        }
    }
    return gyro_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� IMU660RX
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     imu660rx_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rx_init (void)
{
    uint8 return_state = 0;
    system_delay_ms(20);                                                        // �ȴ��豸�ϵ�ɹ�

#if IMU660RX_USE_SOFT_IIC
    soft_iic_init(&imu660rx_iic_struct, IMU660RX_DEV_ADDR, IMU660RX_SOFT_IIC_DELAY, IMU660RX_SCL_PIN, IMU660RX_SDA_PIN);        // ���� IMU660RX �� IIC �˿�
#else
    spi_init(IMU660RX_SPI, SPI_MODE0, IMU660RX_SPI_SPEED, IMU660RX_SPC_PIN, IMU660RX_SDI_PIN, IMU660RX_SDO_PIN, SPI_CS_NULL);   // ���� IMU660RX �� SPI �˿�
    gpio_init(IMU660RX_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);                  // ���� IMU660RX �� CS �˿�

#endif

    do{
        if(imu660rx_self_check())                                               // IMU660RX �Լ�
        {
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� IMU660RX �Լ������ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���
            zf_log(0, "imu660rx self check error.");
            return_state = 1;
            break;
        }

        if (imu660rx_self_check_id == IMU660RX_CHIP_IDA)
        {
            imu660rx_acc_address = IMU660RA_ACC_ADDRESS;
            imu660rx_gyro_address = IMU660RA_GYRO_ADDRESS;
            imu660rx_write_register(IMU660RA_PWR_CONF, 0x00);                       // �رո߼�ʡ��ģʽ
            system_delay_ms(1);
            imu660rx_write_register(IMU660RA_INIT_CTRL, 0x00);                      // ��ʼ��ģ����г�ʼ������
            imu660rx_write_registers(IMU660RA_INIT_DATA, imu660ra_config_file, sizeof(imu660ra_config_file));   // ��������ļ�
            imu660rx_write_register(IMU660RA_INIT_CTRL, 0x01);                      // ��ʼ�����ý���
            system_delay_ms(20);
            if(imu660rx_read_register(IMU660RA_INT_STA,2) == 0)                       // ����Ƿ��������
            {
                // �������������˶�����Ϣ ������ʾ����λ��������
                // ��ô���� IMU660RA ���ó�ʼ���ļ�������
                // ���һ�½�����û������ ���û������ܾ��ǻ���
                zf_log(0, "imu660ra init error.");
                return_state = 1;
                break;
            }
            imu660rx_write_register(IMU660RA_PWR_CTRL, 0x0E);                       // ��������ģʽ  ʹ�������ǡ����ٶȡ��¶ȴ�����
            imu660rx_write_register(IMU660RA_ACC_CONF, 0xA7);                       // ���ٶȲɼ����� ����ģʽ �����ɼ� 50Hz  ����Ƶ��
            imu660rx_write_register(IMU660RA_GYR_CONF, 0xA9);                       // �����ǲɼ����� ����ģʽ �����ɼ� 200Hz ����Ƶ��
            imu660rx_write_register(IMU660RA_ACC_RANGE, IMU660RA_ACC_SAMPLE);       // ���ٶ��������� ��������Ϊ:��8g
            imu660rx_write_register(IMU660RA_GYR_RANGE, IMU660RA_GYR_SAMPLE);       // �������������� ��������Ϊ:��2000dps

            // IMU660RA_ACC_SAMPLE �Ĵ���
            // ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g         ��ȡ���ļ��ٶȼ����� ���� 16384   ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            // ����Ϊ:0x01 ���ٶȼ�����Ϊ:��4g         ��ȡ���ļ��ٶȼ����� ���� 8192    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            // ����Ϊ:0x02 ���ٶȼ�����Ϊ:��8g         ��ȡ���ļ��ٶȼ����� ���� 4096    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
            // ����Ϊ:0x03 ���ٶȼ�����Ϊ:��16g        ��ȡ���ļ��ٶȼ����� ���� 2048    ����ת��Ϊ������λ������ ��λ��g(m/s^2)

            // IMU660RA_GYR_RANGE �Ĵ���
            // ����Ϊ:0x00 ����������Ϊ:��2000dps     ��ȡ�������������� ���� 16.4       ����ת��Ϊ������λ������ ��λΪ����/s
            // ����Ϊ:0x01 ����������Ϊ:��1000dps     ��ȡ�������������� ���� 32.8       ����ת��Ϊ������λ������ ��λΪ����/s
            // ����Ϊ:0x02 ����������Ϊ:��500 dps     ��ȡ�������������� ���� 65.6       ����ת��Ϊ������λ������ ��λΪ����/s
            // ����Ϊ:0x03 ����������Ϊ:��250 dps     ��ȡ�������������� ���� 131.2      ����ת��Ϊ������λ������ ��λΪ����/s
            // ����Ϊ:0x04 ����������Ϊ:��125 dps     ��ȡ�������������� ���� 262.4      ����ת��Ϊ������λ������ ��λΪ����/s

        }
        else if (imu660rx_self_check_id == IMU660RX_CHIP_IDB)
        {

            imu660rx_acc_address = IMU660RB_ACC_ADDRESS;
            imu660rx_gyro_address = IMU660RB_GYRO_ADDRESS;
            imu660rx_write_register(IMU660RB_INT1_CTRL, 0x03);                      // ���������� ���ٶ����ݾ����ж�
            imu660rx_write_register(IMU660RB_CTRL1_XL, IMU660RB_ACC_SAMPLE);        // ���ü��ٶȼ����� ��8G �Լ������������ 52hz �Լ����ٶ���Ϣ�ӵ�һ���˲������
            // IMU660RB_CTRL1_XL �Ĵ���
            // ����Ϊ:0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ����16393������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            // ����Ϊ:0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ����8197�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            // ����Ϊ:0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ����4098�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
            // ����Ϊ:0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ����2049�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)

            imu660rx_write_register(IMU660RB_CTRL2_G, IMU660RB_GYR_SAMPLE);         // ���������Ǽ����� ��2000dps �Լ������������ 208hz
            // IMU660RB_CTRL2_G �Ĵ���
            // ����Ϊ:0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ���228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
            // ����Ϊ:0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ���114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
            // ����Ϊ:0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ���57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            // ����Ϊ:0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ���28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            // ����Ϊ:0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ���14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
            // ����Ϊ:0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ���7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s

            imu660rx_write_register(IMU660RB_CTRL3_C, 0x44);                        // ʹ�����������ֵ�ͨ�˲���
            imu660rx_write_register(IMU660RB_CTRL4_C, 0x02);                        // ʹ�����ֵ�ͨ�˲���
            imu660rx_write_register(IMU660RB_CTRL5_C, 0x00);                        // ���ٶȼ�����������������
            imu660rx_write_register(IMU660RB_CTRL6_C, 0x00);                        // �������ٶȼƸ�����ģʽ �����ǵ�ͨ�˲� 133hz
            imu660rx_write_register(IMU660RB_CTRL7_G, 0x00);                        // ���������Ǹ�����ģʽ �رո�ͨ�˲�
            imu660rx_write_register(IMU660RB_CTRL9_XL, 0x01);                       // �ر�I3C�ӿ�
        }

    }while(0);
    return return_state;
}


